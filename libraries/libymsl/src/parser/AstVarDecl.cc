
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
// @param[in] init_expr 初期化式
// @param[in] global グローバル変数の時 true にするフラグ
// @param[in] loc ファイル位置
AstVarDecl::AstVarDecl(ShString name,
		       ValueType type,
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
  mPrev = NULL;
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
ValueType
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

// @brief 前の要素を取り出す．
AstVarDecl*
AstVarDecl::prev() const
{
  return mPrev;
}

// @brief 前の要素をセットする．
void
AstVarDecl::set_prev(AstVarDecl* prev)
{
  mPrev = prev;
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
}

END_NAMESPACE_YM_YMSL
