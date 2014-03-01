
/// @file IguGen2.cc
/// @brief IguGen2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "IguGen2.h"
#include "IgVar.h"
#include "RegVect.h"
#include "Variable.h"
#include "logic/Bdd.h"
#include "logic/BddMgr.h"
#include <signal.h>

BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

// 変数を並び替えるための比較関数
struct IgVarGt
{
  bool
  operator()(IgVar* left,
	     IgVar* right)
  {
    return left->ds_count() > right->ds_count();
  }
};

bool timeout = false;

void
timer_handler(int signum)
{
  cerr << "time expired" << endl;
  timeout = true;
}

END_NONAMESPACE

// @brief コンストラクタ
IguGen2::IguGen2() :
  mBddMgr("bmc", "Bdd Manager for IguGen2"),
  mAlloc(sizeof(IgVar), 1024)
{
  mBranchLimit = 0;
  mTimeLimit = 0;
  mDebug = 0;

  BddMgrParam param;
  param.mMemLimit = 2 * 1000 * 1000 * 1000; // 2G
  mBddMgr.param(param, BddMgrParam::MEM_LIMIT);
}

// @brief デストラクタ
IguGen2::~IguGen2()
{
}

// @brief ベクタのリストをセットする．
void
IguGen2::set_vector_list(const vector<const RegVect*>& vector_list)
{
  mVectorList = vector_list;
}

// @brief 変数を求める．
// @param[in] variable_list 変数のリスト
// @param[in] solution 解として選ばれた変数を格納するリスト
void
IguGen2::solve(const vector<const Variable*>& variable_list,
	       ymuint best_so_far,
	       vector<const Variable*>& solution)
{
  assert_cond( !mVectorList.empty(), __FILE__, __LINE__);
  mVectorLength = mVectorList[0]->size();

  mDsAll = mBddMgr.make_zero();

  // IgVar を作る．
  ymuint nv = variable_list.size();
  vector<IgVar*> ig_var_list(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    const Variable* var = variable_list[i];
    Bdd ds = calc_ds_func(var);
    IgVar* ig_var = new_IgVar(var, ds);
    mDsAll |= ds;
    ig_var_list[i] = ig_var;
  }
  mDsAllCount = elem_count(mDsAll);

  // mDsCount の降順に並び替える．
  sort(ig_var_list.begin(), ig_var_list.end(), IgVarGt());

  mBestSoFar = best_so_far;
  mSelectedVariables.clear();
  timeout = false;

  // タイマーの設定
  struct sigaction old_act;
  timer_t tid;
  if ( mTimeLimit > 0 ) {
    struct sigaction act;
    act.sa_handler = timer_handler;
    act.sa_flags = 0;
    if ( sigaction(SIGALRM, &act, &old_act) < 0 ) {
      perror("sigaction()");
      exit(-1);
    }

    struct itimerspec itval;
    itval.it_value.tv_sec = mTimeLimit;
    itval.it_value.tv_nsec = 0;
    itval.it_interval.tv_sec = 0;
    itval.it_interval.tv_nsec = 0;

    if ( timer_create(CLOCK_REALTIME, NULL, &tid) < 0 ) {
      perror("timer_create()");
      exit(-1);
    }
    if ( timer_settime(tid, 0, &itval, NULL) < 0 ) {
      perror("timer_settime()");
      exit(-1);
    }
  }

  solve_recur(mBddMgr.make_zero(),
	      ig_var_list, 0);
  solution = mSolutionSoFar;

  if ( mTimeLimit > 0 ) {
    timer_delete(tid);
    sigaction(SIGALRM, &old_act, NULL);
  }
}

// @brief 分岐制限を設定する．
// @param[in] limit 分岐制限
// @note limit = 0 の場合には制限なし
void
IguGen2::set_branch_limit(ymuint limit)
{
  mBranchLimit = limit;
}

// @brief 時間制限を設定する．
// @param[in] limit_min 制限の分の部分
// @param[in] limit_sec 制限の秒の部分
void
IguGen2::set_time_limit(ymuint32 limit_min,
			ymuint32 limit_sec)
{
  mTimeLimit = limit_min * 60 + limit_sec;
}

// @brief デバッグレベルを指定する．
void
IguGen2::set_debug_level(ymuint32 level)
{
  mDebug = level;
}

