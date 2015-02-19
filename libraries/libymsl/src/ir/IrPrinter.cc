
/// @file IrPrinter.cc
/// @brief IrPrinter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrPrinter.h"
#include "IrToplevel.h"
#include "IrFuncBlock.h"
#include "IrNode.h"
#include "IrHandle.h"
#include "Var.h"
#include "Function.h"
#include "Type.h"


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
// @param[in] toplevel トップレベルのコードブロック
void
IrPrinter::print_code(const IrToplevel& toplevel)
{
  const vector<const Var*>& global_var_list = toplevel.global_var_list();
  ymuint ng = global_var_list.size();
  for (ymuint i = 0; i < ng; ++ i) {
    const Var* var = global_var_list[i];
    mS << "global #" << var->index() << ": " << var->name()
       << ": ";
    var->value_type()->print(mS);
    mS << endl;
  }

  const vector<const Var*>& var_list = toplevel.var_list();
  ymuint nv = var_list.size();
  for (ymuint i = 0; i < nv; ++ i) {
    const Var* var = var_list[i];
    mS << "var #" << var->index() << ": " << var->name()
       << ": ";
    var->value_type()->print(mS);
    mS << endl;
  }

  const vector<IrNode*>& node_list = toplevel.node_list();
  print_node_list(node_list);
  mS << "--- end of toplevel code ---" << endl;

  const vector<IrFuncBlock*>& func_list = toplevel.func_list();
  ymuint nf = func_list.size();
  for (ymuint i = 0; i < nf; ++ i) {
    IrFuncBlock* ir_func = func_list[i];
    const Function* func = ir_func->function();
    mS << endl;
    mS << "function #" << ir_func->index()
       << ": " << func->name() << endl;
    const vector<IrNode*>& node_list = ir_func->node_list();
    print_node_list(node_list);
  }
}

// @brief ノードリストの内容を出力する．
void
IrPrinter::print_node_list(const vector<IrNode*>& node_list)
{
  ymuint n = node_list.size();
  vector<IrNode*> all_list;
  for (ymuint i = 0; i < n; ++ i) {
    IrNode* node = node_list[i];
    dfs_node(node, all_list);
  }

  ymuint n2 = all_list.size();
  for (ymuint i = 0; i < n2; ++ i) {
    IrNode* node = all_list[i];
    print_node(node);
  }
}

// @brief ノードに番号をつける．
// @param[in] node ノード
// @param[in] node_list ノードリスト
void
IrPrinter::dfs_node(IrNode* node,
		    vector<IrNode*>& node_list)
{
  switch ( node->node_type() ) {
  case IrNode::kUniOp:
    dfs_node(node->operand(0), node_list);
    break;

  case IrNode::kBinOp:
    dfs_node(node->operand(0), node_list);
    dfs_node(node->operand(1), node_list);
    break;

  case IrNode::kTriOp:
    dfs_node(node->operand(0), node_list);
    dfs_node(node->operand(1), node_list);
    dfs_node(node->operand(2), node_list);
    break;

  case IrNode::kLoad:
    dfs_handle(node->address(), node_list);
    break;

  case IrNode::kStore:
    dfs_handle(node->address(), node_list);
    dfs_node(node->store_val(), node_list);
    break;

  case IrNode::kInplaceUniOp:
    dfs_handle(node->address(), node_list);
    break;

  case IrNode::kInplaceBinOp:
    dfs_handle(node->address(), node_list);
    dfs_node(node->operand(0), node_list);
    break;

  case IrNode::kFuncCall:
    dfs_handle(node->func_addr(), node_list);
    {
      ymuint n = node->arglist_num();
      for (ymuint i = 0; i < n; ++ i) {
	dfs_node(node->arglist_elem(i), node_list);
      }
    }
    break;

  case IrNode::kReturn:
    if ( node->return_val() ) {
      dfs_node(node->return_val(), node_list);
    }
    break;

  case IrNode::kJump:
    break;

  case IrNode::kBranchTrue:
  case IrNode::kBranchFalse:
    dfs_node(node->branch_cond(), node_list);
    break;

  default:
    break;
  }
  node->set_id(node_list.size());
  node_list.push_back(node);
}

// @brief ハンドルが指しているノードに番号をつける．
// @param[in] handle ハンドル
// @param[in] node_list ノードリスト
void
IrPrinter::dfs_handle(IrHandle* handle,
		      vector<IrNode*>& node_list)
{
  switch ( handle->handle_type() ) {
  case IrHandle::kArrayRef:
    dfs_node(handle->array_expr(), node_list);
    dfs_node(handle->array_index(), node_list);
    break;

  case IrHandle::kMemberRef:
    dfs_node(handle->obj_expr(), node_list);
    break;

  case IrHandle::kMethodRef:
    dfs_node(handle->obj_expr(), node_list);
    break;

  default:
    break;
  }
}

