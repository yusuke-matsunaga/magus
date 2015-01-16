
/// @file AstReturn.cc
/// @brief AstReturn の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstReturn.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstReturn
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 値
// @param[in] loc ファイル位置
AstReturn::AstReturn(AstExpr* expr,
		     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr)
{
}

// @brief デストラクタ
AstReturn::~AstReturn()
{
}

// @brief 種類を返す．
StmtType
AstReturn::stmt_type() const
{
  return kReturn;
}

// @brief 式を返す．
//
// kExprStmt, kReturn のみ有効
const AstExpr*
AstReturn::expr() const
{
  return mExpr;
}

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstReturn::phase1(YmslScope* parent_scope,
		  YmslTypeMgr* type_mgr)
{
  if ( mExpr != NULL ) {
    mExpr->resolve_var(parent_scope);
  }
}

// @brief 参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstReturn::phase2(YmslScope* parent_scope)
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstReturn::calc_size()
{
  ymuint size = 1;
  if ( mExpr != NULL ) {
    size += mExpr->calc_size();
  }
  return size;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstReturn::compile(YmslDriver& driver,
		   YmslCodeList& code_list,
		   Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstReturn::print(ostream& s,
		 ymuint indent) const
{
  print_indent(s, indent);
  s << "return ";
  if ( mExpr != NULL ) {
    mExpr->print(s);
  }
  s << endl;
}
#endif

END_NAMESPACE_YM_YMSL
