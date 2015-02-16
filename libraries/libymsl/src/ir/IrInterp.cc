
/// @file IrInterp.cc
/// @brief IrInterp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrInterp.h"
#include "IrNode.h"
#include "IrHandle.h"
#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrInterp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrInterp::IrInterp()
{
}

// @brief デストラクタ
IrInterp::~IrInterp()
{
}

// @brief boolean 型の式の評価を行う．
// @param[in] node 式を表すノード
bool
IrInterp::eval_boolean(IrNode* node)
{
  switch ( node->node_type() ) {
  case IrNode::kUniOp:
    switch ( node->opcode() ) {
    case kOpCastBoolean:
      switch ( node->operand(0)->value_type()->type_id() ) {
      case kBooleanType:
	return eval_boolean(node->operand(0));

      case kIntType:
	return eval_int(node->operand(0)) != 0;

      case kFloatType:
	return eval_float(node->operand(0)) != 0.0;

      default:
	ASSERT_NOT_REACHED;
      }
      break;

    case kOpLogNot:
      return !eval_boolean(node->operand(0));

    case kOpCastInt:
    case kOpCastFloat:
    case kOpBitNeg:
    case kOpUniMinus:
    default:
      ASSERT_NOT_REACHED;
    }
    break;

  case IrNode::kBinOp:
    switch ( node->opcode() ) {
    case kOpLogAnd:
      return eval_boolean(node->operand(0)) && eval_boolean(node->operand(1));

    case kOpLogOr:
      return eval_boolean(node->operand(0)) || eval_boolean(node->operand(1));

    case kOpEqual:
      switch ( node->operand(0)->value_type()->type_id() ) {
      case kBooleanType:
	return eval_boolean(node->operand(0)) == eval_boolean(node->operand(1));

      case kIntType:
	return eval_int(node->operand(0)) == eval_int(node->operand(1));

      case kFloatType:
	return eval_float(node->operand(0)) == eval_float(node->operand(1));

      default:
	ASSERT_NOT_REACHED;
      }
      break;

    case kOpNotEq:
      switch ( node->operand(0)->value_type()->type_id() ) {
      case kBooleanType:
	return eval_boolean(node->operand(0)) != eval_boolean(node->operand(1));

      case kIntType:
	return eval_int(node->operand(0)) != eval_int(node->operand(1));

      case kFloatType:
	return eval_float(node->operand(0)) != eval_float(node->operand(1));

      default:
	ASSERT_NOT_REACHED;
      }
      break;

    case kOpLt:
      switch ( node->operand(0)->value_type()->type_id() ) {
      case kIntType:
	return eval_int(node->operand(0)) < eval_int(node->operand(1));

      case kFloatType:
	return eval_float(node->operand(0)) < eval_float(node->operand(1));

      default:
	ASSERT_NOT_REACHED;
      }
      break;

    case kOpLe:
      switch ( node->operand(0)->value_type()->type_id() ) {
      case kIntType:
	return eval_int(node->operand(0)) <= eval_int(node->operand(1));

      case kFloatType:
	return eval_float(node->operand(0)) <= eval_float(node->operand(1));

      default:
	ASSERT_NOT_REACHED;
      }
      break;

    case kOpBitAnd:
    case kOpBitOr:
    case kOpBitXor:
    case kOpAdd:
    case kOpSub:
    case kOpMul:
    case kOpDiv:
    case kOpMod:
    case kOpLshift:
    case kOpRshift:
    default:
      ASSERT_NOT_REACHED;
    }
    break;

  case IrNode::kTriOp:
    switch( node->opcode() ) {
    case kOpIte:
      return eval_boolean(node->operand(0)) ? eval_boolean(node->operand(1)) : eval_boolean(node->operand(2));

    default:
      ASSERT_NOT_REACHED;
    }
    break;

  case IrNode::kLoad:
    {
      IrHandle* handle = node->address();
      switch ( handle->handle_type() ) {
      case IrHandle::kBooleanConst:
	return handle->boolean_val();

      case IrHandle::kIntConst:
      case IrHandle::kFloatConst:
      case IrHandle::kStringConst:
      case IrHandle::kScope:
      case IrHandle::kVar:
      case IrHandle::kFunction:
      case IrHandle::kLabel:
      case IrHandle::kNamedType:
      case IrHandle::kArrayRef:
      case IrHandle::kMemberRef:
      case IrHandle::kMethodRef:
	ASSERT_NOT_REACHED;
      }
    }
    break;

  case IrNode::kStore:
  case IrNode::kInplaceUniOp:
  case IrNode::kInplaceBinOp:
  case IrNode::kFuncCall:
  case IrNode::kReturn:
  case IrNode::kJump:
  case IrNode::kBranchTrue:
  case IrNode::kBranchFalse:
  case IrNode::kLabel:
  case IrNode::kHalt:
    ASSERT_NOT_REACHED;
  }

  // ダミー
  return false;
}

