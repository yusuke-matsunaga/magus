#ifndef YMSLVAREXPR_H
#define YMSLVAREXPR_H

/// @file IrVarExpr.h
/// @brief IrVarExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrVarExpr IrVarExpr.h "IrVarExpr.h"
/// @brief 変数式を表すクラス
//////////////////////////////////////////////////////////////////////
class IrVarExpr :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] var 変数
  IrVarExpr(Var* var);

  /// @brief デストラクタ
  virtual
  ~IrVarExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 変数を返す．
  ///
  /// kPrimary の時のみ有効
  virtual
  Var*
  var() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数
  Var* mVar;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLVAREXPR_H
