
/// @file AstIf.cc
/// @brief AstIf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstIf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstIf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 条件式
// @param[in] then_stmt 条件が成り立った時実行される文のリスト
// @param[in] else_stmt 条件が成り立たなかった時実行される文
// @param[in] loc ファイル位置
AstIf::AstIf(AstExpr* expr,
	     AstStatement* then_stmt,
	     AstStatement* else_stmt,
	     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr),
  mStmt(then_stmt),
  mElseStmt(else_stmt)
{
}

// @brief デストラクタ
AstIf::~AstIf()
{
}

// @brief 種類を返す．
StmtType
AstIf::stmt_type() const
{
  return kIf;
}

// @brief 式を返す．
//
// kAssignment,
// kDoWhile, kFor, kIf, kWhile, kSwitch
// kExprStmt, kReturn, kVarDecl のみ有効
const AstExpr*
AstIf::expr() const
{
  return mExpr;
}

// @brief 本体の文を返す．
//
// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
const AstStatement*
AstIf::stmt() const
{
  return mStmt;
}

// @brief else 節を得る．
//
// kIf のみ有効
const AstStatement*
AstIf::else_stmt() const
{
  return mElseStmt;
}

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstIf::phase1(YmslScope* parent_scope,
	      YmslTypeMgr* type_mgr)
{
  ymuint n = mIfBlockList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mIfBlockList[i]->phase1(parent_scope, type_mgr);
  }
}

// @brief 参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstIf::phase2(YmslScope* parent_scope)
{
  ymuint n = mIfBlockList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mIfBlockList[i]->phase2(parent_scope);
  }
}

// @brief 命令コードのサイズを計算する．
ymuint
AstIf::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstIf::compile(YmslDriver& driver,
	       YmslCodeList& code_list,
	       Ymsl_INT& addr)
{
  ymuint n = mIfBlockList.size();
  vector<Ymsl_INT> addr_list;
  for (ymuint i = 0; i < n; ++ i) {
    AstIfBlock* if_block = mIfBlockList[i];
    AstExpr* cond = if_block->cond();
    if ( cond != NULL ) {
      cond->compile(driver, code_list, addr);
      code_list.write_opcode(addr, YMVM_BRANCH_FALSE);
      Ymsl_INT label1_addr = addr;
      code_list.write_int(addr, 0);
      if_block->compile(driver, code_list, addr);
      if ( i < n - 1 ) {
	code_list.write_opcode(addr, YMVM_JUMP);
	addr_list.push_back(addr);
	code_list.write_int(addr, 0);
      }
      code_list.write_int(label1_addr, addr);
    }
    else {
      ASSERT_COND( i == n - 1 );
      if_block->compile(driver, code_list, addr);
    }
  }
  for (vector<Ymsl_INT>::iterator p = addr_list.begin();
       p != addr_list.end(); ++ p) {
    Ymsl_INT label_addr = *p;
    code_list.write_int(label_addr, addr);
  }
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstIf::print(ostream& s,
	     ymuint indent) const
{
  ymuint n = mIfBlockList.size();
  for (ymuint i = 0; i < n; ++ i) {
    AstIfBlock* if_block = mIfBlockList[i];
    AstExpr* cond = if_block->cond();
    print_indent(s, indent);
    if ( cond != NULL ) {
      if ( i == 0 ) {
	s << "if ";
      }
      else {
	s << "elif ";
      }
      cond->print(s);
    }
    else {
      s << "else";
    }
    s << " {" << endl;
    if_block->print(s, indent + 1);
    print_indent(s, indent);
    s << "}" << endl;
  }
}
#endif

END_NAMESPACE_YM_YMSL
