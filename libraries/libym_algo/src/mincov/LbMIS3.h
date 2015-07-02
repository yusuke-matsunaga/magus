#ifndef LBMIS3_H
#define LBMIS3_H

/// @file LbMIS3.h
/// @brief LbMIS3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "LbCalc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class LbMIS3 LbMIS3.h "LbMIS3.h"
/// @brief MIS(maximum independent set) を用いた下界の計算クラス
///
/// get_mis() 関数を使う
//////////////////////////////////////////////////////////////////////
class LbMIS3 :
  public LbCalc
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 下界の計算をする
  virtual
  ymuint32
  operator()(const McMatrix& matrix);

};

END_NAMESPACE_YM_MINCOV

#endif // LBMIS3_H
