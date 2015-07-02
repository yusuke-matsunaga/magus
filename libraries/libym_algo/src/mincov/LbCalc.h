#ifndef LBCALC_H
#define LBCALC_H

/// @file LbCalc.h
/// @brief LbCalc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class LbCalc LbCalc.h "LbCalc.h"
/// @brief 下界の計算クラス
//////////////////////////////////////////////////////////////////////
class LbCalc
{
public:

  /// @brief デストラクタ
  virtual
  ~LbCalc() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 下界の計算をする
  virtual
  ymuint32
  operator()(const McMatrix& matrix) = 0;

};

END_NAMESPACE_YM_MINCOV


#endif // LBMIS_H
