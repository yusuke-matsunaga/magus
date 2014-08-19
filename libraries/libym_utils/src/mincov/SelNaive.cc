
/// @file SelNaive.cc
/// @brief SelNaive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SelNaive.h"
#include "McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス SelNaive
//////////////////////////////////////////////////////////////////////

// @brief 次の列を選ぶ．
// @param[in] matrix 対象の行列
// @return 選ばれた列番号を返す．
ymuint
SelNaive::operator()(const McMatrix& matrix)
{
  // 最もカバーしている行の多い列を選ぶ
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
}

END_NAMESPACE_YM_MINCOV
