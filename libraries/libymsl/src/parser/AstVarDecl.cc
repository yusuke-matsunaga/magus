
/// @file AstVarDecl.cc
/// @brief AstVarDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstVarDecl.h"
#include "AstType.h"
#include "YmslScope.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstVarDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] global グローバル変数の時 true にするフラグ
// @param[in] loc ファイル位置
AstVarDecl::AstVarDecl(ShString name,
		       AstType* type,
		       AstExpr* init_expr,
		       bool global,
		       const FileRegion& loc) :
  AstStatement(loc),
  mName(name),
  mIndex(-1),
  mType(type),
  mInitExpr(init_expr),
  mGlobal(global)
{
}

// @brief デストラクタ
AstVarDecl::~AstVarDecl()
{
}

// @brief 名前を得る．
ShString
AstVarDecl::name() const
{
  return mName;
}

// @brief インデックス番号を返す．
ymuint
AstVarDecl::index() const
{
  return mIndex;
}

// @brief 型を得る．
AstType*
AstVarDecl::type() const
{
  return mType;
}

// @brief 初期化式を返す．
//
// NULL の場合もある．
AstExpr*
AstVarDecl::init_expr() const
{
  return mInitExpr;
}

// @brief グローバル変数の時 true を返す．
bool
AstVarDecl::global() const
{
  return mGlobal;
}

// @brief スコープの生成と変数名の参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstVarDecl::phase1(YmslScope* parent_scope)
{
  //parent_scope->add_var(this);
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
  if ( global() ) {
    s << "global ";
  }
  s << "var " << name() << ": ";
  type()->print(s);
  s << ";" << endl;
}

END_NAMESPACE_YM_YMSL
