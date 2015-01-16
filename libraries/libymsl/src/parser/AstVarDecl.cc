
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
  mIndex(-1),
  mType(type),
  mExpr(expr)
{
}

// @brief デストラクタ
AstVarDecl::~AstVarDecl()
{
}

// @brief インデックス番号を返す．
ymuint
AstVarDecl::index() const
{
  return mIndex;
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

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstVarDecl::phase1(YmslScope* parent_scope,
		   YmslTypeMgr* type_mgr)
{
  //parent_scope->add_var(this);

  if ( mExpr != NULL ) {
    mExpr->resolve_var(parent_scope);
  }
}

// @brief 参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstVarDecl::phase2(YmslScope* parent_scope)
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstVarDecl::calc_size()
{
  return 0;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstVarDecl::compile(YmslDriver& driver,
		    YmslCodeList& code_list,
		    Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstVarDecl::print(ostream& s,
		  ymuint indent) const
{
  print_indent(s, indent);
  s << "var " << name() << ": ";
  type()->print(s);
  if ( expr() != NULL ) {
    expr()->print(s);
  }
  s << ";" << endl;
}
#endif

END_NAMESPACE_YM_YMSL
