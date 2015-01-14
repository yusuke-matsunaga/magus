
/// @file AstExprStmt.cc
/// @brief AstExprStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstExprStmt.h"
#include "AstExpr.h"

#include "YmslCodeList.h"
#include "YmslScope.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstExprStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 式
AstExprStmt::AstExprStmt(AstExpr* expr) :
  AstStatement(expr->file_region()),
  mExpr(expr)
{
}

// @brief デストラクタ
AstExprStmt::~AstExprStmt()
{
}

// @brief スコープの生成と関数の登録を行う．
// @param[in] parent_scope 親のスコープ
void
AstExprStmt::phase1(YmslScope* parent_scope)
{
}

// @brief 参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstExprStmt::phase2(YmslScope* parent_scope)
{
  mExpr->resolve_var(parent_scope);
}

// @brief 命令コードのサイズを計算する．
ymuint
AstExprStmt::calc_size()
{
  return mExpr->calc_size();
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstExprStmt::compile(YmslDriver& driver,
		     YmslCodeList& code_list,
		     Ymsl_INT& addr)
{
  mExpr->compile(driver, code_list, addr);
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstExprStmt::print(ostream& s,
		   ymuint indent) const
{
  print_indent(s, indent);
  mExpr->print(s);
  s << endl;
}

END_NAMESPACE_YM_YMSL
