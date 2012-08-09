
/// @file CellLut.cc
/// @brief  CellLut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLut.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellLutTemplate
//////////////////////////////////////////////////////////////////////

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
CellLutTemplate::dump(BinO& s) const
{
  ymuint8 d = dimension();
  s << name()
    << d;
  for (ymuint i = 0; i < d; ++ i) {
    s << static_cast<ymuint8>(variable_type(i));
    ymuint8 n = index_num(i);
    s << n;
    for (ymuint j = 0; j < n; ++ j) {
      s << index(i, j);
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス CellLut
//////////////////////////////////////////////////////////////////////

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
CellLut::dump(BinO& s) const
{
  s << lut_template()->name()
    << name();
  ymuint8 d = dimension();
  for (ymuint i = 0; i < d; ++ i) {
    ymuint n = index_num(i);
    for (ymuint j = 0; j < n; ++ j) {
      s << index(i, j);
    }
  }

  vector<ymuint32> pos_array(d);
  ymuint n = 1;
  for (ymuint i = 0; i < d; ++ i) {
    n *= index_num(i);
  }
  for (ymuint v = 0; v < n; ++ v) {
    ymuint v0 = v;
    for (ymuint j = 0; j < d; ++ j) {
      pos_array[j] = v0 % index_num(j);
      v0 /= index_num(j);
    }
    double val = value(pos_array);
    s << val;
  }
}

END_NAMESPACE_YM_CELL
