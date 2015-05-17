
/// @file NlSolver3.cc
/// @brief NlSolver3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolver3.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "MazeRouter.h"
#include "YmLogic/SatSolver.h"
#include "YmUtils/HashMap.h"
#include "YmUtils/HashSet.h"

BEGIN_NAMESPACE_YM

template<>
struct
HashFunc<vector<ymuint> >
{
  ymuint
  operator()(const vector<ymuint>& key) const
  {
    ymuint ans = 0U;
    for (ymuint i = 0; i < key.size(); ++ i) {
      ans += ans * 117 + ans;
    }
    return ans;
  }
};

END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolver3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlSolver3::NlSolver3()
{
}

// @brief デストラクタ
NlSolver3::~NlSolver3()
{
}

BEGIN_NONAMESPACE

inline
void
one_hot2(SatSolver& solver,
	 Literal lit1,
	 Literal lit2)
{
  solver.add_clause(~lit1, ~lit2);
  solver.add_clause( lit1,  lit2);
}

inline
void
one_hot3(SatSolver& solver,
	 Literal lit1,
	 Literal lit2,
	 Literal lit3)
{
  solver.add_clause(~lit1, ~lit2);
  solver.add_clause(~lit1, ~lit3);
  solver.add_clause(~lit2, ~lit3);
  solver.add_clause( lit1,  lit2,  lit3);
}

inline
void
one_hot4(SatSolver& solver,
	 Literal lit1,
	 Literal lit2,
	 Literal lit3,
	 Literal lit4)
{
  solver.add_clause(~lit1, ~lit2);
  solver.add_clause(~lit1, ~lit3);
  solver.add_clause(~lit1, ~lit4);
  solver.add_clause(~lit2, ~lit3);
  solver.add_clause(~lit2, ~lit4);
  solver.add_clause(~lit3, ~lit4);
  solver.add_clause( lit1,  lit2,  lit3,  lit4);
}

inline
void
zero_two_hot2(SatSolver& solver,
	      Literal lit1,
	      Literal lit2)
{
  solver.add_clause(~lit1,  lit2);
  solver.add_clause( lit1, ~lit2);
}

inline
void
zero_two_hot3(SatSolver& solver,
	      Literal lit1,
	      Literal lit2,
	      Literal lit3)
{
  solver.add_clause(~lit1, ~lit2, ~lit3);
  solver.add_clause( lit1, ~lit2,  lit3);
  solver.add_clause( lit1,  lit2, ~lit3);
  solver.add_clause(~lit1,  lit2,  lit3);
}

inline
void
zero_two_hot4(SatSolver& solver,
	      Literal lit1,
	      Literal lit2,
	      Literal lit3,
	      Literal lit4)
{
  solver.add_clause( lit1,  lit2,  lit3, ~lit4);
  solver.add_clause( lit1,  lit2, ~lit3,  lit4);
  solver.add_clause( lit1, ~lit2,  lit3,  lit4);
  solver.add_clause( lit1, ~lit2, ~lit3, ~lit4);
  solver.add_clause(~lit1,  lit2,  lit3,  lit4);
  solver.add_clause(~lit1,  lit2, ~lit3, ~lit4);
  solver.add_clause(~lit1, ~lit2,  lit3, ~lit4);
  solver.add_clause(~lit1, ~lit2, ~lit3,  lit4);
  solver.add_clause(~lit1, ~lit2, ~lit3, ~lit4);
}

END_NONAMESPACE


