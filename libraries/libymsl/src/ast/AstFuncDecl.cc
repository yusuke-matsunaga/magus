
/// @file AstDeclBase.cc
/// @brief AstDeclBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFuncDecl.h"
#include "AstParam.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFuncDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] type 型
// @param[in] param_num パラメータの数
// @param[in] param_list パラメータリスト
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstFuncDecl::AstFuncDecl(AstSymbol* name,
			 AstType* type,
			 ymuint param_num,
			 AstParam** param_list,
			 AstStatement* stmt,
			 const FileRegion& loc) :
  AstStatement(loc),
  mName(name),
  mType(type),
  mParamNum(param_num),
  mParamList(param_list),
  mStmt(stmt)
{
}

// @brief デストラクタ
AstFuncDecl::~AstFuncDecl()
{
}

// @brief 種類を返す．
StmtType
AstFuncDecl::stmt_type() const
{
  return kFuncDecl;
}

// @brief 名前を得る．
const AstSymbol*
AstFuncDecl::name() const
{
  return mName;
}

// @brief 出力の型を返す．
const AstType*
AstFuncDecl::type() const
{
  return mType;
}

// @brief パラメータリストの要素数を返す．
//
// kFuncDecl のみ有効
ymuint
AstFuncDecl::param_num() const
{
  return mParamNum;
}

// @brief パラメータ名を返す．
// @param[in] pos 位置 ( 0 <= pos < param_num() )
//
// kFuncDecl のみ有効
const AstSymbol*
AstFuncDecl::param_name(ymuint pos) const
{
  ASSERT_COND( pos < param_num() );
  return mParamList[pos]->name();
}

// @brief パラメータの型を返す．
// @param[in] pos 位置 ( 0 <= pos < param_num() )
//
// kFuncDecl のみ有効
const AstType*
AstFuncDecl::param_type(ymuint pos) const
{
  ASSERT_COND( pos < param_num() );
  return mParamList[pos]->type();
}

// @brief パラメータの初期値を返す．
// @param[in] pos 位置 ( 0 <= pos < param_num() )
//
// kFuncDecl のみ有効
const AstExpr*
AstFuncDecl::param_expr(ymuint pos) const
{
  ASSERT_COND( pos < param_num() );
  return mParamList[pos]->init_expr();
}

// @brief 本体の文を返す．
//
// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
const AstStatement*
AstFuncDecl::stmt() const
{
  return mStmt;
}

END_NAMESPACE_YM_YMSL
