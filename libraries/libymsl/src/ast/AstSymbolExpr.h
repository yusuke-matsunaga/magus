#ifndef ASTSYMBOLEXPR_H
#define ASTSYMBOLEXPR_H

/// @file AstSymbolExpr.h
/// @brief AstSymbolExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstSymbolExpr AstSymbolExpr.h "AstSymbolExpr.h"
/// @brief シンボル式を表すクラス
//////////////////////////////////////////////////////////////////////
class AstSymbolExpr :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] symbol シンボル
  AstSymbolExpr(AstSymbol* symbol);

  /// @brief デストラクタ
  virtual
  ~AstSymbolExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief シンボル名を返す．
  ///
  /// kSymbolExpr のみ有効
  virtual
  const AstSymbol*
  symbol() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // シンボル
  AstSymbol* mSymbol;

};

END_NAMESPACE_YM_YMSL

#endif // ASTSYMBOLEXPR_H