// @brief 問題を解く
// @param[in] problem 問題
// @param[out] solution 解
void
NlSolver3::solve(const NlProblem& problem,
		 NlSolution& solution)
{
  SatSolver solver("minisat2", string(), NULL);
  //SatSolver solver("satrec", string(), &cout);

  ymuint w = problem.width();
  ymuint h = problem.height();
  ymuint n = problem.elem_num();

  ASSERT_COND( w > 0 );
  ASSERT_COND( h > 0 );
  ASSERT_COND( n > 0 );

  init(solver, w, h, n);

  // 始点と終点に印をつける．
  for (ymuint k = 0; k < n; ++ k) {
    NlConnection con = problem.connection(k);
    {
      NlPoint start_point = con.start_point();
      ymuint x1 = start_point.x();
      ymuint y1 = start_point.y();
      Node* node1 = _node(x1, y1);
      node1->mEndMark = k + 1;
      Literal lit1(node1->mVarArray[k]);
      solver.add_clause(lit1);
    }
    {
      NlPoint end_point = con.end_point();
      ymuint x2 = end_point.x();
      ymuint y2 = end_point.y();
      Node* node2 = _node(x2, y2);
      node2->mEndMark = k + 1;
      Literal lit2(node2->mVarArray[k]);
      solver.add_clause(lit2);
    }
  }

  // one-hot 制約を作る．
  // 一つのノードでは高々1つの変数しか1にならない．
  for (ymuint x = 0; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      Node* node = _node(x, y);
      for (ymuint k1 = 0; k1 < n; ++ k1) {
	Literal lit1(node->mVarArray[k1]);
	for (ymuint k2 = k1 + 1; k2 < n; ++ k2) {
	  Literal lit2(node->mVarArray[k2]);
	  solver.add_clause(~lit1, ~lit2);
	}
      }
    }
  }

  // 枝の条件を作る．
  for (ymuint x = 0; x < w; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      Node* node = _node(x, y);

      const vector<Edge*>& edge_list = node->mEdgeList;
      ymuint ne = edge_list.size();
      vector<Literal> lit_list(ne);
      for (ymuint i = 0; i < ne; ++ i) {
	lit_list[i] = Literal(edge_list[i]->mVar);
      }

      if ( node->mEndMark > 0 ) {
	// 端点の場合
	// 必ずただ1つの枝が選ばれる．
	switch ( ne ) {
	case 2:
	  one_hot2(solver, lit_list[0], lit_list[1]);
	  break;

	case 3:
	  one_hot3(solver, lit_list[0], lit_list[1], lit_list[2]);
	  break;

	case 4:
	  one_hot4(solver, lit_list[0], lit_list[1], lit_list[2], lit_list[3]);
	  break;

	default:
	  ASSERT_NOT_REACHED;
	}
      }
      else {
	// そうでない場合
	// 0個か2個の枝が選ばれる．
	switch ( ne ) {
	case 2:
	  zero_two_hot2(solver, lit_list[0], lit_list[1]);
	  break;

	case 3:
	  zero_two_hot3(solver, lit_list[0], lit_list[1], lit_list[2]);
	  break;

	case 4:
	  zero_two_hot4(solver, lit_list[0], lit_list[1], lit_list[2], lit_list[3]);
	  break;

	default:
	  ASSERT_NOT_REACHED;
	}
      }
    }
  }

  // 横方向の隣接関係の条件を作る．
  for (ymuint x = 0; x < w - 1; ++ x) {
    for (ymuint y = 0; y < h; ++ y) {
      Edge* edge = right_edge(x, y);
      Literal e_lit(edge->mVar);
      Node* node1 = _node(x, y);
      Node* node2 = _node(x + 1, y);
      for (ymuint k = 0; k < n; ++ k) {
	Literal v_lit1(node1->mVarArray[k]);
	Literal v_lit2(node2->mVarArray[k]);
	solver.add_clause(~e_lit,  v_lit1, ~v_lit2);
	solver.add_clause(~e_lit, ~v_lit1,  v_lit2);
      }
    }
  }

  // 縦方向の隣接関係の条件を作る．
  for (ymuint y = 0; y < h - 1; ++ y) {
    for (ymuint x = 0; x < w; ++ x) {
      Edge* edge = lower_edge(x, y);
      Literal e_lit(edge->mVar);
      Node* node1 = _node(x, y);
      Node* node2 = _node(x, y + 1);
      for (ymuint k = 0; k < n; ++ k) {
	Literal v_lit1(node1->mVarArray[k]);
	Literal v_lit2(node2->mVarArray[k]);
	solver.add_clause(~e_lit,  v_lit1, ~v_lit2);
	solver.add_clause(~e_lit, ~v_lit1,  v_lit2);
      }
    }
  }

  // (0, 0) のノードから (0, 1) ... (0, h - 1), (1, 0), (1, 1) , ...
  // の順にたどる．
  vector<vector<ymuint> > cur_sol_list;
  cur_sol_list.push_back(vector<ymuint>(mEdgeNum, 0));
  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    mCutMark[i] = false;
  }
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      cout << "Processing (" << x << ", " << y << ")" << endl;
      Node* node = _node(x, y);
      // (x, y) のノードを加える．
      // 上と左の枝が消えて
      // 下と右の枝が加わる．
      vector<Edge*> old_edge_list;
      vector<Edge*> new_edge_list;
      if ( x > 0 ) {
	Edge* edge = left_edge(x, y);
	old_edge_list.push_back(edge);
	mCutMark[edge->mId] = false;
      }
      if ( y > 0 ) {
	Edge* edge = upper_edge(x, y);
	old_edge_list.push_back(edge);
	mCutMark[edge->mId] = false;
      }
      if ( x < mWidth - 1 ) {
	Edge* edge = right_edge(x, y);
	new_edge_list.push_back(edge);
	mCutMark[edge->mId] = true;
      }
      if ( y < mHeight - 1 ) {
	Edge* edge = lower_edge(x, y);
	new_edge_list.push_back(edge);
	mCutMark[edge->mId] = true;
      }

      if ( false ) {
	cout << "cut = ";
	for (ymuint i = 0; i < mEdgeNum; ++ i) {
	  if ( mCutMark[i] ) {
	    cout << " " << mEdgeArray[i]->str();
	  }
	}
	cout << endl;
      }

      vector<vector<ymuint> > new_sol_list;
      HashSet<vector<ymuint> > sig_set;
      for (ymuint i = 0; i < cur_sol_list.size(); ++ i) {
	const vector<ymuint>& cur_sol = cur_sol_list[i];

	// 今，加えた節点に接続するすでにこちら側に含まれている
	// 枝のうち，選ばれているものの数を数える．
	ymuint old_count = 0;
	ymuint old_val = 0;
	for (ymuint j = 0; j < old_edge_list.size(); ++ j) {
	  Edge* edge = old_edge_list[j];
	  if ( cur_sol[edge->mId] > 0 ) {
	    ++ old_count;
	    old_val = cur_sol[edge->mId];
	  }
	}

	if ( node->mEndMark > 0 ) {
	  switch ( old_count ) {
	  case 0:
	    // new_edge_list のなかから1つとりだしてその枝を選択する．
	    for (ymuint j = 0;j < new_edge_list.size(); ++ j) {
	      Edge* edge = new_edge_list[j];
	      vector<ymuint> new_sol = cur_sol;
	      // 番号は端子番号となる．
	      new_sol[edge->mId] = node->mEndMark;
	      add_sol_list(new_sol, new_sol_list, sig_set);
	    }
	    break;

	  case 1:
	    // 新しい枝を選択しない．
	    // ただし，old_val の番号を持つ線分は端子番号で置き換える．
	    // ただし，old_val が別の端子番号の場合は追加しない．
	    if ( old_val > mNum || old_val == node->mEndMark ) {
	      vector<ymuint> new_sol = cur_sol;
	      for (ymuint i = 0; i < cur_sol.size(); ++ i) {
		if ( new_sol[i] == old_val ) {
		  new_sol[i] = node->mEndMark;
		}
	      }
	      add_sol_list(new_sol, new_sol_list, sig_set);
	    }
	    break;

	  default:
	    ASSERT_NOT_REACHED;
	    break;
	  }
	}
	else {
	  switch ( old_count ) {
	  case 0:
	    // new_edge_list のなかから2つ取り出して接続する．
	    if ( new_edge_list.size() < 2 ) {
	      // 解はない．
	    }
	    else {
	      Edge* edge1 = new_edge_list[0];
	      Edge* edge2 = new_edge_list[1];
	      ymuint val = edge1->mId;
	      if ( val > edge2->mId ) {
		val = edge2->mId;
	      }
	      vector<ymuint> new_sol = cur_sol;
	      // 番号は2つの枝の番号のうち小さい番号となる．
	      new_sol[edge1->mId] = val + mNum + 1;
	      new_sol[edge2->mId] = val + mNum + 1;
	      add_sol_list(new_sol, new_sol_list, sig_set);
	    }
	    break;

	  case 1:
	    // new_edge_list の中から1つ取り出して接続する．
	    for (ymuint j = 0; j < new_edge_list.size(); ++ j) {
	      Edge* edge = new_edge_list[j];
	      vector<ymuint> new_sol = cur_sol;
	      // 番号はもう一方と同じ番号となる．
	      new_sol[edge->mId] = old_val;
	      add_sol_list(new_sol, new_sol_list, sig_set);
	    }
	    break;

	  case 2:
	    {
	      Edge* edge1 = old_edge_list[0];
	      Edge* edge2 = old_edge_list[1];
	      ymuint old_val1 = cur_sol[edge1->mId];
	      ymuint old_val2 = cur_sol[edge2->mId];
	      if ( old_val1 != old_val2 ) {
		vector<ymuint> new_sol = cur_sol;
		if ( old_val1 <= mNum ) {
		  if ( old_val2 <= mNum ) {
		    // 異なる端子は繋げない．
		  }
		  else {
		    // old_val2 を old_val1 に置き換える．
		    for (ymuint i = 0; i < cur_sol.size(); ++ i) {
		      if ( new_sol[i] == old_val2 ) {
			new_sol[i] = old_val1;
		      }
		    }
		    add_sol_list(new_sol, new_sol_list, sig_set);
		  }
		}
		else if ( old_val2 <= mNum ) {
		  // old_val1 を old_val2 に置き換える．
		  for (ymuint i = 0; i < cur_sol.size(); ++ i) {
		    if ( new_sol[i] == old_val1 ) {
		      new_sol[i] = old_val2;
		    }
		  }
		  add_sol_list(new_sol, new_sol_list, sig_set);
		}
		else if ( old_val1 < old_val2 ) {
		  // old_val2 を old_val1 に置き換える．
		  for (ymuint i = 0; i < cur_sol.size(); ++ i) {
		    if ( new_sol[i] == old_val2 ) {
		      new_sol[i] = old_val1;
		    }
		  }
		  add_sol_list(new_sol, new_sol_list, sig_set);
		}
		else {
		  // old_val1 を old_val2 に置き換える．
		  for (ymuint i = 0; i < cur_sol.size(); ++ i) {
		    if ( new_sol[i] == old_val1 ) {
		      new_sol[i] = old_val2;
		    }
		  }
		  add_sol_list(new_sol, new_sol_list, sig_set);
		}
	      }
	      else {
		add_sol_list(cur_sol, new_sol_list, sig_set);
	      }
	    }
	    break;

	  default:
	    ASSERT_NOT_REACHED;
	    break;
	  }
	}
      }

      cout << " --> " << new_sol_list.size() << endl;
      cur_sol_list = new_sol_list;
    }
  }

  solution.init(problem);

  vector<Bool3> model;
  Bool3 stat = solver.solve(model);
  switch ( stat ) {
  case kB3True:
    cout << "SAT" << endl;
    setup_solution(model, solution);
    break;

  case kB3False:
    cout << "UNSAT" << endl;
    break;

  case kB3X:
    cout << "ABORT" << endl;
    break;
  }
}

