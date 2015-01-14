
/// @file AstIf.cc
/// @brief AstIf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstIf.h"
#include "AstIfBlock.h"
#include "AstExpr.h"
#include "AstList.h"

#include "YmslCodeList.h"
#include "YmslScope.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstIf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] if_list IfBlock のリスト
// @param[in] loc ファイル位置
AstIf::AstIf(AstIfList* if_list,
	     const FileRegion& loc) :
  AstStatement(loc),
  mIfBlockList(if_list->size())
{
  ymuint pos = 0;
  for (AstIfList::Iterator p = if_list->begin();
       !p.is_end(); p.next()) {
    mIfBlockList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstIf::~AstIf()
{
}

// @brief 関数の登録を行う．
// @param[in] parent_scope 親のスコープ
void
AstIf::phase1(YmslScope* parent_scope)
{
}

// @brief スコープの生成と参照解決を行う．
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


//////////////////////////////////////////////////////////////////////
// クラス AstIfBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cond 条件
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstIfBlock::AstIfBlock(AstExpr* cond,
		       AstStmtList* stmt_list,
		       const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc),
  mCond(cond)
{
}

// @brief デストラクタ
AstIfBlock::~AstIfBlock()
{
}

// @brief 条件を返す．
AstExpr*
AstIfBlock::cond() const
{
  return mCond;
}

// @brief 関数の登録を行う．
// @param[in] parent_scope 親のスコープ
void
AstIfBlock::phase1(YmslScope* parent_scope)
{
}

// @brief スコープの生成と参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstIfBlock::phase2(YmslScope* parent_scope)
{
  mCond->resolve_var(parent_scope);
  AstBlockStmt::phase2(parent_scope);
}

END_NAMESPACE_YM_YMSL
