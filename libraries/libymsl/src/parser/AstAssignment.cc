
/// @file AstAssignment.cc
/// @brief AstAssignment の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstAssignment.h"
#include "AstExpr.h"

#include "YmslCodeList.h"
#include "YmslScope.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

#include "grammer.hh"

//////////////////////////////////////////////////////////////////////
// クラス AstAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] token トークン
// @param[in] left 左辺
// @param[in] right 右辺
AstAssignment::AstAssignment(TokenType token,
			     AstExpr* left,
			     AstExpr* right) :
  AstStatement(FileRegion(left->file_region(), right->file_region())),
  mToken(token),
  mLeft(left),
  mRight(right)
{
}

// @brief デストラクタ
AstAssignment::~AstAssignment()
{
}

// @brief 関数の登録を行う．
// @param[in] parent_scope 親のスコープ
void
AstAssignment::phase1(YmslScope* parent_scope)
{
}

// @brief スコープの生成と参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstAssignment::phase2(YmslScope* parent_scope)
{
  mLeft->resolve_var(parent_scope);
  mRight->resolve_var(parent_scope);
}

// @brief 命令コードのサイズを計算する．
ymuint
AstAssignment::calc_size()
{
  ymuint size = mRight->calc_size();
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
AstAssignment::compile(YmslDriver& driver,
		       YmslCodeList& code_list,
		       Ymsl_INT& addr)
{
#if 0
  mRight->compile(driver, code_list, addr);

  Ymsl_CODE op = mLeft->opcode();
  code_list.write_opcode(addr, op);
  code_list.write_int(addr, mLeft->var()->index());
#endif
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstAssignment::print(ostream& s,
		     ymuint indent) const
{
  print_indent(s, indent);
  mLeft->print(s);
  switch ( mToken ) {
  case EQ:       s << " = "; break;
  case EQPLUS:   s << " += "; break;
  case EQMINUS:  s << " -= "; break;
  case EQMULT:   s << " *= "; break;
  case EQDIV:    s << " /= "; break;
  case EQMOD:    s << " %= "; break;
  case EQLSHIFT: s << " <<= "; break;
  case EQRSHIFT: s << " >>= "; break;
  case EQBITAND: s << " &= "; break;
  case EQBITOR:  s << " |= "; break;
  case EQBITXOR: s << " ^= "; break;
  default: ASSERT_NOT_REACHED;
  }
  mRight->print(s);
  s << endl;
}

END_NAMESPACE_YM_YMSL
