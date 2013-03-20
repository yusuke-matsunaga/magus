
/// @file IguGen.cc
/// @brief IguGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "IguGen.h"
#include "RegVect.h"
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
  mLimit = 0;
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
  mBestSoFar = nv;
  mSelectedVariables.clear();
  timeout = false;

  // タイマーの設定
  struct sigaction old_act;
  timer_t tid;
  if ( mLimit > 0 ) {
    struct sigaction act;
    act.sa_handler = timer_handler;
    act.sa_flags = 0;
    if ( sigaction(SIGALRM, &act, &old_act) < 0 ) {
      perror("sigaction()");
      exit(-1);
    }

    struct itimerspec itval;
    itval.it_value.tv_sec = mLimit;
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

  vector<vector<RegVect*> > initial_list(1, vector_list);
  solve_recur(initial_list,
	      new_variable_list.begin(), new_variable_list.end());
  solution = mSolutionSoFar;

  if ( mLimit > 0 ) {
    timer_delete(tid);
    sigaction(SIGALRM, &old_act, NULL);
  }
}

// @brief 時間制限を設定する．
// @param[in] limit_min 制限の分の部分
// @param[in] limit_sec 制限の秒の部分
void
IguGen::set_time_limit(ymuint32 limit_min,
		       ymuint32 limit_sec)
{
  mLimit = limit_min * 60 + limit_sec;
}

// @brief solve() の下請け関数
// @param[in] vector_list 分割されたベクタのリスト
// @param[in] var_begin 使用可能な変数のリストの先頭の反復子
// @param[in] var_end 使用可能な変数のリストの末尾の反復子
void
IguGen::solve_recur(const vector<vector<RegVect*> >& vector_list,
		    vector<Variable*>::const_iterator var_begin,
		    vector<Variable*>::const_iterator var_end)
{
  if ( 0 ) {
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
    ymuint n = vector_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      const vector<RegVect*>& v_list = vector_list[i];
      for (vector<RegVect*>::const_iterator p = v_list.begin();
	   p != v_list.end(); ++ p) {
	RegVect* vect = *p;
	vect->dump(cerr);
      }
      cerr << "-----------------" << endl;
    }
  }

  ymuint n = vector_list.size();

  // 下界を計算して mBestSoFar を越えないことがわかったら枝刈りを行なう．
  {
    ymuint max_size = 0;
    for (ymuint i = 0; i < n; ++ i) {
      const vector<RegVect*>& v_list = vector_list[i];
      ymuint size = v_list.size();
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
    for (ymuint j = 0; j < n; ++ j) {
      const vector<RegVect*>& v_list = vector_list[j];
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (vector<RegVect*>::const_iterator p = v_list.begin();
	   p != v_list.end(); ++ p) {
	RegVect* vect = *p;
	if ( var->classify(vect) == 0 ) {
	  ++ n0;
	}
	else {
	  ++ n1;
	}
      }
      if ( max_size < n0 ) {
	max_size = n0;
      }
      if ( max_size < n1 ) {
	max_size = n1;
      }
      am += n0 * n0 + n1 * n1;
    }
    ymuint lb = blog(max_size) + mSelectedVariables.size();
    if ( lb >= mBestSoFar ) {
      // 枝刈り
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

  for (vector<Variable*>::const_iterator p = new_variables.begin();
       p != new_variables.end(); ++ p) {
    Variable* var = *p;

    vector<vector<RegVect*> > new_vector_list;
    new_vector_list.reserve(n * 2);
    for (ymuint j = 0; j < n; ++ j) {
      const vector<RegVect*>& v_list = vector_list[j];
      if ( v_list.size() <= mMulti ) {
	continue;
      }
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (vector<RegVect*>::const_iterator p = v_list.begin();
	   p != v_list.end(); ++ p) {
	RegVect* vect = *p;
	if ( var->classify(vect) == 0 ) {
	  ++ n0;
	}
	else {
	  ++ n1;
	}
      }
      if ( n0 > mMulti ) {
	vector<RegVect*> tmp_list;
	for (vector<RegVect*>::const_iterator p = v_list.begin();
	     p != v_list.end(); ++ p) {
	  RegVect* vect = *p;
	  if ( var->classify(vect) == 0 ) {
	    tmp_list.push_back(vect);
	  }
	}
	new_vector_list.push_back(tmp_list);
      }
      if ( n1 > mMulti ) {
	vector<RegVect*> tmp_list;
	for (vector<RegVect*>::const_iterator p = v_list.begin();
	     p != v_list.end(); ++ p) {
	  RegVect* vect = *p;
	  if ( var->classify(vect) == 1 ) {
	    tmp_list.push_back(vect);
	  }
	}
	new_vector_list.push_back(tmp_list);
      }
    }

    mSelectedVariables.push_back(var);
    solve_recur(new_vector_list, p, new_variables.end());
    mSelectedVariables.pop_back();

    if ( timeout ) {
      break;
    }
  }
}

END_NAMESPACE_YM_IGF
