
/// @file AstSymbolExpr.cc
/// @brief AstSymbolExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstSymbolExpr.h"
#include "AstSymbol.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstSymbolExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] symbol シンボル
AstSymbolExpr::AstSymbolExpr(AstSymbol* symbol) :
  AstExpr(symbol->file_region()),
  mSymbol(symbol)
{
}

// @brief デストラクタ
AstSymbolExpr::~AstSymbolExpr()
{
}

// @brief 種類を返す．
AstExpr::Type
AstSymbolExpr::expr_type() const
{
  return kSymbolExpr;
}

// @brief シンボル名を返す．
//
// kSymbol のみ有効
const AstSymbol*
AstSymbolExpr::symbol() const
{
  return mSymbol;
}

END_NAMESPACE_YM_YMSL
