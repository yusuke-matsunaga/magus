
/// @file LbMIS.cc
/// @brief LbMIS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbMIS.h"
#include "McMatrix.h"
#include "MaxClique.h"
#include "utils/MFSet.h"


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
      rpos0 = rset.merge(rpos0, rpos);
    }
  }

  vector<ymuint32> row_list;
  row_list.reserve(rs);
  for (const McRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    row_list.push_back(row->pos());
  }

  ymuint32 nr = row_list.size();
  for (ymuint i = 0; i < nr - 1; ++ i) {
    ymuint32 rpos1 = row_list[i];
    row_list[i] = rset.find(rpos1);
  }
  MaxClique mc(nr);
  for (ymuint i = 0; i < nr - 1; ++ i) {
    ymuint32 rpos1 = row_list[i];
    for (ymuint j = i + 1; j < nr; ++ j) {
      ymuint32 rpos2 = row_list[j];
      if ( rpos1 != rpos2 ) {
	mc.connect(i, j);
      }
    }
  }
  for (ymuint i = 0; i < nr; ++ i) {
    ymuint32 rpos1 = row_list[i];
    const McRowHead* row = matrix.row(rpos1);
    ymuint32 min_cost = UINT_MAX;
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
  ymuint32 cost = mc.solve(mis);

  return cost;
}

END_NAMESPACE_YM_MINCOV
