#ifndef COMBSET_H
#define COMBSET_H

/// @file CombSet.h
/// @brief CombSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/zdd_nsdef.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class CombSet CombSet.h "CombSet.h"
/// @brief 組み合わせ集合を表す単純なクラス
//////////////////////////////////////////////////////////////////////
class CombSet
{
public:

  /// @brief コンストラクタ
  /// @param[in] num 要素数
  /// @note 空の集合となる．
  CombSet();

  /// @brief デストラクタ
  ~CombSet();


public:

  /// @brief
};


END_NAMESPACE_YM_ZDD

#endif // COMBSET_H