// @brief int 型の式の評価を行う．
// @param[in] node 式を表すノード
Ymsl_INT
IrInterp::eval_int(IrNode* node)
{
  switch ( node->node_type() ) {
  case IrNode::kUniOp:
    switch ( node->opcode() ) {
    case kOpCastInt:
      switch ( node->operand(0)->value_type()->type_id() ) {
      case kBooleanType:
	return eval_boolean(node->operand(0)) ? 1 : 0;

      case kIntType:
	return eval_int(node->operand(0));

      default:
	ASSERT_NOT_REACHED;
      }
      break;


    case kOpBitNeg:
      return ~eval_int(node->operand(0));

    case kOpUniMinus:
      return -eval_int(node->operand(0));

    case kOpCastBoolean:
    case kOpCastFloat:
    case kOpLogNot:
    default:
      ASSERT_NOT_REACHED;
    }
    break;

  case IrNode::kBinOp:
    switch ( node->opcode() ) {
    case kOpBitAnd:
      return eval_int(node->operand(0)) & eval_int(node->operand(1));

    case kOpBitOr:
      return eval_int(node->operand(0)) | eval_int(node->operand(1));

    case kOpBitXor:
      return eval_int(node->operand(0)) ^ eval_int(node->operand(1));

    case kOpAdd:
      return eval_int(node->operand(0)) + eval_int(node->operand(1));

    case kOpSub:
      return eval_int(node->operand(0)) - eval_int(node->operand(1));

    case kOpMul:
      return eval_int(node->operand(0)) * eval_int(node->operand(1));

    case kOpDiv:
      return eval_int(node->operand(0)) / eval_int(node->operand(1));

    case kOpMod:
      return eval_int(node->operand(0)) % eval_int(node->operand(1));

    case kOpLshift:
      return eval_int(node->operand(0)) << eval_int(node->operand(1));

    case kOpRshift:
      return eval_int(node->operand(0)) >> eval_int(node->operand(1));

    case kOpLogAnd:
    case kOpLogOr:
    case kOpEqual:
    case kOpNotEq:
    case kOpLt:
    case kOpLe:
    default:
      ASSERT_NOT_REACHED;
    }
    break;

  case IrNode::kTriOp:
    switch( node->opcode() ) {
    case kOpIte:
      return eval_boolean(node->operand(0)) ? eval_int(node->operand(1)) : eval_int(node->operand(2));

    default:
      ASSERT_NOT_REACHED;
    }

  case IrNode::kLoad:
    {
      IrHandle* handle = node->address();
      switch ( handle->handle_type() ) {
      case IrHandle::kIntConst:
	return handle->int_val();

      case IrHandle::kBooleanConst:
      case IrHandle::kFloatConst:
      case IrHandle::kStringConst:
      case IrHandle::kScope:
      case IrHandle::kVar:
      case IrHandle::kFunction:
      case IrHandle::kLabel:
      case IrHandle::kNamedType:
      case IrHandle::kArrayRef:
      case IrHandle::kMemberRef:
      case IrHandle::kMethodRef:
	ASSERT_NOT_REACHED;
      }
      break;
    }
    break;

  case IrNode::kStore:
  case IrNode::kInplaceUniOp:
  case IrNode::kInplaceBinOp:
  case IrNode::kFuncCall:
  case IrNode::kReturn:
  case IrNode::kJump:
  case IrNode::kBranchTrue:
  case IrNode::kBranchFalse:
  case IrNode::kLabel:
  case IrNode::kHalt:
    ASSERT_NOT_REACHED;
  }

  // ダミー
  return 0;
}

