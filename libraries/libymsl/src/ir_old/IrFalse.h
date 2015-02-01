#ifndef YMSLFALSE_H
#define YMSLFALSE_H

/// @file IrFalse.h
/// @brief IrFalse のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFalse IrFalse.h "IrFalse.h"
/// @brief false を表すクラス
//////////////////////////////////////////////////////////////////////
class IrFalse :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  IrFalse();

  /// @brief デストラクタ
  virtual
  ~IrFalse();


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

#endif // YMSLFALSE_H
