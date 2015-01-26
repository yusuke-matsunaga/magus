
/// @file AstDoWhile.cc
/// @brief AstDoWhile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstDoWhile.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstDoWhile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt 本体の文
// @param[in] cond 条件式
// @param[in] loc ファイル位置
AstDoWhile::AstDoWhile(AstStatement* stmt,
		       AstExpr* cond,
		       const FileRegion& loc) :
  AstStatement(loc),
  mStmt(stmt),
  mExpr(cond)
{
}

// @brief デストラクタ
AstDoWhile::~AstDoWhile()
{
}

// @brief 種類を返す．
StmtType
AstDoWhile::stmt_type() const
{
  return kDoWhile;
}

// @brief 式を返す．
//
// kAssignment,
// kDoWhile, kFor, kIf, kWhile, kSwitch
// kExprStmt, kReturn, kVarDecl のみ有効
const AstExpr*
AstDoWhile::expr() const
{
  return mExpr;
}

// @brief 本体の文を返す．
//
// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
const AstStatement*
AstDoWhile::stmt() const
{
  return mStmt;
}

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstDoWhile::phase1(YmslScope* parent_scope,
		   YmslTypeMgr* type_mgr)
{
  mCond->resolve_var(parent_scope);
  AstBlockStmt::phase1(parent_scope, type_mgr);
}

// @brief 関数の参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstDoWhile::phase2(YmslScope* parent_scope)
{
  AstBlockStmt::phase2(parent_scope);
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
#endif

END_NAMESPACE_YM_YMSL