// @brief solve() の下請け関数
// @param[in] ds_so_far 既に区別されている対の集合を表す BDD
// @param[in] var_list 使用可能な変数のリスト
// @param[in] pos var_list の開始位置
void
IguGen2::solve_recur(Bdd ds_so_far,
		     const vector<IgVar*>& var_list,
		     ymuint pos)
{
  if ( mDebug > 0 ) {
    cerr << "best so far = " << mBestSoFar << endl
	 << "selected variables = ";
    for (vector<const Variable*>::const_iterator p = mSelectedVariables.begin();
	 p != mSelectedVariables.end(); ++ p) {
      cerr << " ";
      const Variable* var = *p;
      var->dump(cerr);
    }
    cerr << endl;
    cerr << "remain variables = ";
    ymuint nv = var_list.size();
    for (ymuint i = pos; i < nv; ++ i) {
      cerr << " ";
      const Variable* var = var_list[i]->variable();
      var->dump(cerr);
    }
    cerr << endl;
  }

  ymuint nv = var_list.size();

  // Distinguished Set の差分の降順にソートする．
  vector<IgVar*> new_variables;
  for (ymuint i = pos; i < nv; ++ i) {
    IgVar* var = var_list[i];
    Bdd ds_new = var->ds_func() & ~ds_so_far;
    if ( ds_new.is_zero() ) {
      // 新しい分割を含まない変数は除外する．
      continue;
    }
    IgVar* new_var = new_IgVar(var->variable(), ds_new);
    new_variables.push_back(new_var);
  }

  sort(new_variables.begin(), new_variables.end(), IgVarGt());

  ymuint nv2 = new_variables.size();

  vector<Bdd> ds_rest(nv2);
  for (ymuint i = 0; i < nv2; ++ i) {
    ymuint j = nv2 - i - 1;
    IgVar* var = new_variables[j];
    ds_rest[j] = var->ds_func();
    if ( j < nv2 - 1 ) {
      ds_rest[j] |= ds_rest[j + 1];
    }
  }

  ymuint ds_so_far_size = elem_count(ds_so_far);
  for (ymuint i = 0; i < nv2; ++ i) {
    IgVar* ig_var = new_variables[i];
    if ( mBranchLimit > 0 &&
	 i >= mBranchLimit &&
	 new_variables[mBranchLimit - 1]->ds_count() > ig_var->ds_count() ) {
      break;
    }

    // 残りの変数の DS をすべて足しても全体集合にならないとき
    // 枝刈りをする．
    Bdd ds_tmp = ds_so_far | ds_rest[i];
    if ( ds_tmp != mDsAll ) {
      break;
    }

    // mBestSoFar との差分の数の変数の要素数を足しても全体集合
    // の要素数に足りない時は枝刈りする．
    ymuint diff = mBestSoFar - mSelectedVariables.size();
    ymuint count = 0;
    ymuint tmp_size = ds_so_far_size;
    for (ymuint j = i; j < nv2 && count < diff; ++ j, ++ count) {
      IgVar* var = new_variables[j];
      tmp_size += var->ds_count();
    }
    if ( tmp_size < mDsAllCount ) {
      break;
    }

#if 0
    cout << "Choose Variable#" << (mSelectedVariables.size() + 1)
	 << ": ";
    ig_var->dump(cout);
    cout << endl;
#endif

    Bdd ds_new = ds_so_far | ig_var->ds_func();
    if ( ds_new == mDsAll ) {
      // すべての対を区別している．
      ymuint ans = mSelectedVariables.size() + 1;
      if ( ans < mBestSoFar ) {
	mSolutionSoFar = mSelectedVariables;
	mSolutionSoFar.push_back(ig_var->variable());
	mBestSoFar = ans;
	cerr << "best_so_far => " << mBestSoFar << endl;
      }
      continue;
    }

    if ( mSelectedVariables.size() + 2 < mBestSoFar ) {
      mSelectedVariables.push_back(ig_var->variable());
      solve_recur(ds_new, new_variables, i + 1);
      mSelectedVariables.pop_back();
    }

    if ( timeout ) {
      break;
    }
  }

  for (ymuint i = 0; i < nv2; ++ i) {
    delete_IgVar(new_variables[i]);
  }
}

// @brief 下界を計算する．
// @param[in] num 要素数
ymuint
IguGen2::lower_bound(ymuint num) const
{
  ymuint ans = 0;
  ymuint m = 1;
  while ( m < num ) {
    ++ ans;
    m <<= 1;
  }
  return ans;
}

// @brief IgVar を生成する．
IgVar*
IguGen2::new_IgVar(const Variable* var,
		   Bdd ds_func)
{
  void* p = mAlloc.get_memory(sizeof(IgVar));
  IgVar* ig_var = new (p) IgVar(var, ds_func, elem_count(ds_func));
  return ig_var;
}

