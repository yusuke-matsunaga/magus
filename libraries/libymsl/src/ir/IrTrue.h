#ifndef YMSLTRUE_H
#define YMSLTRUE_H

/// @file IrTrue.h
/// @brief IrTrue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrTrue IrTrue.h "IrTrue.h"
/// @brief true を表すクラス
//////////////////////////////////////////////////////////////////////
class IrTrue :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  IrTrue();

  /// @brief デストラクタ
  virtual
  ~IrTrue();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  ExprType
  expr_type() const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLTRUE_H