// @brief 内容をクリアする．
void
NlSolver3::clear()
{
  for (ymuint i = 0; i < mHarray.size(); ++ i) {
    delete mHarray[i];
  }
  mHarray.clear();

  for (ymuint i = 0; i < mVarray.size(); ++ i) {
    delete mVarray[i];
  }
  mVarray.clear();

  for (ymuint i = 0; i < mNodeArray.size(); ++ i) {
    delete mNodeArray[i];
  }
  mNodeArray.clear();
}

// @brief 内容を初期化する．
// @param[in] solver SAT ソルバ
// @param[in] width 幅
// @param[in] height 高さ
// @param[in] num 線分数
void
NlSolver3::init(SatSolver& solver,
		ymuint width,
		ymuint height,
		ymuint num)
{
  mWidth = width;
  mHeight = height;
  mNum = num;

  // 横の辺を作る．
  ymuint nh = (width - 1) * height;
  ymuint nv = (height - 1) * width;

  mEdgeNum = nh + nv;
  mEdgeArray.reserve(nh + nv);
  mHarray.resize(nh);
  for (ymuint i = 0; i < nh; ++ i) {
    Edge* edge = new Edge;
    mEdgeArray.push_back(edge);
    mHarray[i] = edge;
    VarId var = solver.new_var();
    edge->mVar = var;
    edge->mId = i;
    edge->mVertical = false;
    edge->mX0 = i / height;
    edge->mY0 = i % height;
  }

  // 縦の辺を作る．
  mVarray.resize(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    Edge* edge = new Edge;
    mEdgeArray.push_back(edge);
    mVarray[i] = edge;
    VarId var = solver.new_var();
    edge->mId = i + nh;
    edge->mVar = var;
    edge->mVertical = true;
    edge->mY0 = i / width;
    edge->mX0 = i % width;
  }

  mCutMark.clear();
  mCutMark.resize(mEdgeNum, false);

  // ノードを作る．
  ymuint nn = width * height;
  mNodeArray.resize(nn);
  for (ymuint i = 0; i < nn; ++ i) {
    Node* node = new Node;
    node->mEndMark = 0;
    mNodeArray[i] = node;
    node->mVarArray.resize(num);
    for (ymuint j = 0; j < num; ++ j) {
      VarId var = solver.new_var();
      node->mVarArray[j] = var;
    }
  }

  // 辺の隣接関係を作る．
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      Node* node = mNodeArray[x * mHeight + y];
      node->mEdgeList.reserve(4);
      if ( x > 0 ) {
	// 左に辺がある．
	Edge* edge = left_edge(x, y);
	node->mEdgeList.push_back(edge);
      }
      if ( x < mWidth - 1 ) {
	// 右に辺がある．
	Edge* edge = right_edge(x, y);
	node->mEdgeList.push_back(edge);
      }
      if ( y > 0 ) {
	// 上に辺がある．
	Edge* edge = upper_edge(x, y);
	node->mEdgeList.push_back(edge);
      }
      if ( y < mHeight - 1 ) {
	// 下に辺がある．
	Edge* edge = lower_edge(x, y);
	node->mEdgeList.push_back(edge);
      }
    }
  }
}

