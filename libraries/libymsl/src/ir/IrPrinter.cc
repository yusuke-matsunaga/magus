
/// @file IrPrinter.cc
/// @brief IrPrinter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrPrinter.h"
#include "IrNode.h"
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

  case kOpPlus:
    mS << "add %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpMinus:
    mS << "sub %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case kOpMult:
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
    mS << "load " << node->var()->name();
    break;

  case kOpStore:
    mS << "store " << node->var()->name() << " %" << node->store_val()->id();
    break;

  case kOpArrayLoad:
    mS << "load_array %" << node->array_expr()->id() << " %" << node->array_index()->id();
    break;

  case kOpArrayStore:
    mS << "store_array %" << node->array_expr()->id() << " %" << node->array_index()->id()
       << " %" << node->store_val()->id();
    break;

  case kOpMemberLoad:
    mS << "load_member %" << node->obj_expr()->id() << " " << node->var()->name();
    break;

  case kOpMemberStore:
    mS << "store_member %" << node->obj_expr()->id() << " " << node->var()->name()
       << " %" << node->store_val()->id();
    break;

  case kOpFuncCall:
    mS << "func_call " << node->function()->name();
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

END_NAMESPACE_YM_YMSL
