
/// @file LbMIS.cc
/// @brief LbMIS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbMIS.h"
#include "McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LBMIS
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] matrix 対象の行列
// @return 下限値
ymuint32
LbMIS::operator()(const McMatrix& matrix)
{
  if ( matrix.row_num() == 0 ) {
    return 0;
  }

  // MIS を用いた下限

  // まず，列を共有する行の関係を表す行列を作る．
  ymuint32 rs = matrix.row_size();
  McMatrix M(rs, rs);
  for (const McRowHead* row1 = matrix.row_front();
       !matrix.is_row_end(row1); row1 = row1->next()) {
    for (const McCell* cell1 = row1->front();
	 !row1->is_end(cell1); cell1 = cell1->row_next()) {
      const McColHead* col1 = matrix.col(cell1->col_pos());
      for (const McCell* cell2 = col1->front();
	   !col1->is_end(cell2); cell2 = cell2->col_next()) {
	ymuint row_pos = cell2->row_pos();
	matrix.row(row_pos)->mWork = 0;
      }
    }
    for (const McCell* cell1 = row1->front();
	 !row1->is_end(cell1); cell1 = cell1->row_next()) {
      const McColHead* col1 = matrix.col(cell1->col_pos());
      for (const McCell* cell2 = col1->front();
	   !col1->is_end(cell2); cell2 = cell2->col_next()) {
	ymuint row_pos = cell2->row_pos();
	if ( matrix.row(row_pos)->mWork == 0 ) {
	  matrix.row(row_pos)->mWork = 1;
	  M.insert_elem(row1->pos(), row_pos);
	}
      }
    }
  }

  vector<ymuint32> del_list;
  ymuint32 cost = 0;
  while ( M.row_num() > 0 ) {
    const McRowHead* best_row = M.row_front();
    for (const McRowHead* row1 = best_row->next(); !M.is_row_end(row1);
	 row1 = row1->next()) {
      if ( best_row->num() > row1->num() ) {
	best_row = row1;
      }
    }
    ymuint32 min_cost = UINT_MAX;
    const McRowHead* row = matrix.row(best_row->pos());
    for (const McCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      ymuint32 cpos = cell->col_pos();
      if ( min_cost > matrix.col_cost(cpos) ) {
	min_cost = matrix.col_cost(cpos);
      }
    }
    cost += min_cost;

    del_list.clear();
    del_list.reserve(best_row->num());
    for (const McCell* cell = best_row->front();
	 !best_row->is_end(cell); cell = cell->row_next()) {
      del_list.push_back(cell->col_pos());
    }
    for (ymuint i = 0; i < del_list.size(); ++ i) {
      ymuint pos = del_list[i];
      M.delete_row(pos);
      M.delete_col(pos);
    }
  }

  return cost;
}

END_NAMESPACE_YM_MINCOV
