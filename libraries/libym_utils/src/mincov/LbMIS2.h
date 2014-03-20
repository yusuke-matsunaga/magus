#ifndef LBMIS2_H
#define LBMIS2_H

/// @file LbMIS2.h
/// @brief LbMIS2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbCalc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class LbMIS2 LbMIS2.h "LbMIS2.h"
/// @brief MIS(maximum independent set) を用いた下界の計算クラス
///
/// ヒープを使って最小要素を取り出す．
//////////////////////////////////////////////////////////////////////
class LbMIS2 :
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

#endif // LBMIS_H
