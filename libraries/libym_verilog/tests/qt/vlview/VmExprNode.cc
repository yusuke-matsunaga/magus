
/// @file VmExprNode.cc
/// @brief VmExprNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VmExprNode.h"
#include "verilog/VlValue.h"
#include "verilog/vl/VlExpr.h"
#include "verilog/vl/VlDecl.h"
#include "verilog/vl/VlTaskFunc.h"
#include "verilog/vl/VlUserSystf.h"
#include "verilog/vl/VlPrimitive.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmNode の関数
//////////////////////////////////////////////////////////////////////

// @brief ExprListNode を追加する．
// @param[in] label ラベル
// @param[in] expr_list 式の配列
void
VmNode::add_exprlist(const QString& label,
		     const vector<const VlExpr*>& expr_list) const
{
  add_child( new VmExprListNode(label, expr_list) );
}

// @brief ExprNode を追加する．
// @param[in] expr 式
void
VmNode::add_expr(const QString& label,
		 const VlExpr* expr) const
{
  add_child( new VmExprNode(label, expr) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmExprListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] expr_array 式の配列
VmExprListNode::VmExprListNode(const QString& label,
			       const vector<const VlExpr*>& expr_array) :
  mLabel(label),
  mExprArray(expr_array)
{
}

// @brief デストラクタ
VmExprListNode::~VmExprListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmExprListNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel + " list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmExprListNode::loc() const
{
  if ( mExprArray.empty() ) {
    return FileRegion();
  }
  else {
    FileRegion first = mExprArray.front()->file_region();
    FileRegion last = mExprArray.back()->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmExprListNode::expand() const
{
  for (vector<const VlExpr*>::const_iterator p = mExprArray.begin();
       p != mExprArray.end(); ++ p) {
    add_expr(mLabel, *p);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmExprNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] expr 式
VmExprNode::VmExprNode(const QString& label,
		       const VlExpr* expr) :
  mLabel(label),
  mExpr(expr)
{
}

// @brief デストラクタ
VmExprNode::~VmExprNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmExprNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return QString(mExpr->decompile().c_str());
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmExprNode::loc() const
{
  return mExpr->file_region();
}

BEGIN_NONAMESPACE

// 式の型を表す文字列を返す．
QString
type2str(tVpiObjType type)
{
  switch ( type ) {
  case kVpiBitSelect:   return "vpiBitSelect";
  case kVpiPartSelect:  return "vpiPartSelect";
  case kVpiOperation:   return "vpiOperation";
  case kVpiConstant:    return "vpiConstant";
  case kVpiFuncCall:    return "vpiFuncCall";
  case kVpiSysFuncCall: return "vpiSysFuncCall";
  case kVpiNetBit:      return "vpiNetBit";
  case kVpiRegBit:      return "vpiRegBit";
  default:              return "Reference";
  }
  assert_not_reached(__FILE__, __LINE__);
  return "";
}

// 式の値の型を表す文字列を返す．
QString
value_type2str(const VlValueType& value_type)
{
  if ( value_type.is_int_type() ) {
    return "integer";
  }
  if ( value_type.is_real_type() ) {
    return "real";
  }
  if ( value_type.is_time_type() ) {
    return "time";
  }
  ymuint s = value_type.size();
  ostringstream buf;
  if ( value_type.is_signed() ) {
    buf << "signed ";
  }
  else {
    buf << "unsigned ";
  }
  if ( value_type.is_sized() ) {
    buf << "sized ";
  }
  else {
    buf << "unsized ";
  }
  if ( s > 0 ) {
    buf << s << " bit ";
  }
  buf << "bitvector";
  return buf.str().c_str();
}

// 演算子の型を表す文字列を返す．
QString
op_type_str(tVlOpType type)
{
  switch ( type ) {
  case kVlMinusOp:       return "minus";
  case kVlPlusOp:        return "plus";
  case kVlNotOp:         return "not";
  case kVlBitNegOp:      return "bitneg";
  case kVlUnaryAndOp:    return "unary and";
  case kVlUnaryNandOp:   return "unary nand";
  case kVlUnaryOrOp:     return "unary or";
  case kVlUnaryNorOp:    return "unary nor";
  case kVlUnaryXorOp:    return "unary xor";
  case kVlUnaryXNorOp:   return "unary xnor";
  case kVlSubOp:         return "sub";
  case kVlDivOp:         return "div";
  case kVlModOp:         return "mod";
  case kVlEqOp:          return "equal";
  case kVlNeqOp:         return "not equal";
  case kVlCaseEqOp:      return "case equal";
  case kVlCaseNeqOp:     return "case not equal";
  case kVlGtOp:          return "greater than";
  case kVlGeOp:          return "greater than or equal";
  case kVlLtOp:          return "less than";
  case kVlLeOp:          return "leess than or equal";
  case kVlLShiftOp:      return "left shift";
  case kVlRShiftOp:      return "right shift";
  case kVlAddOp:         return "add";
  case kVlMultOp:        return "mult";
  case kVlLogAndOp:      return "logical and";
  case kVlLogOrOp:       return "logical or";
  case kVlBitAndOp:      return "bitwise and";
  case kVlBitOrOp:       return "bitwise or";
  case kVlBitXorOp:      return "bitwise xor";
  case kVlBitXNorOp:     return "bitwise xnor";
  case kVlConditionOp:   return "conditional";
  case kVlConcatOp:      return "concat";
  case kVlMultiConcatOp: return "multi concat";
  case kVlEventOrOp:     return "event or";
  case kVlNullOp:        return "null";
  case kVlListOp:        return "list";
  case kVlMinTypMaxOp:   return "min-typ-max";
  case kVlPosedgeOp:     return "positive edge";
  case kVlNegedgeOp:     return "negative edge";
  case kVlArithLShiftOp: return "arithmetic left shift";
  case kVlArithRShiftOp: return "arithmetic right shift";
  case kVlPowerOp:       return "power";
  default: assert_not_reached(__FILE__, __LINE__);
  }
  return "";
}

// 定数の型を表す文字列を返す．
QString
constant_type_str(tVpiConstType type)
{
  switch ( type ) {
  case kVpiDecConst:          return "vpiDecConst";
  case kVpiRealConst:         return "vpiRealConst";
  case kVpiBinaryConst:       return "vpiBinaryConst";
  case kVpiOctConst:          return "vpiOctConst";
  case kVpiHexConst:          return "vpiHexConst";
  case kVpiStringConst:       return "vpiStringConst";
  case kVpiIntConst:          return "vpiIntConst";
  case kVpiSignedDecConst:    return "vpiSignedDecConst";
  case kVpiSignedBinaryConst: return "vpiSignedBinaryConst";
  case kVpiSignedOctConst:    return "vpiSignedOctConst";
  case kVpiSignedHexConst:    return "vpiSignedHexConst";
  default: assert_not_reached(__FILE__, __LINE__);
  }
  return "";
}

// 値を表す文字列を返す．
QString
value2str(VlValue value)
{
  ostringstream buf;
  switch ( value.type() ) {
  case VlValue::kIntType:
    buf << value.int_value();
    break;

  case VlValue::kUintType:
    buf << value.uint_value();
    break;

  case VlValue::kScalarType:
    buf << value.scalar_value();
    break;

  case VlValue::kRealType:
    buf << value.real_value();
    break;

  case VlValue::kTimeType:
    buf << value.time_value().value();
    break;

  case VlValue::kBitVectorType:
    buf << value.bitvector_value();
    break;

  case VlValue::kErrorType:
    buf << "error";
    break;
  }
  return buf.str().c_str();
}

END_NONAMESPACE


// @brief 子供の配列を作る．
void
VmExprNode::expand() const
{
  add_str("vpiType", type2str(mExpr->type()));
  add_str("vpiValueType", value_type2str(mExpr->value_type()));
  switch ( mExpr->type() ) {
  case kVpiOperation:
    add_str("vpiOpType", op_type_str(mExpr->op_type()));
    for (ymuint i = 0; i < mExpr->operand_num(); ++ i) {
      add_expr("Operand", mExpr->operand(i));
    }
    break;

  case kVpiConstant:
    add_str("vpiConstType", constant_type_str(mExpr->constant_type()));
    add_str("vpiConstValue", value2str(mExpr->constant_value()));
    break;

  case kVpiFuncCall:
    add_str("vpiFunction", mExpr->function()->full_name());
    for (ymuint i = 0; i < mExpr->argument_num(); ++ i) {
      add_expr("vpiArgument", mExpr->argument(i));
    }
    break;

  case kVpiSysFuncCall:
    add_str("vpiUserSystf", mExpr->user_systf()->name());
    for (ymuint i = 0; i < mExpr->argument_num(); ++ i) {
      add_expr("vpiArgument", mExpr->argument(i));
    }
    break;

  default:
    if ( mExpr->is_bitselect() ) {
      add_bool("vpiConstantSelect", mExpr->is_constant_select());
      if ( mExpr->decl_obj() ) {
	add_str("vpiParent", mExpr->decl_obj()->full_name());
	for (ymuint i = 0; i < mExpr->declarray_dimension(); ++ i) {
	  add_expr("array_index", mExpr->declarray_index(i));
	}
	add_expr("vpiIndex", mExpr->index());
      }
      else {
	add_str("vpiParent", mExpr->parent_expr()->decompile());
	add_int("vpiIndex", mExpr->index_val());
      }
    }
    else if ( mExpr->is_partselect() ) {
      add_bool("vpiConstantSelect", mExpr->is_constant_select());
      if ( mExpr->decl_obj() ) {
	add_str("vpiParent", mExpr->decl_obj()->full_name());
	for (ymuint i = 0; i < mExpr->declarray_dimension(); ++ i) {
	  add_expr("array_index", mExpr->declarray_index(i));
	}
	add_expr("vpiLeftRange", mExpr->left_range());
	add_expr("vpiRightRange", mExpr->right_range());
      }
      else {
	add_str("vpiParent", mExpr->parent_expr()->decompile());
	add_int("vpiLeftRange", mExpr->left_range_val());
	add_int("vpiRightRange", mExpr->right_range_val());
      }
    }
    else if ( mExpr->is_primary() ) {
      if ( mExpr->decl_obj() ) {
	add_str("decl obj", mExpr->decl_obj()->full_name());
	for (ymuint i = 0; i < mExpr->declarray_dimension(); ++ i) {
	  add_expr("vpiArrayIndex", mExpr->declarray_index(i));
	}
      }
      else if ( mExpr->scope_obj() ) {
	add_str("scope obj", mExpr->scope_obj()->full_name());
      }
      else if ( mExpr->primitive_obj() ) {
	add_str("primitive obj", mExpr->primitive_obj()->full_name());
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    break;
  }
}

END_NAMESPACE_YM_VERILOG