// @brief ノードを得る．
NlSolver3::Node*
NlSolver3::_node(ymuint x,
		 ymuint y)
{
  ASSERT_COND( x > 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y > 0 );
  ASSERT_COND( y < mHeight );

  return mNodeArray[x * mHeight + y];
}

// @brief 左の辺を得る．
// @param[in] x, y 辺の右端の座標
NlSolver3::Edge*
NlSolver3::left_edge(ymuint x,
		     ymuint y)
{
  ASSERT_COND( x > 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y >= 0 );
  ASSERT_COND( y < mHeight );

  return mHarray[(x - 1) * mHeight + y];
}

// @brief 右の辺を得る．
// @param[in] x, y 辺の左端の座標
NlSolver3::Edge*
NlSolver3::right_edge(ymuint x,
		      ymuint y)
{
  ASSERT_COND( x >= 0 );
  ASSERT_COND( x < mWidth - 1 );
  ASSERT_COND( y >= 0 );
  ASSERT_COND( y < mHeight );

  return mHarray[x * mHeight + y];
}

// @brief 上の辺を得る．
// @param[in] x, y 辺の下端の座標
NlSolver3::Edge*
NlSolver3::upper_edge(ymuint x,
		      ymuint y)
{
  ASSERT_COND( x >= 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y > 0 );
  ASSERT_COND( y < mHeight );

  return mVarray[(y - 1) * mWidth + x];
}

