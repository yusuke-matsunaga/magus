
/// @file AstVarDecl.cc
/// @brief AstVarDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstVarDecl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstVarDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] type 型
// @param[in] expr 初期化式
// @param[in] loc ファイル位置
AstVarDecl::AstVarDecl(ShString name,
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
AstVarDecl::~AstVarDecl()
{
}

// @brief 種類を返す．
StmtType
AstVarDecl::stmt_type() const
{
  return kVarDecl;
}

// @brief 名前を返す．
//
// kEnumDecl, kFuncDecl, kVarDecl のみ有効
ShString
AstVarDecl::name() const
{
  return mName;
}

// @brief 型を返す．
//
// kFuncDecl, kVarDecl のみ有効
const AstType*
AstVarDecl::type() const
{
  return mType;
}

// @brief 式を返す．
//
// kExprStmt, kReturn, kVarDecl のみ有効
const AstExpr*
AstVarDecl::expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_YMSL
