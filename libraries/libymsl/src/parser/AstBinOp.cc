
/// @file AstBinOp.cc
/// @brief AstBinOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBinOp.h"

#include "YmslCodeList.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstBinOp
//////////////////////////////////////////////////////////////////////

// @breif コンストラクタ
// @param[in] op 演算子のタイプ
// @param[in] left, right オペランド
AstBinOp::AstBinOp(ExprType op,
		   AstExpr* left,
		   AstExpr* right) :
  AstExpr(FileRegion(left->file_region(), right->file_region())),
  mOp(op),
  mLeft(left),
  mRight(right)
{
}

// @brief デストラクタ
AstBinOp::~AstBinOp()
{
}

// @brief 種類を返す．
ExprType
AstBinOp::expr_type() const
{
  return mOp;
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
AstBinOp::operand_num() const
{
  return 2;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num()
//
// 演算子のみ有効
const AstExpr*
AstBinOp::operand(ymuint pos) const
{
  switch ( pos ) {
  case 0: return mLeft;
  case 1: return mRight;
  default: ASSERT_NOT_REACHED; break;
  }
  return NULL;
}

#if 0
// @brief 変数の参照を解決する．
void
AstBinOp::resolve_var(YmslScope* parent_scope)
{
  mLeft->resolve_var(parent_scope);
  mRight->resolve_var(parent_scope);
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstBinOp::type_analysis()
{
}

// @brief 式の型を返す．
const YmslType*
AstBinOp::type()
{
  return mType;
}

// @brief 命令コードのサイズを計算する．
ymuint
AstBinOp::calc_size()
{
  return mLeft->calc_size() + mRight->calc_size() + 1;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstBinOp::compile(YmslDriver& driver,
		  YmslCodeList& code_list,
		  Ymsl_INT& addr)
{
#if 0
  mLeft->compile(driver, code_list, addr);
  mRight->compile(driver, code_list, addr);

  Ymsl_CODE op = YMVM_NOP;
  switch ( mType ) {
  case kVoidType:
    break;

  case kBooleanType:
    switch ( mToken) {
    case LOGAND: op = YMVM_INT_AND; break;
    case LOGOR:  op = YMVM_INT_OR; break;
    case EQEQ:   op = YMVM_INT_EQ; break;
    case NOTEQ:  op = YMVM_INT_NE; break;
    default: ASSERT_NOT_REACHED;
    }
    break;

  case kIntType:
    switch ( mToken ) {
    case PLUS:   op = YMVM_INT_ADD; break;
    case MINUS:  op = YMVM_INT_SUB; break;
    case MULT:   op = YMVM_INT_MUL; break;
    case DIV:    op = YMVM_INT_DIV; break;
    case MOD:    op = YMVM_INT_MOD; break;
    case BITAND: op = YMVM_INT_AND; break;
    case BITOR:  op = YMVM_INT_OR; break;
    case BITXOR: op = YMVM_INT_XOR; break;
    case EQEQ:   op = YMVM_INT_EQ; break;
    case NOTEQ:  op = YMVM_INT_NE; break;
    case LT:     op = YMVM_INT_LT; break;
    case GT:     op = YMVM_INT_GT; break;
    case LE:     op = YMVM_INT_LE; break;
    case GE:     op = YMVM_INT_GE; break;
    default: ASSERT_NOT_REACHED;
    }
    break;

  case kFloatType:
    switch ( mToken) {
    case PLUS:   op = YMVM_FLOAT_ADD; break;
    case MINUS:  op = YMVM_FLOAT_SUB; break;
    case MULT:   op = YMVM_FLOAT_MUL; break;
    case DIV:    op = YMVM_FLOAT_DIV; break;
    case EQEQ:   op = YMVM_FLOAT_EQ; break;
    case NOTEQ:  op = YMVM_FLOAT_NE; break;
    case LT:     op = YMVM_FLOAT_LT; break;
    case GT:     op = YMVM_FLOAT_GE; break;
    case LE:     op = YMVM_FLOAT_LE; break;
    case GE:     op = YMVM_FLOAT_GE; break;
    default: ASSERT_NOT_REACHED;
    }
    break;

  case kStringType:
    switch ( mToken) {
    case PLUS:
    case MULT:
    case EQEQ:
    case NOTEQ:
    case LT:
    case GT:
    case LE:
    case GE:
    default: ASSERT_NOT_REACHED;
    }
    break;

  case kUserType:
    break;
  }
  code_list.write_opcode(addr, op);
#endif
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstBinOp::print(ostream& s) const
{
  s << "(";
  mLeft->print(s);
  switch ( mOp) {
  case kPlus:   s << "+"; break;
  case kMinus:  s << "-"; break;
  case kMult:   s << "*"; break;
  case kDiv:    s << "/"; break;
  case kMod:    s << "%"; break;
  case kBitAnd: s << "&"; break;
  case kBitOr:  s << "|"; break;
  case kBitXor: s << "^"; break;
  case kLogAnd: s << " and "; break;
  case kLogOr:  s << " or "; break;
  case kEqual:  s << "=="; break;
  case kNotEq:  s << "!="; break;
  case kLt:     s << "<"; break;
  case kGt:     s << ">"; break;
  case kLe:     s << "<="; break;
  case kGe:     s << ">="; break;
  case kLshift: s << "<<"; break;
  case kRshift: s << ">>"; break;
  default: ASSERT_NOT_REACHED;
  }
  mRight->print(s);
  s << ")";
}
#endif

END_NAMESPACE_YM_YMSL
