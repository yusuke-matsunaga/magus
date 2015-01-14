
/// @file AstDoWhile.cc
/// @brief AstDoWhile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstDoWhile.h"
#include "AstExpr.h"

#include "YmslCodeList.h"
#include "YmslDriver.h"
#include "YmslLabel.h"
#include "YmslScope.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstDoWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt_list 本体の文
// @param[in] cond 条件式
// @param[in] loc ファイル位置
AstDoWhile::AstDoWhile(AstStmtList* stmt_list,
		       AstExpr* cond,
		       const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc),
  mCond(cond)
{
}

// @brief デストラクタ
AstDoWhile::~AstDoWhile()
{
}

// @brief スコープの生成と変数名の参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstDoWhile::phase1(YmslScope* parent_scope)
{
  mCond->resolve_var(parent_scope);
  AstBlockStmt::phase1(parent_scope);
}

// @brief 命令コードのサイズを計算する．
ymuint
AstDoWhile::calc_size()
{
  ymuint size = AstBlockStmt::calc_size();
  size += mCond->calc_size();
  size += 2;
  return size;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstDoWhile::compile(YmslDriver& driver,
		    YmslCodeList& code_list,
		    Ymsl_INT& addr)
{
  Ymsl_INT label1 = addr;
  AstBlockStmt::compile(driver, code_list, addr);
  mCond->compile(driver, code_list, addr);
  code_list.write_opcode(addr, YMVM_BRANCH_TRUE);
  code_list.write_int(addr, label1);
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstDoWhile::print(ostream& s,
		  ymuint indent) const
{
  print_indent(s, indent);
  s << "do {" << endl;

  AstBlockStmt::print(s, indent + 1);

  print_indent(s, indent);
  s << "while ";
  mCond->print(s);
  s << endl;
}

END_NAMESPACE_YM_YMSL
