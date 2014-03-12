
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
  McMatrix work(*mMatrix);
  mNoBranch = false;
  mBest = DBL_MAX;
  solve(work, vector<ymuint32>());
  solution = mBestSolution;
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
  McMatrix work(*mMatrix);

  solution.clear();
  for ( ; ; ) {
    reduce(work, solution);

    if ( work.remain_row_size() == 0 ) {
      assert_cond( verify(solution), __FILE__, __LINE__);
      double cost = work.cost(solution);
      return cost;
    }

    // 次の分岐のための列をとってくる．
    ymuint col = sel(work);

    // その列を選択する．
    work.select_col(col);
    solution.push_back(col);

    if ( mincov_debug ) {
      cout << "Col#" << col << " is selected heuristically" << endl;
    }
  }
}

// @brief 解を求める再帰関数
// @param[in] matrix 対象の行列
// @param[in] best_sofar 現時点の最良解
void
McSolverImpl::solve(McMatrix& matrix,
		    const vector<ymuint32>& cur_solution)
{
  if ( mincov_debug ) {
    ymuint nr = matrix.remain_row_size();
    ymuint nc = matrix.remain_col_size();
    double cur_cost = matrix.cost(cur_solution);
    cout << "solve(" << nr << " x " << nc << "): " << mBest << " : " << cur_cost << endl;
  }

  vector<ymuint32> tmp_solution(cur_solution);
  reduce(matrix, tmp_solution);

  if ( mincov_debug ) {
    ymuint nr = matrix.remain_row_size();
    ymuint nc = matrix.remain_col_size();
    cout << "  after reduce(" << nr << " x " << nc << ")" << endl;
  }

  double tmp_cost = matrix.cost(tmp_solution);

  if ( matrix.remain_row_size() == 0 ) {
    if ( mBest > tmp_cost ) {
      mBest = tmp_cost;
      mBestSolution = tmp_solution;
    }
    return;
  }

  double lb = lower_bound(matrix) + tmp_cost;
  if ( lb >= mBest ) {
    return;
  }

  Selector sel;

  // 次の分岐のための列をとってくる．
  ymuint col = sel(matrix);

  // その列を選択したときの最良解を求める．
  McMatrix orig_matrix(matrix);
  matrix.backup();
  matrix.select_col(col);
  tmp_solution.push_back(col);

  if ( mincov_debug ) {
    cout << "select column#" << col << endl;
  }

  solve(matrix, tmp_solution);
  matrix.restore();

  if ( mincov_debug ) {
    verify_matrix(orig_matrix, matrix);
  }

  // その列を選択しなかったときの最良解を求める．
  matrix.delete_col(col);
  tmp_solution.pop_back();

  if ( mincov_debug ) {
    cout << "delete column#" << col << endl;
  }

  solve(matrix, tmp_solution);
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

// @brief 簡単化を行う．
// @param[in] matrix 対象の行列
// @param[out] selected_cols 簡単化中で選択された列の集合
void
McSolverImpl::reduce(McMatrix& matrix,
		     vector<ymuint32>& selected_cols)
{
  for ( ; ; ) {
    bool change = false;

    // 行支配を探し，行の削除を行う．
    if ( row_dominance(matrix) ) {
      change = true;
    }

    // 列支配を探し，列の削除を行う．
    if ( col_dominance(matrix) ) {
      change = true;
    }

    // 必須列を探し，列の選択を行う．
    if ( essential_col(matrix, selected_cols) ) {
      change = true;
    }

    if ( !change ) {
      break;
    }
  }
}


BEGIN_NONAMESPACE

struct RowLt
{
  bool
  operator()(const McRowHead* a,
	     const McRowHead* b)
  {
    return a->num() < b->num();
  }
};

END_NONAMESPACE


// @brief 行支配を探し，行を削除する．
// @param[in] matrix 対象の行列
// @return 削除された行があったら true を返す．
bool
McSolverImpl::row_dominance(McMatrix& matrix)
{
  bool change = false;

  // 削除された行番号に印をつけるための配列
  vector<bool> del_mark(matrix.row_size(), false);

  // 残っている行のリストを作る．
  vector<const McRowHead*> row_list;
  row_list.reserve(matrix.row_size());
  for (const McRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    row_list.push_back(row);
  }

  // 要素数の少ない順にソートする．
  sort(row_list.begin(), row_list.end(), RowLt());

  for (vector<const McRowHead*>::iterator p = row_list.begin();
       p != row_list.end(); ++ p) {
    const McRowHead* row = *p;
    if ( del_mark[row->pos()] ) continue;

    // row の行に要素を持つ列で要素数が最小のものを求める．
    ymuint32 min_num = matrix.row_size() + 1;
    const McColHead* min_col = NULL;
    for (const McCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      ymuint32 col_pos = cell->col_pos();
      const McColHead* col = matrix.col(col_pos);
      ymuint32 col_num = col->num();
      if ( min_num > col_num ) {
	min_num = col_num;
	min_col = col;
      }
    }

    // min_col の列に要素を持つ行を tmp_rows に入れる．
    vector<ymuint32> tmp_rows;
    tmp_rows.reserve(min_num);
    for (const McCell* cell = min_col->front();
	 !min_col->is_end(cell); cell = cell->col_next()) {
      ymuint32 row_pos = cell->row_pos();
      if ( matrix.row(row_pos)->num() > row->num() ) {
	// ただし row よりも要素数の多いもののみを対象にする．
	tmp_rows.push_back(row_pos);
      }
    }

    // min_col 以外の列に含まれない行を tmp_rows から落とす．
    for (const McCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      ymuint32 col_pos = cell->col_pos();
      const McColHead* col = matrix.col(col_pos);
      if ( col == min_col ) continue;
      ymuint rpos = 0;
      ymuint wpos = 0;
      ymuint32 row1 = tmp_rows[rpos];
      const McCell* cell = col->front();
      ymuint32 row2 = cell->row_pos();
      while ( rpos < tmp_rows.size() && !col->is_end(cell) ) {
	if ( row1 == row2 ) {
	  if ( wpos != rpos ) {
	    tmp_rows[wpos] = row1;
	  }
	  ++ wpos;
	  ++ rpos;
	  row1 = tmp_rows[rpos];
	}
	else if ( row1 < row2 ) {
	  // row1 を削除
	  ++ rpos;
	}
	else {
	  cell = cell->col_next();
	}
      }
      if ( wpos < tmp_rows.size() ) {
	tmp_rows.erase(tmp_rows.begin() + wpos, tmp_rows.end());
      }
    }

    // tmp_rows に残った行は row に支配されている．
    for (vector<ymuint32>::iterator p = tmp_rows.begin();
	 p != tmp_rows.end(); ++ p) {
      ymuint row_pos = *p;
      matrix.delete_row(row_pos);
      del_mark[row_pos] = true;
      change = true;
      if ( mincov_debug ) {
	cout << "Row#" << row_pos << " is dominated by Row#" << row->pos() << endl;
      }
    }
  }

  return change;
}


BEGIN_NONAMESPACE

struct ColLt
{
  bool
  operator()(const McColHead* a,
	     const McColHead* b)
  {
    return a->num() < b->num();
  }
};

END_NONAMESPACE


// @brief 列支配を探し，列を削除する．
// @param[in] matrix 対象の行列
// @return 削除された列があったら true を返す．
bool
McSolverImpl::col_dominance(McMatrix& matrix)
{
  bool change = false;

  // 残っている列のリストを作る．
  vector<const McColHead*> col_list;
  col_list.reserve(matrix.col_size());
  for (const McColHead* col = matrix.col_front();
       !matrix.is_col_end(col); col = col->next()) {
    col_list.push_back(col);
  }
  // 要素数の少ない順にソートする．
  sort(col_list.begin(), col_list.end(), ColLt());

  for (vector<const McColHead*>::iterator p = col_list.begin();
       p != col_list.end(); ++ p) {
    const McColHead* col = *p;

    if ( col->num() == 0 ) continue;

    // col の列に要素を持つ行で要素数が最小のものを求める．
    ymuint32 min_num = matrix.col_size() + 1;
    const McRowHead* min_row = NULL;
    for (const McCell* cell = col->front();
	 !col->is_end(cell); cell = cell->col_next()) {
      ymuint32 row_pos = cell->row_pos();
      const McRowHead* row = matrix.row(row_pos);
      ymuint32 row_num = row->num();
      if ( min_num > row_num ) {
	min_num = row_num;
	min_row = row;
      }
    }

    // min_row の行に要素を持つ列を対象にして支配関係のチェックを行う．
    for (const McCell* cell = min_row->front();
	 !min_row->is_end(cell); cell = cell->row_next()) {
      const McColHead* col2 = matrix.col(cell->col_pos());
      if ( col2->num() <= col->num() ) {
	// ただし col よりも要素数の多くない列は調べる必要はない．
	continue;
      }
      if ( matrix.col_cost(col2->pos()) > matrix.col_cost(col->pos()) ) {
	// col2 のコストが col のコストより高ければ調べる必要はない．
	continue;
      }

      const McCell* cell1 = col->front();
      ymuint32 pos1 = cell1->row_pos();
      const McCell* cell2 = col2->front();
      ymuint32 pos2 = cell2->row_pos();
      bool found = false;
      for ( ; ; ) {
	if ( pos1 < pos2 ) {
	  // col に含まれていて col2 に含まれない行があるので
	  // col2 は col を支配しない．
	  break;
	}
	if ( pos1 == pos2 ) {
	  cell1 = cell1->col_next();
	  if ( col->is_end(cell1) ) {
	    found = true;
	    break;
	  }
	}
	cell2 = cell2->col_next();
	if ( col2->is_end(cell2) ) {
	  break;
	}
      }
      if ( found ) {
	// col2 は col を支配している．
	matrix.delete_col(col->pos());
	if ( mincov_debug ) {
	  cout << "Col#" << col->pos() << " is dominated by Col#" << col2->pos() << endl;
	}
	change = true;
	break;
      }
    }
  }

  return change;
}

// @brief 必須列を探し，列を選択する．
// @param[in] matrix 対象の行列
// @param[out] selected_cols 選択された列を追加する列集合
// @return 選択された列があったら true を返す．
bool
McSolverImpl::essential_col(McMatrix& matrix,
			    vector<ymuint32>& selected_cols)
{
  vector<ymuint32> tmp_list;
  vector<bool> col_mark(matrix.col_size(), false);
  tmp_list.reserve(matrix.col_size());
  for (const McRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    if ( row->num() == 1 ) {
      const McCell* cell = row->front();
      ymuint32 col_pos = cell->col_pos();
      if ( !col_mark[col_pos] ) {
	col_mark[col_pos] = true;
	tmp_list.push_back(col_pos);
      }
    }
  }
  for (vector<ymuint32>::iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    ymuint32 col_pos = *p;
    selected_cols.push_back(col_pos);
    matrix.select_col(col_pos);
    if ( mincov_debug ) {
      cout << "Col#" << col_pos << " is essential" << endl;
    }
  }
  return !tmp_list.empty();
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
