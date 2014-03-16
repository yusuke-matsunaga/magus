
/// @file SelCS.cc
/// @brief SelCS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SelCS.h"
#include "McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス SelCS
//////////////////////////////////////////////////////////////////////

// @brief 次の列を選ぶ．
// @param[in] matrix 対象の行列
// @return 選ばれた列番号を返す．
ymuint
SelCS::operator()(const McMatrix& matrix)
{
  // 各行にカバーしている列数に応じた重みをつけ，
  // その重みの和が最大となる列を選ぶ．
  ymuint nr = matrix.row_size();
  vector<double> row_weights(nr);
  for (const McRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    ymuint row_pos = row->pos();
    double min_cost = DBL_MAX;
    for (const McCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      const McColHead* col = matrix.col(cell->col_pos());
      double col_cost = static_cast<double>(matrix.col_cost(col->pos())) / col->num();
      if ( min_cost > col_cost ) {
	min_cost = col_cost;
      }
    }
    row_weights[row_pos] = min_cost;
  }
  double max_weight = DBL_MAX;
  ymuint max_col = 0;
  for (const McColHead* col = matrix.col_front();
       !matrix.is_col_end(col); col = col->next()) {
    double weight = matrix.col_cost(col->pos());
    for (const McCell* cell = col->front();
	 !col->is_end(cell); cell = cell->col_next()) {
      weight -= row_weights[cell->row_pos()];
    }

    cout << "Col#" << col->pos() << ": " << weight << endl;
    if ( max_weight > weight ) {
      max_weight = weight;
      max_col = col->pos();
    }
  }
  return max_col;
}

END_NAMESPACE_YM_MINCOV
