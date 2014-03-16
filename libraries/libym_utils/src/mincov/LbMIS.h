#ifndef LBMIS_H
#define LBMIS_H

/// @file LbMIS.h
/// @brief LbMIS のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbCalc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class LbMIS LbMIS.h "LbMIS.h"
/// @brief MIS(maximum independent set) を用いた下界の計算クラス
//////////////////////////////////////////////////////////////////////
class LbMIS :
  LbCalc
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
