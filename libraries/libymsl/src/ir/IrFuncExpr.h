#ifndef YMSLFUNCEXPR_H
#define YMSLFUNCEXPR_H

/// @file IrFuncExpr.h
/// @brief IrFuncExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFuncExpr IrFuncExpr.h "IrFuncExpr.h"
/// @brief 関数を表す IrExpr
//////////////////////////////////////////////////////////////////////
class IrFuncExpr :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 関数
  IrFuncExpr(IrFunction* func);

  /// @brief デストラクタ
  virtual
  ~IrFuncExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 関数を返す．
  ///
  /// kFuncExpr のみ有効
  virtual
  IrFunction*
  function() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  IrFunction* mFunction;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLFUNCEXPR_H
