
/// @file AstAssignment.cc
/// @brief AstAssignment の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstAssignment.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstAssignment
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt_type 文の種類
// @param[in] left 左辺
// @param[in] right 右辺
// @param[in] loc ファイル位置
AstAssignment::AstAssignment(StmtType stmt_type,
			     AstLeaf* left,
			     AstExpr* right,
			     const FileRegion& loc) :
  AstStatement(loc),
  mStmtType(stmt_type),
  mLeft(left),
  mRight(right)
{
}

// @brief デストラクタ
AstAssignment::~AstAssignment()
{
}

// @brief 種類を返す．
StmtType
AstAssignment::stmt_type() const
{
  return mStmtType;
}

// @brief 左辺式を返す．
//
// kAssignment のみ有効
const AstLeaf*
AstAssignment::lhs_expr() const
{
  return mLeft;
}

// @brief 式を返す．
//
// kAssignment,
// kDoWhile, kFor, kIf, kWhile, kSwitch
// kExprStmt, kReturn, kVarDecl のみ有効
const AstExpr*
AstAssignment::expr() const
{
  return mRight;
}

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstAssignment::phase1(YmslScope* parent_scope,
		      YmslTypeMgr* type_mgr)
{
  mLeft->resolve_var(parent_scope);
  mRight->resolve_var(parent_scope);
}

// @brief 関数の参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstAssignment::phase2(YmslScope* parent_scope)
{
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
#endif

#if 0
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
#endif

END_NAMESPACE_YM_YMSL
