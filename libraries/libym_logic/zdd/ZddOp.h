#ifndef ZDDOP_H
#define ZDDOP_H

/// @file ZddOp.h
/// @brief ZddOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/zdd_nsdef.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddOp ZddOp.h "ZddOp.h"
/// @brief ZDD の二項演算を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddBinOp
{
public:

  /// @brief コンストラクタ
  ZddBinOp() { }

  /// @brief デストラクタ
  virtual
  ~ZddBinOp() { }


public:

  /// @brief 演算を行う関数
  /// @param[in] left, right オペランド
  virtual
  ZddEdge
  operator()(ZddEdge left,
	     ZddEdge right) = 0;

};

END_NAMESPACE_YM_ZDD

#endif // ZDDOP_H