// @brief ノードの内容を出力する．
void
IrPrinter::print_node(IrNode* node)
{
  mS << "%" << node->id() << " = ";
  switch ( node->node_type() ) {
  case IrNode::kUniOp:
    switch ( node->opcode() ) {
    case kOpCastBoolean:
      mS << "cast_to_boolean";
      break;

    case kOpCastInt:
      mS << "cast_to_int";
      break;

    case kOpCastFloat:
      mS << "cast_to_float";
      break;

    case kOpBitNeg:
      mS << "bit_neg";
      break;

    case kOpLogNot:
      mS << "log_not";
      break;

    case kOpUniMinus:
      mS << "complement";
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    mS << " %" << node->operand(0)->id();
    break;

  case IrNode::kBinOp:
    switch ( node->opcode() ) {
    case kOpBitAnd:
      mS << "bit_and";
      break;

    case kOpBitOr:
      mS << "bit_or";
      break;

    case kOpBitXor:
      mS << "bit_xor";
      break;

    case kOpLogAnd:
      mS << "log_and";
      break;

    case kOpLogOr:
      mS << "log_or";
      break;

    case kOpAdd:
      mS << "add";
      break;

    case kOpSub:
      mS << "sub";
      break;

    case kOpMul:
      mS << "mul";
      break;

    case kOpDiv:
      mS << "div";
      break;

    case kOpMod:
      mS << "mod";
      break;

    case kOpLshift:
      mS << "l_shift";
      break;

    case kOpRshift:
      mS << "r_shift";
      break;

    case kOpEqual:
      mS << "equal";
      break;

    case kOpNotEq:
      mS << "not_equal";
      break;

    case kOpLt:
      mS << "less_than";
      break;

    case kOpLe:
      mS << "less_than_or_equal";
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    mS << " %" << node->operand(0)->id() << " %" << node->operand(1)->id();
    break;

  case IrNode::kTriOp:
    switch ( node->opcode() ) {
    case kOpIte:
      mS << "if_then_else";
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    mS << " %" << node->operand(0)->id()
       << " %" << node->operand(1)->id()
       << " %" << node->operand(2)->id();
    break;

  case IrNode::kLoad:
    mS << "load ";
    print_handle(node->address());
    break;

  case IrNode::kStore:
    mS << "store ";
    print_handle(node->address());
    mS << " %" << node->store_val()->id();
    break;

  case IrNode::kInplaceUniOp:
    switch ( node->opcode() ) {
    case kOpInc:
      mS << "inc ";
      break;

    case kOpDec:
      mS << "dec ";
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    print_handle(node->address());
    break;

  case IrNode::kInplaceBinOp:
    switch ( node->opcode() ) {
    case kOpBitAnd:
      mS << "inplace_bit_and ";
      break;

    case kOpBitOr:
      mS << "inplace_bit_or ";
      break;

    case kOpBitXor:
      mS << "inplace_bit_xor ";
      break;

    case kOpAdd:
      mS << "inplace_add ";
      break;

    case kOpSub:
      mS << "inplace_sub ";
      break;

    case kOpMul:
      mS << "inplace_mul ";
      break;

    case kOpDiv:
      mS << "inplace_div ";
      break;

    case kOpMod:
      mS << "inplace_mod ";
      break;

    case kOpLshift:
      mS << "inplace_lshift ";
      break;

    case kOpRshift:
      mS << "inplace_rshift ";
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    print_handle(node->address());
    mS << " %" << node->operand(0)->id();
    break;

  case IrNode::kFuncCall:
    mS << "func_call ";
    print_handle(node->func_addr());
    {
      ymuint n = node->arglist_num();
      for (ymuint i = 0; i < n; ++ i) {
	mS << " %" << node->arglist_elem(i)->id();
      }
    }
    break;

  case IrNode::kReturn:
    mS << "return";
    if ( node->return_val() ) {
      mS << " %" << node->return_val()->id();
    }
    break;

  case IrNode::kJump:
    mS << "jump %" << node->jump_addr()->id();
    break;

  case IrNode::kBranchTrue:
    mS << "branch_true %" << node->jump_addr()->id()
       << " %" << node->branch_cond()->id();
    break;

  case IrNode::kBranchFalse:
    mS << "branch_false %" << node->jump_addr()->id()
       << " %" << node->branch_cond()->id();
    break;

  case IrNode::kLabel:
    mS << "label";
    break;

  case IrNode::kHalt:
    mS << "halt";
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

  case IrHandle::kBooleanConst:
    mS << "boolean[";
    if ( handle->boolean_val() ) {
      mS << "true";
    }
    else {
      mS << "false";
    }
    mS << "]";
    break;

  case IrHandle::kIntConst:
    mS << "int[" << handle->int_val() << "]";
    break;

  case IrHandle::kFloatConst:
    mS << "float[" << handle->float_val() << "]";
    break;

  case IrHandle::kStringConst:
    mS << "string[" << handle->string_val() << "]";
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
