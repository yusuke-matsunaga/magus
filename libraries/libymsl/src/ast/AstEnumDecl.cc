
/// @file AstEnumDecl.cc
/// @brief AstEnumDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstEnumDecl.h"
#include "AstEnumConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstEnumDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] name 名前
// @param[in] const_list 定数リスト
// @param[in] loc ファイル位置
AstEnumDecl::AstEnumDecl(AstSymbol* name,
			 ymuint const_num,
			 AstEnumConst** const_list,
			 const FileRegion& loc) :
  AstStatement(loc),
  mName(name),
  mConstNum(const_num),
  mConstList(const_list)
{
}

// @brief デストラクタ
AstEnumDecl::~AstEnumDecl()
{
}

// @brief 種類を返す．
StmtType
AstEnumDecl::stmt_type() const
{
  return kEnumDecl;
}

// @brief enum 定数の数を返す．
//
// kEnumDecl のみ有効
ymuint
AstEnumDecl::enum_num() const
{
  return mConstNum;
}

// @brief enum 定数を返す．
// @param[in] pos 位置 ( 0 <= pos < enum_num() )
//
// kEnumDecl のみ有効
const AstEnumConst*
AstEnumDecl::enum_const(ymuint pos) const
{
  ASSERT_COND( pos < enum_num() );
  return mConstList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス AstEnumConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] expr 値を表す式
// @param[in] loc ファイル位置
AstEnumConst::AstEnumConst(AstSymbol* name,
			   AstExpr* expr,
			   const FileRegion& loc) :
  Ast(loc),
  mName(name),
  mExpr(expr)
{
}

// @brief デストラクタ
AstEnumConst::~AstEnumConst()
{
}

// @brief 名前を返す．
const AstSymbol*
AstEnumConst::name() const
{
  return mName;
}

// @brief 値を表す式を返す．
//
// NULL の場合もある．
const AstExpr*
AstEnumConst::expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_YMSL