// @brief 下の辺を得る．
// @param[in] x, y 辺の上端の座標
NlSolver3::Edge*
NlSolver3::lower_edge(ymuint x,
		      ymuint y)
{
  ASSERT_COND( x >= 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y >= 0 );
  ASSERT_COND( y < mHeight - 1 );

  return mVarray[y * mWidth + x];
}

// @brief 解を出力する．
// @param[in] model SATの解
// @param[in] solution 解
void
NlSolver3::setup_solution(const vector<Bool3>& model,
			  NlSolution& solution)
{
  for (ymuint y = 0; y < mHeight; ++ y) {
    for (ymuint x = 0; x < mWidth; ++ x) {
      if ( solution.get(x, y) < 0 ) {
	continue;
      }
      Node* node = _node(x, y);
      for (ymuint k = 0; k < mNum; ++ k) {
	VarId var = node->mVarArray[k];
	if ( model[var.val()] == kB3True ) {
	  solution.set(x, y, k + 1);
	  break;
	}
      }
    }
  }
}

void
NlSolver3::add_sol_list(const vector<ymuint>& cur_sol,
			vector<vector<ymuint> >& sol_list,
			HashSet<vector<ymuint> >& sig_set)
{
  // シグネチャを計算する．
  vector<ymuint> cut_list;
  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    if ( mCutMark[i] ) {
      cut_list.push_back(i);
    }
  }
  ymuint n = cut_list.size();
  vector<ymuint> signature(n);
  HashMap<ymuint, ymuint> num_map;

  ymuint ecount = 0;
  for (ymuint i = 0; i < n; ++ i) {
    ymuint edge_id = cut_list[i];
    ymuint num = cur_sol[edge_id];
    if ( num <= mNum ) {
      // 端子番号ならそのまま
      signature[i] = num;
    }
    else {
      ymuint val;
      if ( !num_map.find(num, val) ) {
	val = ecount + mNum + 1;
	num_map.add(num, val);
	++ ecount;
      }
      signature[i] = val;
    }
  }

  if ( false ) {
    for (ymuint i = 0; i < mEdgeNum; ++ i) {
      ymuint num = cur_sol[i];
      if ( num == 0 && !mCutMark[i] ) {
	continue;
      }
      Edge* edge = mEdgeArray[i];
      cout << " " << edge->str();
      if ( mCutMark[i] ) {
	cout << "*";
      }
      cout << ": ";
      if ( num == 0 ) {
	cout << "__";
      }
      else if ( num <= mNum ) {
	cout << "#" << num;
      }
      else {
	cout << "@" << (num - mNum);
      }
    }
    cout << endl;
    cout << " signature = ";
    for (ymuint i = 0; i < n; ++ i) {
      ymuint val = signature[i];
      cout << " ";
      if ( val == 0 ) {
	cout << " __";
      }
      else if ( val <= mNum ) {
	cout << "#" << val;
      }
      else {
	cout << "@" << val;
      }
    }
    cout << endl;
  }

  // シグネチャのチェックを行う．
  if ( !sig_set.check(signature) ) {
    sol_list.push_back(cur_sol);
    sig_set.add(signature);
  }
}

// 枝を表す文字列を返す．
string
NlSolver3::Edge::str() const
{
  ostringstream buf;
  if ( mVertical ) {
    buf << "V";
  }
  else {
    buf << "H";
  }
  buf << "(" << mX0 << ", " << mY0 << ")";
  return buf.str();
}

END_NAMESPACE_YM_NLINK
