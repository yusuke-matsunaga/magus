#ifndef SELNAIVE_H
#define SELNAIVE_H

/// @file SelNaive.h
/// @brief SelNaive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class SelNaive SelNaive.h "SelNaive.h"
/// @brief 単純な Selector
///
/// 最もカバーしている行の多い列を選ぶ
//////////////////////////////////////////////////////////////////////
class SelNaive :
  public Selector
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の列を選ぶ．
  /// @param[in] matrix 対象の行列
  /// @return 選ばれた列番号を返す．
  virtual
  ymuint
  operator()(const McMatrix& matrix);

};

END_NAMESPACE_YM_MINCOV


#endif // SELECTOR_H
