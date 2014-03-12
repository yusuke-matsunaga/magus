
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
#include "MaxClique.h"
#include "Selector.h"
#include "utils/MFSet.h"


BEGIN_NAMESPACE_YM_MINCOV

bool mincov_debug = false;

void
verify_matrix(McMatrix& a,
	      McMatrix& b)
{
  assert_cond( a.remain_row_size() == b.remain_row_size(), __FILE__, __LINE__);
  assert_cond( a.remain_col_size() == b.remain_col_size(), __FILE__, __LINE__);
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
McSolverImpl::McSolverImpl()
{
  mMatrix = NULL;
}

// @brief デストラクタ
McSolverImpl::~McSolverImpl()
{
  delete mMatrix;
}

// @brief 問題のサイズを設定する．
// @param[in] row_size 行数
// @param[in] col_size 列数
void
McSolverImpl::set_size(ymuint32 row_size,
		       ymuint32 col_size)
{
  delete mMatrix;
  mMatrix = new McMatrix(row_size, col_size);
}

// @brief 列のコストを設定する
// @param[in] col_pos 追加する要素の列番号
// @param[in] cost コスト
void
McSolverImpl::set_col_cost(ymuint32 col_pos,
			   double cost)
{
  mMatrix->set_col_cost(col_pos, cost);
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
void
McSolverImpl::insert_elem(ymuint32 row_pos,
			  ymuint32 col_pos)
{
  mMatrix->insert_elem(row_pos, col_pos);
}

// @brief 最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @return 解のコスト
double
McSolverImpl::exact(vector<ymuint32>& solution)
{
  McMatrix orig_matrix(*mMatrix);
  mMatrix->save();

  mBest = DBL_MAX;
  mCurSolution.clear();
  solve();
  solution = mBestSolution;

  mMatrix->restore();
  verify_matrix(*mMatrix, orig_matrix);

  assert_cond( verify(solution), __FILE__, __LINE__);
  return mBest;
}

// @brief ヒューリスティックで最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @return 解のコスト
double
McSolverImpl::heuristic(vector<ymuint32>& solution)
{
  Selector sel;

  McMatrix orig_matrix(*mMatrix);
  mMatrix->save();

  solution.clear();
  for ( ; ; ) {
    mMatrix->reduce(solution);

    if ( mMatrix->remain_row_size() == 0 ) {
      break;
    }

    // 次の分岐のための列をとってくる．
    ymuint col = sel(*mMatrix);

    // その列を選択する．
    mMatrix->select_col(col);
    solution.push_back(col);

    if ( mincov_debug ) {
      cout << "Col#" << col << " is selected heuristically" << endl;
    }
  }

  mMatrix->restore();
  verify_matrix(*mMatrix, orig_matrix);

  assert_cond( verify(solution), __FILE__, __LINE__);
  double cost = mMatrix->cost(solution);

  return cost;
}

// @brief 解を求める再帰関数
void
McSolverImpl::solve()
{
  if ( mincov_debug ) {
    ymuint nr = mMatrix->remain_row_size();
    ymuint nc = mMatrix->remain_col_size();
    double cur_cost = mMatrix->cost(mCurSolution);
    cout << "solve(" << nr << " x " << nc << "): "
	 << mBest << " : " << cur_cost << endl;
  }

  mMatrix->reduce(mCurSolution);

  if ( mincov_debug ) {
    ymuint nr = mMatrix->remain_row_size();
    ymuint nc = mMatrix->remain_col_size();
    cout << "  after reduce(" << nr << " x " << nc << ")" << endl;
  }

  double tmp_cost = mMatrix->cost(mCurSolution);

  if ( mMatrix->remain_row_size() == 0 ) {
    if ( mBest > tmp_cost ) {
      mBest = tmp_cost;
      mBestSolution = mCurSolution;
    }
    return;
  }

  double lb = lower_bound(*mMatrix) + tmp_cost;
  if ( lb >= mBest ) {
    return;
  }

  Selector sel;

  // 次の分岐のための列をとってくる．
  ymuint col = sel(*mMatrix);

  // その列を選択したときの最良解を求める．
  //McMatrix orig_matrix(matrix);

  vector<ymuint32> orig_solution(mCurSolution);
  ymuint cur_n = mCurSolution.size();
  mMatrix->save();
  mMatrix->select_col(col);
  mCurSolution.push_back(col);

  if ( mincov_debug ) {
    cout << "select column#" << col << endl;
  }

  solve();
  mMatrix->restore();
  ymuint c = mCurSolution.size() - cur_n;
  for (ymuint i = 0; i < c; ++ i) {
    mCurSolution.pop_back();
  }

  if ( mincov_debug ) {
    //verify_matrix(orig_matrix, *mMatrix);
  }

  // その列を選択しなかったときの最良解を求める．
  mMatrix->delete_col(col);

  if ( mincov_debug ) {
    cout << "delete column#" << col << endl;
  }

  solve();
}

// @brief 下限を求める．
// @param[in] matrix 対象の行列
// @return 下限値
double
McSolverImpl::lower_bound(McMatrix& matrix)
{
  // MIS を用いた下限

  // まず，列を共有する行のグループを求める．
  ymuint32 rs = matrix.row_size();
  MFSet rset(rs);
  for (const McColHead* col = matrix.col_front();
       !matrix.is_col_end(col); col = col->next()) {
    const McCell* cell = col->front();
    ymuint32 rpos0 = cell->row_pos();
    for (cell = cell->col_next();
	 !col->is_end(cell); cell = cell->col_next()) {
      ymuint32 rpos = cell->row_pos();
      rset.merge(rpos0, rpos);
    }
  }

  vector<ymuint32> row_list;
  row_list.reserve(rs);
  for (const McRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    row_list.push_back(row->pos());
  }
  ymuint32 nr = row_list.size();
  MaxClique mc(nr);
  for (ymuint i = 0; i < nr - 1; ++ i) {
    ymuint32 rpos1 = row_list[i];
    for (ymuint j = i + 1; j < nr; ++ j) {
      ymuint32 rpos2 = row_list[j];
      if ( rset.find(rpos1) != rset.find(rpos2) ) {
	mc.connect(i, j);
      }
    }
  }
  for (ymuint i = 0; i < nr; ++ i) {
    ymuint32 rpos1 = row_list[i];
    const McRowHead* row = matrix.row(rpos1);
    double min_cost = DBL_MAX;
    for (const McCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      ymuint32 cpos = cell->col_pos();
      if ( min_cost > matrix.col_cost(cpos) ) {
	min_cost = matrix.col_cost(cpos);
      }
    }
    mc.set_cost(i, min_cost);
  }
  vector<ymuint32> mis;
  double cost1 = mc.solve(mis);
  assert_cond( cost1 > 0.0, __FILE__, __LINE__);
  return cost1;
}

// @brief 検証する．
bool
McSolverImpl::verify(const vector<ymuint32>& solution) const
{
  vector<bool> row_mark(mMatrix->row_size(), false);
  for (ymuint i = 0; i < solution.size(); ++ i) {
    ymuint32 col_pos = solution[i];
    const McColHead* col = mMatrix->col(col_pos);
    for (const McCell* cell = col->front();
	 !col->is_end(cell); cell = cell->col_next()) {
      ymuint32 row_pos = cell->row_pos();
      row_mark[row_pos] = true;
    }
  }
  bool status = true;
  for (const McRowHead* row = mMatrix->row_front();
       !mMatrix->is_row_end(row); row = row->next()) {
    ymuint32 row_pos = row->pos();
    if ( !row_mark[row_pos] ) {
      cerr << "Row#" << row_pos << " is not covered" << endl;
      status = false;
    }
  }
  if ( !status ) {
    for (ymuint i = 0; i < solution.size(); ++ i) {
      cerr << " " << solution[i];
    }
    cerr << endl;
  }
  return status;
}

END_NAMESPACE_YM_MINCOV
