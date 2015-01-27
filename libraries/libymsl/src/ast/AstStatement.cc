
/// @file AstStatement.cc
/// @brief AstStatement の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstStatement
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstStatement::AstStatement(const FileRegion& loc) :
  Ast(loc)
{
}

// @brief デストラクタ
AstStatement::~AstStatement()
{
}

// @brief 名前を返す．
//
// kEnumDecl, kFuncDecl, kVarDecl のみ有効
ShString
AstStatement::name() const
{
  ASSERT_NOT_REACHED;
  return ShString();
}

// @brief 型を返す．
//
// kFuncDecl, kVarDecl のみ有効
const AstType*
AstStatement::type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief パラメータリストの要素数を返す．
//
// kFuncDecl のみ有効
ymuint
AstStatement::param_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief パラメータ名を返す．
// @param[in] pos 位置 ( 0 <= pos < param_num() )
//
// kFuncDecl のみ有効
ShString
AstStatement::param_name(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return ShString();
}

// @brief パラメータの型を返す．
// @param[in] pos 位置 ( 0 <= pos < param_num() )
//
// kFuncDecl のみ有効
const AstType*
AstStatement::param_type(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief パラメータの初期値を返す．
// @param[in] pos 位置 ( 0 <= pos < param_num() )
//
// kFuncDecl のみ有効
const AstExpr*
AstStatement::param_expr(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief enum 定数の数を返す．
//
// kEnumDecl のみ有効
ymuint
AstStatement::enum_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief enum 定数名を返す．
// @param[in] pos 位置 ( 0 <= pos < enum_num() )
//
// kEnumDecl のみ有効
const AstSymbol*
AstStatement::enum_const(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief enum 定数の規定値を返す．
// @param[in] pos 位置 ( 0 <= pos < enum_num() )
//
// kEnumDecl のみ有効
const AstExpr*
AstStatement::enum_const_expr(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 左辺式を返す．
//
// kAssignment のみ有効
const AstExpr*
AstStatement::lhs_expr() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 式を返す．
//
// kAssignment,
// kDoWhile, kFor, kIf, kWhile, kSwitch
// kExprStmt, kReturn, kVarDecl のみ有効
const AstExpr*
AstStatement::expr() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 文のリストの要素数を返す．
//
// AstBlockStmt の継承クラスのみ有効
ymuint
AstStatement::stmtlist_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 文のリストの要素を返す．
// @param[in] pos 位置 ( 0 <= pos < stmt_num() )
//
// AstBlockStmt の継承クラスのみ有効
const AstStatement*
AstStatement::stmtlist_elem(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 本体の文を返す．
//
// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
const AstStatement*
AstStatement::stmt() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief else 節を得る．
//
// kIf のみ有効
const AstStatement*
AstStatement::else_stmt() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 初期化文を返す．
//
// kFor のみ有効
const AstStatement*
AstStatement::init_stmt() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 増加文を返す．
//
// kFor のみ有効
const AstStatement*
AstStatement::next_stmt() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief switch 文の case 数を返す．
//
// kSwitch のみ有効
ymuint
AstStatement::switch_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief switch 文の case ラベルを返す．
// @param[in] pos 位置 ( 0 <= pos < switch_num() )
//
// kSwitch のみ有効
const AstExpr*
AstStatement::case_label(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief switch 文の case ブロックを返す．
// @param[in] pos 位置 ( 0 <= pos < switch_num() )
//
// kSwitch のみ有効
const AstStatement*
AstStatement::case_stmt(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief ラベルを得る．
//
// kGoto, kLabel のみ有効
const AstSymbol*
AstStatement::label() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief インポートするモジュール数を返す．
//
// kImport のみ有効
ymuint
AstStatement::import_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief インポートするモジュール名を返す．
// @param[in] pos 位置 ( 0 <= pos < inport_num() )
//
// kImport のみ有効
const AstSymbol*
AstStatement::import_module(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief インポートするモジュールのエイリアスを返す．
// @param[in] pos 位置 ( 0 <= pos < inport_num() )
//
// kImport のみ有効
const AstSymbol*
AstStatement::import_alias(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

END_NAMESPACE_YM_YMSL
