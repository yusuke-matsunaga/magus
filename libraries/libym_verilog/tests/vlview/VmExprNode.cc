
/// @file libym_verilog/tests/vlview/VmExprNode.cc
/// @brief VmExprNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmExprNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmExprNode.h"
#include "ym_verilog/vl/VlExpr.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlTaskFunc.h"
#include "ym_verilog/vl/VlUserSystf.h"
#include "ym_verilog/vl/VlPrimitive.h"


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
      return mLabel;
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
      switch ( mExpr->type() ) {
      case kVpiBitSelect:   return "BitSelect";
      case kVpiPartSelect:  return "PartSelect";
      case kVpiOperation:   return "Operation";
      case kVpiConstant:    return "Constant";
      case kVpiFuncCall:    return "FuncCall";
      case kVpiSysFuncCall: return "SysFuncCall";
      default:              return "Reference";
      }
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

// 演算子の型を表す文字列を返す．
QString
op_type_str(tVpiOpType type)
{
  switch ( type ) {
  case kVpiMinusOp:       return "minus";
  case kVpiPlusOp:        return "plus";
  case kVpiNotOp:         return "not";
  case kVpiBitNegOp:      return "bitneg";
  case kVpiUnaryAndOp:    return "unary and";
  case kVpiUnaryNandOp:   return "unary nand";
  case kVpiUnaryOrOp:     return "unary or";
  case kVpiUnaryNorOp:    return "unary nor";
  case kVpiUnaryXorOp:    return "unary xor";
  case kVpiUnaryXNorOp:   return "unary xnor";
  case kVpiSubOp:         return "sub";
  case kVpiDivOp:         return "div";
  case kVpiModOp:         return "mod";
  case kVpiEqOp:          return "equal";
  case kVpiNeqOp:         return "not equal";
  case kVpiCaseEqOp:      return "case equal";
  case kVpiCaseNeqOp:     return "case not equal";
  case kVpiGtOp:          return "greater than";
  case kVpiGeOp:          return "greater than or equal";
  case kVpiLtOp:          return "less than";
  case kVpiLeOp:          return "leess than or equal";
  case kVpiLShiftOp:      return "left shift";
  case kVpiRShiftOp:      return "right shift";
  case kVpiAddOp:         return "add";
  case kVpiMultOp:        return "mult";
  case kVpiLogAndOp:      return "logical and";
  case kVpiLogOrOp:       return "logical or";
  case kVpiBitAndOp:      return "bitwise and";
  case kVpiBitOrOp:       return "bitwise or";
  case kVpiBitXorOp:      return "bitwise xor";
  case kVpiBitXNorOp:     return "bitwise xnor";
  case kVpiConditionOp:   return "conditional";
  case kVpiConcatOp:      return "concat";
  case kVpiMultiConcatOp: return "multi concat";
  case kVpiEventOrOp:     return "event or";
  case kVpiNullOp:        return "null";
  case kVpiListOp:        return "list";
  case kVpiMinTypMaxOp:   return "min-typ-max";
  case kVpiPosedgeOp:     return "positive edge";
  case kVpiNegedgeOp:     return "negative edge";
  case kVpiArithLShiftOp: return "arithmetic left shift";
  case kVpiArithRShiftOp: return "arithmetic right shift";
  case kVpiPowerOp:       return "power";
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

END_NONAMESPACE


// @brief 子供の配列を作る．
void
VmExprNode::expand() const
{
  add_child("vpiDecompile", mExpr->decompile());
  switch ( mExpr->type() ) {
  case kVpiBitSelect:
    add_child("vpiConstantSelect", mExpr->is_constant_select());
    if ( mExpr->decl_obj() ) {
      add_child("vpiParent", mExpr->decl_obj()->full_name());
      for (ymuint i = 0; i < mExpr->declarray_dimension(); ++ i) {
	add_child("array_index", mExpr->declarray_index(i));
      }
      add_child("vpiIndex", mExpr->index());
    }
    else {
      add_child("vpiParent", mExpr->parent_expr());
      add_child("vpiIndex", mExpr->index_val());
    }
    break;

  case kVpiPartSelect:
    add_child("vpiConstantSelect", mExpr->is_constant_select());
    if ( mExpr->decl_obj() ) {
      add_child("vpiParent", mExpr->decl_obj()->full_name());
      for (ymuint i = 0; i < mExpr->declarray_dimension(); ++ i) {
	add_child("array_index", mExpr->declarray_index(i));
      }
      add_child("vpiLeftRange", mExpr->left_range());
      add_child("vpiRightRange", mExpr->right_range());
    }
    else {
      add_child("vpiParent", mExpr->parent_expr());
      add_child("vpiLeftRange", mExpr->left_range_val());
      add_child("vpiRightRange", mExpr->right_range_val());
    }
    break;
    
  case kVpiOperation:
    add_child("vpiOpType", op_type_str(mExpr->op_type()));
    for (ymuint i = 0; i < mExpr->operand_num(); ++ i) {
      add_child("Operand", mExpr->operand(i));
    }
    break;

  case kVpiConstant:
    add_child("vpiConstType", constant_type_str(mExpr->constant_type()));
    break;

  case kVpiFuncCall:
    add_child("vpiFunction", mExpr->function()->full_name());
    for (ymuint i = 0; i < mExpr->argument_num(); ++ i) {
      add_child("vpiArgument", mExpr->argument(i));
    }
    break;
    
  case kVpiSysFuncCall:
    add_child("vpiUserSystf", mExpr->user_systf()->name());
    for (ymuint i = 0; i < mExpr->argument_num(); ++ i) {
      add_child("vpiArgument", mExpr->argument(i));
    }
    break;

  default:
    if ( mExpr->decl_obj() ) {
      add_child("decl obj", mExpr->decl_obj()->full_name());
      for (ymuint i = 0; i < mExpr->declarray_dimension(); ++ i) {
	add_child("vpiArrayIndex", mExpr->declarray_index(i));
      }
    }
    else if ( mExpr->scope_obj() ) {
      add_child("scope obj", mExpr->scope_obj()->full_name());
    }
    else if ( mExpr->primitive_obj() ) {
      add_child("primitive obj", mExpr->primitive_obj()->full_name());
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    break;
  }
}

END_NAMESPACE_YM_VERILOG
