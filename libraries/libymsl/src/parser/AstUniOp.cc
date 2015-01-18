
/// @file AstUniOp.cc
/// @brief AstUniOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstUniOp.h"

#include "YmslCodeList.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] op 演算子の種類
// @param[in] opr オペランド
// @param[in] loc ファイル位置
AstUniOp::AstUniOp(ExprType op,
		   AstExpr* opr,
		   const FileRegion& loc) :
  AstExpr(FileRegion(loc, opr->file_region())),
  mOp(op),
  mOperand(opr)
{
}

// @brief デストラクタ
AstUniOp::~AstUniOp()
{
}

// @brief 種類を返す．
ExprType
AstUniOp::expr_type() const
{
  return mOp;
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
AstUniOp::operand_num() const
{
  return 1;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num()
//
// 演算子のみ有効
const AstExpr*
AstUniOp::operand(ymuint pos) const
{
  ASSERT_COND( pos <= operand_num() );
  return mOperand;
}

#if 0
// @brief 変数の参照を解決する．
void
AstUniOp::resolve_var(YmslScope* parent_scope)
{
  mOperand->resolve_var(parent_scope);
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstUniOp::type_analysis()
{
}

// @brief 式の型を返す．
const YmslType*
AstUniOp::type()
{
  return mType;
}

// @brief 命令コードのサイズを計算する．
ymuint
AstUniOp::calc_size()
{
  return mOperand->calc_size() + 1;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstUniOp::compile(YmslDriver& driver,
		  YmslCodeList& code_list,
		  Ymsl_INT& addr)
{
#if 0
  mOperand->compile(driver, code_list, addr);

  Ymsl_CODE op = YMVM_NOP;
  switch ( mType ) {
  case kVoidType:
    break;

  case kBooleanType:
    switch( mToken ) {
    case LOGNOT: op = YMVM_INT_NOT; break;
    case BOOLEAN:
      switch ( mOperand->type() ) {
      case kVoidType:
	ASSERT_NOT_REACHED;
	break;

      case kBooleanType:
	// なにもしない．
	break;

      case kIntType:
	op = YMVM_INT_TO_BOOL;
	break;

      case kFloatType:
	op = YMVM_FLOAT_TO_BOOL;
	break;

      default:
	ASSERT_NOT_REACHED;
	break;
      }
      break;

    default: ASSERT_NOT_REACHED;
    }
    break;

  case kIntType:
    switch ( mToken ) {
    case MINUS:  op = YMVM_INT_MINUS; break;
    case BITNEG: op = YMVM_INT_NOT; break;
    case INT:
      switch ( mOperand->type() ) {
      case kVoidType:
	ASSERT_NOT_REACHED;
	break;

      case kBooleanType:
      case kIntType:
	// なにもしない．
	break;

      case kFloatType:
	op = YMVM_FLOAT_TO_INT;
	break;

      default:
	ASSERT_NOT_REACHED;
	break;
      }
      break;

    default: ASSERT_NOT_REACHED;
    }
    break;

  case kFloatType:
    switch ( mToken ) {
    case MINUS:  op = YMVM_FLOAT_MINUS; break;
    case FLOAT:
      switch ( mOperand->type() ) {
      case kVoidType:
	ASSERT_NOT_REACHED;
	break;

      case kBooleanType:
      case kIntType:
	op = YMVM_INT_TO_FLOAT;
	break;

      case kFloatType:
	// なにもしない．
	break;

      default:
	ASSERT_NOT_REACHED;
	break;
      }
      break;

    default: ASSERT_NOT_REACHED;
    }
    break;

  case kStringType:
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
AstUniOp::print(ostream& s) const
{
  switch ( mOp ) {
  case kUniPlus:     s << "+"; break;
  case kUniMinus:    s << "-"; break;
  case kBitNeg:      s << "~"; break;
  case kLogNot:      s << "!"; break;
  case kCastInt:     s << "int("; break;
  case kCastBoolean: s << "boolean("; break;
  case kCastFloat:   s << "float("; break;
  default: ASSERT_NOT_REACHED;
  }

  mOperand->print(s);

  switch ( mOp ) {
  case kCastInt:
  case kCastBoolean:
  case kCastFloat:
    s << ")"; break;
  default: break;
  }
}
#endif

END_NAMESPACE_YM_YMSL
