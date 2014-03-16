
/// @file LbCS.cc
/// @brief LbCS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbCS.h"
#include "McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LBCS
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] matrix 対象の行列
// @return 下限値
ymuint32
LbCS::operator()(const McMatrix& matrix)
{
  double cost = 0.0;
  for (const McRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    double min_cost = DBL_MAX;
    for (const McCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      const McColHead* col = matrix.col(cell->col_pos());
      double col_cost = static_cast<double>(matrix.col_cost(col->pos())) / col->num();
      if ( min_cost > col_cost ) {
	min_cost = col_cost;
      }
    }
    cost += min_cost;
  }
  cost = ceil(cost);

  return static_cast<ymuint32>(cost);
}

END_NAMESPACE_YM_MINCOV
