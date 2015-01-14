
/// @file AstWhile.cc
/// @brief AstWhile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstWhile.h"
#include "AstExpr.h"

#include "YmslCodeList.h"
#include "YmslScope.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件式
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstWhile::AstWhile(AstExpr* cond,
		   AstStmtList* stmt_list,
		   const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc),
  mCond(cond)
{
}

// @brief デストラクタ
AstWhile::~AstWhile()
{
}

// @brief 関数の登録を行う．
// @param[in] parent_scope 親のスコープ
void
AstWhile::phase1(YmslScope* parent_scope)
{
}

// @brief スコープの生成と参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstWhile::phase2(YmslScope* parent_scope)
{
  mCond->resolve_var(parent_scope);
  AstBlockStmt::phase2(parent_scope);
}

// @brief 命令コードのサイズを計算する．
ymuint
AstWhile::calc_size()
{
  ymuint size = mCond->calc_size();
  size += AstBlockStmt::calc_size();
  size += 4;
  return size;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstWhile::compile(YmslDriver& driver,
		  YmslCodeList& code_list,
		  Ymsl_INT& addr)
{
  Ymsl_INT label1 = addr;
  mCond->compile(driver, code_list, addr);
  code_list.write_opcode(addr, YMVM_BRANCH_FALSE);
  Ymsl_INT label2_addr = addr;
  code_list.write_int(addr, 0);
  AstBlockStmt::compile(driver, code_list, addr);
  code_list.write_opcode(addr, YMVM_JUMP);
  code_list.write_int(addr, label1);
  code_list.write_int(label2_addr, addr);
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstWhile::print(ostream& s,
		ymuint indent) const
{
  print_indent(s, indent);
  s << "while ";
  mCond->print(s);
  s << " {" << endl;
  AstBlockStmt::print(s, indent + 1);
  s << "}" << endl;
}

END_NAMESPACE_YM_YMSL
