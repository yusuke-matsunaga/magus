
/// @file IguGen.cc
/// @brief IguGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "IguGen.h"
#include "RegVect.h"
#include "VectSetList.h"
#include "Variable.h"
#include <signal.h>

BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

// 変数を並び替えるための比較関数
struct Lt
{
  bool
  operator()(const pair<ymuint, Variable*>& left,
	     const pair<ymuint, Variable*>& right)
  {
    return left < right;
  }
};

// 2の対数を計算する関数
ymuint
blog(ymuint n)
{
  ymuint ans = 0;
  ymuint m = 1;
  for ( ; m < n; ++ m) {
    ++ ans;
    m *= 2;
  }
  return ans;
}

bool timeout = false;

void
timer_handler(int signum)
{
  cerr << "time expired" << endl;
  timeout = true;
}

END_NONAMESPACE

// @brief コンストラクタ
IguGen::IguGen()
{
  mBranchLimit = 0;
  mTimeLimit = 0;
  mDebug = 0;
}

// @brief デストラクタ
IguGen::~IguGen()
{
}

// @brief 変数を求める．
// @param[in] vector_list ベクタのリスト
// @param[in] multi 多重度
// @param[in] variable_list 変数のリスト
// @param[in] solution 解として選ばれた変数を格納するリスト
void
IguGen::solve(const vector<RegVect*>& vector_list,
	      ymuint multi,
	      const vector<Variable*>& variable_list,
	      vector<Variable*>& solution)
{
  // imbalance measure の昇順に並び替える．
  ymuint nv = variable_list.size();
  vector<pair<ymuint, Variable*> > tmp_list(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    Variable* var = variable_list[i];
    ymuint n0 = 0;
    ymuint n1 = 0;
    for (vector<RegVect*>::const_iterator p = vector_list.begin();
	 p != vector_list.end(); ++ p) {
      RegVect* vect = *p;
      if ( var->classify(vect) == 0 ) {
	++ n0;
      }
      else {
	++ n1;
      }
    }
    ymuint im = n0 * n0 + n1 * n1;
    tmp_list[i] = make_pair(im, var);
  }
  sort(tmp_list.begin(), tmp_list.end(), Lt());
  vector<Variable*> new_variable_list(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    new_variable_list[i] = tmp_list[i].second;
  }

  mMulti = multi;
  mBestSoFar = nv + 1;
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

  VectSetList initial_list(vector_list.size(), 1);
  initial_list.add_set();
  for (vector<RegVect*>::const_iterator p = vector_list.begin();
       p != vector_list.end(); ++ p) {
    RegVect* vect = *p;
    initial_list.add_vect(vect);
  }

  solve_recur(initial_list,
	      new_variable_list.begin(), new_variable_list.end());
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
IguGen::set_branch_limit(ymuint limit)
{
  mBranchLimit = limit;
}

// @brief 時間制限を設定する．
// @param[in] limit_min 制限の分の部分
// @param[in] limit_sec 制限の秒の部分
void
IguGen::set_time_limit(ymuint32 limit_min,
		       ymuint32 limit_sec)
{
  mTimeLimit = limit_min * 60 + limit_sec;
}

// @brief デバッグレベルを指定する．
void
IguGen::set_debug_level(ymuint32 level)
{
  mDebug = level;
}

// @brief solve() の下請け関数
// @param[in] vector_list 分割されたベクタのリスト
// @param[in] var_begin 使用可能な変数のリストの先頭の反復子
// @param[in] var_end 使用可能な変数のリストの末尾の反復子
void
IguGen::solve_recur(const VectSetList& vector_list,
		    vector<Variable*>::const_iterator var_begin,
		    vector<Variable*>::const_iterator var_end)
{
  if ( mDebug > 0 ) {
    cerr << "best so far = " << mBestSoFar << endl
	 << "selected variables = ";
    for (vector<Variable*>::const_iterator p = mSelectedVariables.begin();
	 p != mSelectedVariables.end(); ++ p) {
      Variable* var = *p;
      const vector<ymuint>& vid_list = var->vid_list();
      cerr << " (";
      for (vector<ymuint>::const_iterator q = vid_list.begin();
	   q != vid_list.end(); ++ q) {
	cerr << " " << *q;
      }
      cerr << ")";
    }
    cerr << endl;
    cerr << "remain variables = ";
    for (vector<Variable*>::const_iterator p = var_begin;
	 p != var_end; ++ p) {
      Variable* var = *p;
      const vector<ymuint>& vid_list = var->vid_list();
      cerr << " (";
      for (vector<ymuint>::const_iterator q = vid_list.begin();
	   q != vid_list.end(); ++ q) {
	cerr << " " << *q;
      }
      cerr << ")";
    }
    cerr << endl;
    if ( mDebug > 1 ) {
      ymuint n = vector_list.set_num();
      for (ymuint i = 0; i < n; ++ i) {
	ymuint m = vector_list.set_size(i);
	for (ymuint j = 0; j < m; ++ j) {
	  RegVect* vect = vector_list.set_elem(i, j);
	  vect->dump(cerr);
	}
	cerr << "-----------------" << endl;
      }
    }
    else {
      ymuint n = vector_list.set_num();
      cerr << "[";
      for (ymuint i = 0; i < n; ++ i) {
	cerr << " " << vector_list.set_size(i);
      }
      cerr << " ]" << endl;
    }
  }

  ymuint n = vector_list.set_num();

  // 下界を計算して mBestSoFar を越えないことがわかったら枝刈りを行なう．
  {
    ymuint max_size = 0;
    for (ymuint i = 0; i < n; ++ i) {
      ymuint size = vector_list.set_size(i);
      if ( max_size < size ) {
	max_size = size;
      }
    }
    ymuint lb = blog(max_size) + mSelectedVariables.size();
    if ( lb >= mBestSoFar ) {
      return;
    }

    if ( max_size <= mMulti ) {
      // 解になっていた．
      ymuint ans = mSelectedVariables.size();
      if ( ans < mBestSoFar ) {
	mSolutionSoFar = mSelectedVariables;
	mBestSoFar = ans;
	cerr << "best_so_far => " << mBestSoFar << endl;
      }
      return;
    }
  }

  // ambiguity measure の昇順に並び替える．
  vector<pair<ymuint, Variable*> > tmp_list;
  for (vector<Variable*>::const_iterator p = var_begin;
       p != var_end; ++ p) {
    Variable* var = *p;
    ymuint max_size = 0;
    ymuint am = 0;
    bool split = false;
    for (ymuint j = 0; j < n; ++ j) {
      ymuint m = vector_list.set_size(j);
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (ymuint k = 0; k < m; ++ k) {
	RegVect* vect = vector_list.set_elem(j, k);
	if ( var->classify(vect) == 0 ) {
	  ++ n0;
	}
	else {
	  ++ n1;
	}
      }
      if ( n0 > 0 && n1 > 0 ) {
	split = true;
      }
      if ( max_size < n0 ) {
	max_size = n0;
      }
      if ( max_size < n1 ) {
	max_size = n1;
      }
      if ( n0 > mMulti ) {
	am += n0 * n0;
      }
      if ( n1 > mMulti ) {
	am += n1 * n1;
      }
    }

    if ( !split ) {
      // 一つも区別できない変数はいらない．
      continue;
    }

    ymuint lb = blog(max_size) + mSelectedVariables.size();
    if ( lb >= mBestSoFar ) {
      // 下界が現時点の最適解を下回らないので枝刈り
      continue;
    }

    tmp_list.push_back(make_pair(am, var));
  }
  sort(tmp_list.begin(), tmp_list.end(), Lt());
  ymuint nv2 = tmp_list.size();

  vector<Variable*> new_variables(nv2);
  for (ymuint i = 0; i < nv2; ++ i) {
    new_variables[i] = tmp_list[i].second;
  }

  ymuint bid = 0;
  for (vector<Variable*>::const_iterator p = new_variables.begin();
       p != new_variables.end(); ++ p, ++ bid) {
    Variable* var = *p;
    if ( mBranchLimit > 0 &&
	 bid >= mBranchLimit &&
	 tmp_list[mBranchLimit - 1].first < tmp_list[bid].first ) {
      break;
    }

    VectSetList new_vector_list(vector_list.all_elem_num(), n * 2);
    for (ymuint j = 0; j < n; ++ j) {
      ymuint m = vector_list.set_size(j);
      if ( m <= mMulti ) {
	continue;
      }
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (ymuint k = 0; k < m; ++ k) {
	RegVect* vect = vector_list.set_elem(j, k);
	if ( var->classify(vect) == 0 ) {
	  ++ n0;
	}
	else {
	  ++ n1;
	}
      }
      if ( n0 > mMulti ) {
	bool first = true;
	for (ymuint k = 0; k < m; ++ k) {
	  RegVect* vect = vector_list.set_elem(j, k);
	  if ( var->classify(vect) == 0 ) {
	    if ( first ) {
	      first = false;
	      new_vector_list.add_set();
	    }
	    new_vector_list.add_vect(vect);
	  }
	}
      }
      if ( n1 > mMulti ) {
	bool first = true;
	for (ymuint k = 0; k < m; ++ k) {
	  RegVect* vect = vector_list.set_elem(j, k);
	  if ( var->classify(vect) == 1 ) {
	    if ( first ) {
	      first = false;
	      new_vector_list.add_set();
	    }
	    new_vector_list.add_vect(vect);
	  }
	}
      }
    }

    mSelectedVariables.push_back(var);
    solve_recur(new_vector_list, p + 1, new_variables.end());
    mSelectedVariables.pop_back();

    if ( timeout ) {
      break;
    }
  }
}

END_NAMESPACE_YM_IGF
