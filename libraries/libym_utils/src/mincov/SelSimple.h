#ifndef SELSIMPLE_H
#define SELSIMPLE_H

/// @file Selector.h
/// @brief Selector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class SelSimple SelSimple.h "SelSimple.h"
/// @brief 列を選ぶファンクタ
///
/// 各行にカバーしている列数に応じた重みをつけ，
/// その重みの和が最大となる列を選ぶ．
//////////////////////////////////////////////////////////////////////
class SelSimple :
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


#endif // SELSIMPLE_H
