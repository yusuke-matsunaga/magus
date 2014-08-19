
/// @file McSolverImpl.cc
/// @brief McSolverImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "McSolverImpl.h"
#include "McMatrix.h"
#include "McRowHead.h"
#include "McColHead.h"
#include "McCell.h"
#include "LbCalc.h"
#include "Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

static
ymuint solve_id = 0;

// 2つの行列が等しいかをチェックする関数
// 等しくなければ例外を送出する．
void
verify_matrix(McMatrix& a,
	      McMatrix& b)
{
  assert_cond( a.row_num() == b.row_num(), __FILE__, __LINE__);
  assert_cond( a.col_num() == b.col_num(), __FILE__, __LINE__);
  const McRowHead* row_a = a.row_front();
  const McRowHead* row_b = b.row_front();
  for ( ; ; ) {
    assert_cond( row_a->pos() == row_b->pos(), __FILE__, __LINE__);
    assert_cond( row_a->num() == row_b->num(), __FILE__, __LINE__);
    const McCell* cell_a = row_a->front();
    const McCell* cell_b = row_b->front();
    for ( ; ; ) {
      assert_cond( cell_a->row_pos() == row_a->pos(), __FILE__, __LINE__);
      assert_cond( cell_b->row_pos() == row_b->pos(), __FILE__, __LINE__);
      assert_cond( cell_a->col_pos() == cell_b->col_pos(), __FILE__, __LINE__);
      cell_a = cell_a->row_next();
      cell_b = cell_b->row_next();
      if ( row_a->is_end(cell_a) ) {
	assert_cond( row_b->is_end(cell_b), __FILE__, __LINE__);
	break;
      }
    }
    row_a = row_a->next();
    row_b = row_b->next();
    if ( a.is_row_end(row_a) ) {
      assert_cond( b.is_row_end(row_b), __FILE__, __LINE__);
      break;
    }
  }
  const McColHead* col_a = a.col_front();
  const McColHead* col_b = b.col_front();
  for ( ; ; ) {
    assert_cond( col_a->pos() == col_b->pos(), __FILE__, __LINE__);
    assert_cond( col_a->num() == col_b->num(), __FILE__, __LINE__);
    const McCell* cell_a = col_a->front();
    const McCell* cell_b = col_b->front();
    for ( ; ; ) {
      assert_cond( cell_a->col_pos() == col_a->pos(), __FILE__, __LINE__);
      assert_cond( cell_b->col_pos() == col_b->pos(), __FILE__, __LINE__);
      assert_cond( cell_a->row_pos() == cell_b->row_pos(), __FILE__, __LINE__);
      cell_a = cell_a->col_next();
      cell_b = cell_b->col_next();
      if ( col_a->is_end(cell_a) ) {
	assert_cond( col_b->is_end(cell_b), __FILE__, __LINE__);
	break;
      }
    }
    col_a = col_a->next();
    col_b = col_b->next();
    if ( a.is_col_end(col_a) ) {
      assert_cond( b.is_col_end(col_b), __FILE__, __LINE__);
      break;
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス McSolverImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] matrix 問題の行列
// @param[in] lb_calc 下界の計算クラス
// @param[in] selector 列を選択するクラス
McSolverImpl::McSolverImpl(const McMatrix& matrix,
			   LbCalc& lb_calc,
			   Selector& selector) :
  mMatrix(matrix),
  mLbCalc(lb_calc),
  mSelector(selector)
{
}

// @brief コンストラクタ
// @param[in] matrix 問題の行列
// @param[in] row_list 注目する行番号のリスト
// @param[in] col_list 注目する列番号のリスト
// @param[in] lb_calc 下界の計算クラス
// @param[in] selector 列を選択するクラス
McSolverImpl::McSolverImpl(McMatrix& matrix,
			   const vector<ymuint32>& row_list,
			   const vector<ymuint32>& col_list,
			   LbCalc& lb_calc,
			   Selector& selector) :
  mMatrix(matrix, row_list, col_list),
  mLbCalc(lb_calc),
  mSelector(selector)
{
}

// @brief デストラクタ
McSolverImpl::~McSolverImpl()
{
}

// @brief 最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @return 解のコスト
ymuint32
McSolverImpl::exact(vector<ymuint32>& solution)
{
  // 検証用にもとの行列をコピーしておく．
  McMatrix orig_matrix(mMatrix);
  // こちらは McMatrix 自身が持つ復元機能
  mMatrix.save();

  solve_id = 0;

  mBest = UINT_MAX;
  mCurSolution.clear();
  bool stat = solve(0, 0);
  assert_cond( stat, __FILE__, __LINE__);
  solution = mBestSolution;

  // 復元が正しいかチェックする．
  mMatrix.restore();
  verify_matrix(mMatrix, orig_matrix);

  // solution がカバーになっているかチェックする．
  assert_cond( mMatrix.verify(solution), __FILE__, __LINE__);

  cout << "Total branch: " << solve_id << endl;

  return mBest;
}

// @brief 解を求める再帰関数
bool
McSolverImpl::solve(ymuint lb,
		    ymuint depth)
{
  ymuint cur_id = solve_id;
  ++ solve_id;

  mMatrix.reduce(mCurSolution);

  ymuint32 tmp_cost = mMatrix.cost(mCurSolution);

  ymuint32 tmp_lb = mLbCalc(mMatrix) + tmp_cost;
  if ( lb < tmp_lb ) {
    lb = tmp_lb;
  }

  bool cur_debug = mDebug;
  if (depth > mMaxDepth) {
    cur_debug = false;
  }

  if ( cur_debug ) {
    ymuint nr = mMatrix.row_num();
    ymuint nc = mMatrix.col_num();
    cout << "[" << depth << "] " << nr << "x" << nc
	 << " sel=" << tmp_cost << " bnd=" << mBest
	 << " lb=" << lb;
  }

  if ( lb >= mBest ) {
    if ( cur_debug ) {
      cout << " bounded" << endl;
    }
    return false;
  }

  if ( mMatrix.row_num() == 0 ) {
    mBest = tmp_cost;
    mBestSolution = mCurSolution;
    if ( cur_debug ) {
      cout << " best" << endl;
    }
    return true;
  }

  vector<ymuint32> row_list1;
  vector<ymuint32> row_list2;
  vector<ymuint32> col_list1;
  vector<ymuint32> col_list2;
  if ( mDoPartition && mMatrix.block_partition(row_list1, row_list2, col_list1, col_list2) ) {
    // ブロック分割を行う．
    McSolverImpl solver1(mMatrix, row_list1, col_list1, mLbCalc, mSelector);
    McSolverImpl solver2(mMatrix, row_list2, col_list2, mLbCalc, mSelector);
    if ( cur_debug ) {
      cout << endl
	   << "BLOCK PARTITION" << endl;
      cout << "Matrix#1" << endl;
      solver1.matrix().print(cout);
      cout << "Matrix#2" << endl;
      solver2.matrix().print(cout);
    }
    solver1.mMatrix.save();
    solver2.mMatrix.save();
    ymuint32 cost_so_far = mMatrix.cost(mCurSolution);
    ymuint lb_rest = mLbCalc(solver2.matrix());
    solver1.mBest = mBest - cost_so_far - lb_rest;
    solver1.mCurSolution.clear();
    bool stat1 = solver1.solve(0, depth + 1);
    if ( stat1 ) {
      mCurSolution.insert(mCurSolution.end(), solver1.mBestSolution.begin(), solver1.mBestSolution.end());
      cost_so_far += solver1.mBest;
      solver2.mBest = mBest - cost_so_far;
      solver2.mCurSolution.clear();
      stat1 = solver2.solve(0, depth + 1);
      if ( stat1 ) {
	mCurSolution.insert(mCurSolution.end(), solver2.mBestSolution.begin(), solver2.mBestSolution.end());
	cost_so_far += solver2.mBest;
      }
    }
    solver1.mMatrix.restore();
    solver2.mMatrix.restore();
    mMatrix.merge(solver1.mMatrix, solver2.mMatrix);

    if ( stat1 ) {
      assert_cond( mMatrix.verify(mCurSolution), __FILE__, __LINE__);
      if ( mBest > cost_so_far ) {
	mBest = cost_so_far;
	mBestSolution = mCurSolution;
	return true;
      }
    }
    return false;
  }

  // 次の分岐のための列をとってくる．
  ymuint col = mSelector(mMatrix);

#if defined(VERIFY_MINCOV)
  McMatrix orig_matrix(mMatrix);
  vector<ymuint32> orig_solution(mCurSolution);
#endif

  ymuint cur_n = mCurSolution.size();
  mMatrix.save();

  // その列を選択したときの最良解を求める．
  mMatrix.select_col(col);
  mCurSolution.push_back(col);

  if ( cur_debug ) {
    cout << " select column#" << col << endl;
  }

  bool stat1 = solve(lb, depth + 1);

  mMatrix.restore();
  ymuint c = mCurSolution.size() - cur_n;
  for (ymuint i = 0; i < c; ++ i) {
    mCurSolution.pop_back();
  }

#if defined(VERIFYY_MINCOV)
  verify_matrix(orig_matrix, mMatrix);
  assert_cond( orig_solution == mCurSlution, __FILE__, __LINE__);
#endif

  // 今得た最良解が下界と等しかったら探索を続ける必要はない．
  if ( lb >= mBest ) {
    return true;
  }

  // その列を選択しなかったときの最良解を求める．
  mMatrix.delete_col(col);

  if ( cur_debug ) {
    cout << "delete column#" << col << endl;
  }

  bool stat2 = solve(lb, depth + 1);

  return stat1 || stat2;
}

// @brief 内部の行列を返す．
const McMatrix&
McSolverImpl::matrix() const
{
  return mMatrix;
}

// @brief partition フラグを設定する．
void
McSolverImpl::set_partition(bool flag)
{
  mDoPartition = flag;
}

// @brief デバッグフラグを設定する．
void
McSolverImpl::set_debug(bool flag)
{
  mDebug = flag;
}

// @brief mMaxDepth を設定する．
void
McSolverImpl::set_max_depth(ymuint depth)
{
  mMaxDepth = depth;
}

bool
McSolverImpl::mDoPartition = true;

bool
McSolverImpl::mDebug = false;

ymuint32
McSolverImpl::mMaxDepth = 0;

END_NAMESPACE_YM_MINCOV
