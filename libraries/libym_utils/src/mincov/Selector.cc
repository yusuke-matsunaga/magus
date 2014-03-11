
/// @file Selector.cc
/// @brief Selector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Selector.h"
#include "McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス Selector
//////////////////////////////////////////////////////////////////////

// @brief 次の列を選ぶ．
// @param[in] matrix 対象の行列
// @return 選ばれた列番号を返す．
ymuint
Selector::operator()(const McMatrix& matrix)
{
#if 0
  ymuint max_num = 0;
  ymuint max_col = 0;
  for (const McColHead* col = matrix.col_front();
       !matrix.is_col_end(col); col = col->next()) {
    if ( max_num < col->num() ) {
      max_num = col->num();
      max_col = col->pos();
    }
  }
  return max_col;
#else
  ymuint nr = matrix.row_size();
  vector<double> row_weights(nr);
  for (const McRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    ymuint row_pos = row->pos();
    assert_cond( row->num() > 1, __FILE__, __LINE__);
    row_weights[row_pos] = 1.0 / (row->num() - 1);
  }
  double max_weight = 0.0;
  ymuint max_col = 0;
  for (const McColHead* col = matrix.col_front();
       !matrix.is_col_end(col); col = col->next()) {
    double weight = 0.0;
    for (const McCell* cell = col->front();
	 !col->is_end(cell); cell = cell->col_next()) {
      weight += row_weights[cell->row_pos()];
    }
    if ( max_weight < weight ) {
      max_weight = weight;
      max_col = col->pos();
    }
  }
  return max_col;
#endif
}

END_NAMESPACE_YM_MINCOV
