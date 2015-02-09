
/// @file IrPrinter.cc
/// @brief IrPrinter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrPrinter.h"
#include "IrNode.h"
#include "IrHandle.h"
#include "Var.h"
#include "Function.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrPrinter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力先のストリーム
IrPrinter::IrPrinter(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
IrPrinter::~IrPrinter()
{
}

// @brief コードの内容を出力する．
void
IrPrinter::print_code(const vector<IrNode*>& node_list)
{
  ymuint n = node_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    IrNode* node = node_list[i];
    node->set_id(i + 1);
  }
  for (ymuint i = 0; i < n; ++ i) {
    IrNode* node = node_list[i];
    print_node(node);
  }
}

// @brief ノードの内容を出力する．
void
IrPrinter::print_node(IrNode* node)
{
  mS << "%" << node->id() << " = ";
  switch ( node->opcode() ) {
  case kOpTrue:
    mS << "true";
    break;

  case kOpFalse:
    mS << "false";
    break;

  case kOpIntConst:
    mS << "int(" << node->int_val() << ")";
    break;

  case kOpFloatConst:
    mS << "float(" << node->float_val() << ")";
    break;

  case kOpStringConst:
    mS << "string(" << node->string_val() << ")";
    break;

  case kOpCastBoolean:
    mS << "cast_to_boolean %" << node->operand(0)->id();
    break;

  case kOpCastInt:
    mS << "cast_to_int %" << node->operand(0)->id();
    break;

  case kOpCastFloat:
    mS << "cast_to_float %" << node->operand(0)->id();
    break;

  case kOpBitNeg:
    mS << "bit_neg %" << node->operand(0)->id();
    break;

  case kOpLogNot:
    mS << "log_not %" << node->operand(0)->id();
    break;

  case kOpUniMinus:
    mS << "complement %" << node->operand(0)->id();
    break;

  case kOpBitAnd:
    mS << "bit_and %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpBitOr:
    mS << "bit_or %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpBitXor:
    mS << "bit_xor %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpLogAnd:
    mS << "log_and %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpLogOr:
    mS << "log_or %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpAdd:
    mS << "add %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpSub:
    mS << "sub %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpMul:
    mS << "mul %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpDiv:
    mS << "div %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpMod:
    mS << "mod %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpLshift:
    mS << "l_shift %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpRshift:
    mS << "r_shift %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpEqual:
    mS << "equal %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpNotEq:
    mS << "not_equal %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpLt:
    mS << "less_than %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpLe:
    mS << "less_than_or_equal %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpIte:
    mS << "if_then_else %" << node->operand(0)->id() << " %" << node->operand(1)->id()
       << " %" << node->operand(2)->id();
    break;

  case kOpLoad:
    mS << "load ";
    print_handle(node->address());
    break;

  case kOpStore:
    mS << "store ";
    print_handle(node->address());
    mS << " %" << node->store_val()->id();
    break;

  case kOpInc:
    mS << "inc ";
    print_handle(node->address());
    break;

  case kOpDec:
    mS << "dec ";
    print_handle(node->address());
    break;

  case kOpInplaceBitAnd:
    mS << "inplace_bit_and ";
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case kOpInplaceBitOr:
    mS << "inplace_bit_or ";
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case kOpInplaceBitXor:
    mS << "inplace_bit_xor ";
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case kOpInplaceAdd:
    mS << "inplace_add ";
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case kOpInplaceSub:
    mS << "inplace_sub ";
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case kOpInplaceMul:
    mS << "inplace_mul ";
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case kOpInplaceDiv:
    mS << "inplace_div ";
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case kOpInplaceMod:
    mS << "inplace_mod ";
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case kOpInplaceLshift:
    mS << "inplace_lshift ";
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case kOpInplaceRshift:
    mS << "inplace_rshift ";
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case kOpFuncCall:
    mS << "func_call ";
    print_handle(node->func_addr());
    {
      ymuint n = node->arglist_num();
      for (ymuint i = 0; i < n; ++ i) {
	mS << " %" << node->arglist_elem(i)->id();
      }
    }
    break;

  case kOpReturn:
    mS << "return";
    if ( node->return_val() ) {
      mS << " %" << node->return_val()->id();
    }
    break;

  case kOpJump:
    mS << "jump %" << node->jump_addr()->id();
    break;

  case kOpBranchTrue:
    mS << "branch_true %" << node->jump_addr()->id()
       << " %" << node->branch_cond()->id();
    break;

  case kOpBranchFalse:
    mS << "branch_false %" << node->jump_addr()->id()
       << " %" << node->branch_cond()->id();
    break;

  case kOpLabel:
    mS << "label";
    break;

  case kOpHalt:
    mS << "halt";
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
  mS << endl;
}

// @brief ハンドルの内容を出力する．
void
IrPrinter::print_handle(IrHandle* handle)
{
  switch ( handle->handle_type() ) {
  case IrHandle::kScope:
    mS << "scope[" << handle->name() << "]";
    break;

  case IrHandle::kVar:
    mS << "var[" << handle->name() << "]";
    break;

  case IrHandle::kFunction:
    mS << "function[" << handle->name() << "]";
    break;

  case IrHandle::kConstant:
    mS << "constant[]";
    break;

  case IrHandle::kLabel:
    mS << "label[" << handle->name() << "]";
    break;

  case IrHandle::kNamedType:
    mS << "named_type[" << handle->name() << "]";
    break;

  case IrHandle::kArrayRef:
    mS << "array_ref(%" << handle->array_expr()->id() << ", %" << handle->array_index()->id() << ")";
    break;

  case IrHandle::kMemberRef:
    mS << "member_ref(%" << handle->obj_expr()->id() << ", var[" << handle->var()->name() << "]";
    break;

  case IrHandle::kMethodRef:
    mS << "method_ref(%" << handle->obj_expr()->id() << ", function[" << handle->function()->name() << "]";
    break;
  }
}

END_NAMESPACE_YM_YMSL