// @brief IgVar を削除する．
void
IguGen2::delete_IgVar(IgVar* ig_var)
{
  ig_var->~IgVar();
  mAlloc.put_memory(sizeof(IgVar), ig_var);
}

// @brief ds_func を計算する．
Bdd
IguGen2::calc_ds_func(const Variable* var)
{
  ymuint n = mVectorList.size();
  vector<const RegVect*> list0;
  vector<const RegVect*> list1;
  list0.reserve(n);
  list1.reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    const RegVect* vect = mVectorList[i];
    if ( var->classify(vect) ) {
      list1.push_back(vect);
    }
    else {
      list0.push_back(vect);
    }
  }

  ymuint nv = mVectorLength;

  Bdd bdd1 = mBddMgr.make_one();
  Bdd f0 = vects_to_bdd(list0, 0, nv, bdd1);
  Bdd f1 = vects_to_bdd(list1, 0, nv, bdd1);

  Bdd ds_func = vects_to_bdd(list0, list1, 0, f1, f0);

  return ds_func;
}

// @brief 要素数を計算する．
ymuint
IguGen2::elem_count(Bdd f)
{
  mpz_class mpz_val = f.minterm_count(mVectorLength * 2);
  assert_cond( mpz_val.fits_uint_p(), __FILE__, __LINE__);
  return mpz_val.get_ui();
}

Bdd
IguGen2::vects_to_bdd(const vector<const RegVect*>& vect_list,
		      ymuint pos,
		      ymuint var_base,
		      Bdd leaf_bdd)
{
  if ( vect_list.empty() ) {
    return mBddMgr.make_zero();
  }

  ymuint nvars = vect_list[0]->size();
  if ( pos == nvars ) {
    return leaf_bdd;
  }

  ymuint n = vect_list.size();
  vector<const RegVect*> list0;
  vector<const RegVect*> list1;
  list0.reserve(n);
  list1.reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    const RegVect* vect = vect_list[i];
    if ( vect->val(pos) == 0 ) {
      list0.push_back(vect);
    }
    else {
      list1.push_back(vect);
    }
  }
  Bdd bdd0 = vects_to_bdd(list0, pos + 1, var_base, leaf_bdd);
  Bdd bdd1 = vects_to_bdd(list1, pos + 1, var_base, leaf_bdd);
  Bdd bdd = mBddMgr.make_bdd(VarId(var_base + pos), bdd0, bdd1);
  return bdd;
}

Bdd
IguGen2::vects_to_bdd(const vector<const RegVect*>& vect_list0,
		      const vector<const RegVect*>& vect_list1,
		      ymuint pos,
		      Bdd leaf0,
		      Bdd leaf1)
{
  if ( vect_list0.empty() ) {
    return vects_to_bdd(vect_list1, pos, 0, leaf1);
  }
  if ( vect_list1.empty() ) {
    return vects_to_bdd(vect_list0, pos, 0, leaf0);
  }

  ymuint nvars = vect_list0[0]->size();
  assert_cond( pos < nvars, __FILE__, __LINE__);

  ymuint n0 = vect_list0.size();
  vector<const RegVect*> list0_0;
  vector<const RegVect*> list0_1;
  list0_0.reserve(n0);
  list0_1.reserve(n0);
  for (ymuint i = 0; i < n0; ++ i) {
    const RegVect* vect = vect_list0[i];
    if ( vect->val(pos) == 0 ) {
      list0_0.push_back(vect);
    }
    else {
      list0_1.push_back(vect);
    }
  }

  ymuint n1 = vect_list1.size();
  vector<const RegVect*> list1_0;
  vector<const RegVect*> list1_1;
  list1_0.reserve(n1);
  list1_1.reserve(n1);
  for (ymuint i = 0; i < n1; ++ i) {
    const RegVect* vect = vect_list1[i];
    if ( vect->val(pos) == 0 ) {
      list1_0.push_back(vect);
    }
    else {
      list1_1.push_back(vect);
    }
  }

  Bdd bdd0 = vects_to_bdd(list0_0, list1_0, pos + 1, leaf0, leaf1);
  Bdd bdd1 = vects_to_bdd(list0_1, list1_1, pos + 1, leaf0, leaf1);
  Bdd bdd = mBddMgr.make_bdd(VarId(pos), bdd0, bdd1);
  return bdd;
}

END_NAMESPACE_YM_IGF
