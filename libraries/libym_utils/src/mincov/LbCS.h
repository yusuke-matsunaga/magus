#ifndef LBCS_H
#define LBCS_H

/// @file LbCS.h
/// @brief LbCS のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbCalc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class LbCS LbCS.h "LbCS.h"
/// @brief column splitting を用いた下界の計算クラス
//////////////////////////////////////////////////////////////////////
class LbCS :
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


#endif // LBCS_H
