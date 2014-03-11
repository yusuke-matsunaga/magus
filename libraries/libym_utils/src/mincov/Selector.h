#ifndef SELECTOR_H
#define SELECTOR_H

/// @file Selector.h
/// @brief Selector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class Selector Selector.h "Selector.h"
/// @brief 列を選ぶファンクタ
//////////////////////////////////////////////////////////////////////
class Selector
{
public:

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の列を選ぶ．
  /// @param[in] matrix 対象の行列
  /// @return 選ばれた列番号を返す．
  ymuint
  operator()(const McMatrix& matrix);

};

END_NAMESPACE_YM_MINCOV


#endif // SELECTOR_H
