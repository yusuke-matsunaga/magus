#ifndef YMSLSCOPEEXPR_H
#define YMSLSCOPEEXPR_H

/// @file IrScopeExpr.h
/// @brief IrScopeExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrScopeExpr IrScopeExpr.h "IrScopeExpr.h"
/// @brief scope を表す IrExpr
//////////////////////////////////////////////////////////////////////
class IrScopeExpr :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] scope スコープ
  IrScopeExpr(Scope* scope);

  /// @brief デストラクタ
  virtual
  ~IrScopeExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief スコープを返す．
  ///
  /// kScopeExpr のみ有効
  virtual
  Scope*
  scope() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // スコープ
  Scope* mScope;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLSCOPEEXPR_H
