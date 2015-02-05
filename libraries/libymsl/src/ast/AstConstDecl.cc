
/// @file AstConstDecl.cc
/// @brief AstConstDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstConstDecl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstConstDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 定数名
// @param[in] type 型
// @param[in] expr 初期化式
// @param[in] loc ファイル位置
AstConstDecl::AstConstDecl(AstSymbol* name,
			   AstType* type,
			   AstExpr* expr,
			   const FileRegion& loc) :
  AstStatement(loc),
  mName(name),
  mType(type),
  mExpr(expr)
{
}

// @brief デストラクタ
AstConstDecl::~AstConstDecl()
{
}

// @brief 種類を返す．
StmtType
AstConstDecl::stmt_type() const
{
  return kConstDecl;
}

// @brief 名前を返す．
//
// kEnumDecl, kFuncDecl, kVarDecl, kConstDecl のみ有効
const AstSymbol*
AstConstDecl::name() const
{
  return mName;
}

// @brief 型を返す．
//
// kFuncDecl, kVarDecl, kConstDecl のみ有効
const AstType*
AstConstDecl::type() const
{
  return mType;
}

// @brief 式を返す．
//
// kExprStmt, kReturn, kVarDecl, kConstDecl のみ有効
const AstExpr*
AstConstDecl::expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_YMSL