// @brief float 型の式の評価を行う．
// @param[in] node 式を表すノード
Ymsl_FLOAT
IrInterp::eval_float(IrNode* node)
{
  switch ( node->node_type() ) {
  case IrNode::kUniOp:
    switch ( node->opcode() ) {
    case kOpCastFloat:
      switch ( node->operand(0)->value_type()->type_id() ) {
      case kBooleanType:
	return eval_boolean(node->operand(0)) ? 1.0 : 0.0;

      case kIntType:
	return static_cast<Ymsl_FLOAT>(eval_int(node->operand(0)));

      case kFloatType:
	return eval_float(node->operand(0));

      default:
	ASSERT_NOT_REACHED;
      }
      break;

    case kOpUniMinus:
      return - eval_float(node->operand(0));

    case kOpCastBoolean:
    case kOpCastInt:
    case kOpBitNeg:
    case kOpLogNot:
    default:
      ASSERT_NOT_REACHED;
    }
    break;

  case IrNode::kBinOp:
    switch ( node->opcode() ) {
    case kOpAdd:
      return eval_float(node->operand(0)) + eval_float(node->operand(1));

    case kOpSub:
      return eval_float(node->operand(0)) - eval_float(node->operand(1));

    case kOpMul:
      return eval_float(node->operand(0)) * eval_float(node->operand(1));

    case kOpDiv:
      return eval_float(node->operand(0)) / eval_float(node->operand(1));

    case kOpBitAnd:
    case kOpBitOr:
    case kOpBitXor:
    case kOpLogAnd:
    case kOpLogOr:
    case kOpMod:
    case kOpLshift:
    case kOpRshift:
    case kOpEqual:
    case kOpNotEq:
    case kOpLt:
    case kOpLe:
    default:
      ASSERT_NOT_REACHED;
    }
    break;

  case IrNode::kTriOp:
    switch( node->opcode() ) {
    case kOpIte:
      return eval_boolean(node->operand(0)) ? eval_float(node->operand(1)) : eval_float(node->operand(2));

    default:
      ASSERT_NOT_REACHED;
    }

  case IrNode::kLoad:
    {
      IrHandle* handle = node->address();
      switch ( handle->handle_type() ) {
      case IrHandle::kFloatConst:
	return handle->float_val();

      case IrHandle::kBooleanConst:
      case IrHandle::kIntConst:
      case IrHandle::kStringConst:
      case IrHandle::kScope:
      case IrHandle::kVar:
      case IrHandle::kFunction:
      case IrHandle::kLabel:
      case IrHandle::kNamedType:
      case IrHandle::kArrayRef:
      case IrHandle::kMemberRef:
      case IrHandle::kMethodRef:
	ASSERT_NOT_REACHED;
      }
      break;
    }
    break;

  case IrNode::kStore:
  case IrNode::kInplaceUniOp:
  case IrNode::kInplaceBinOp:
  case IrNode::kFuncCall:
  case IrNode::kReturn:
  case IrNode::kJump:
  case IrNode::kBranchTrue:
  case IrNode::kBranchFalse:
  case IrNode::kLabel:
  case IrNode::kHalt:
    ASSERT_NOT_REACHED;
  }

  // ダミー
  return 0.0;
}

END_NAMESPACE_YM_YMSL
