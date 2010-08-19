
/// @file libym_verilog/tests/vlview/VmExprNode.cc
/// @brief VmNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmExprNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmExprNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/vl/VlExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

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
    add_child("", *p);
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
      case kPtOprExpr:         return "Operation";
      case kPtConstExpr:       return "Constant";
      case kPtFuncCallExpr:    return "Function Call";
      case kPtSysFuncCallExpr: return "System Function Call";
      case kPtPrimaryExpr:     return "Primary";
      }
    }
  }
  return QVariant();
}

BEGIN_NONAMESPACE

QString
int2str(ymuint32 val)
{
  ostringstream buf;
  buf << val;
  return buf.str().c_str();
}

QString
double2str(double val)
{
  ostringstream buf;
  buf << val;
  return buf.str().c_str();
}

END_NONAMESPACE

// @brief 対象のファイル上での位置を返す．
FileRegion
VmExprNode::loc() const
{
  return mExpr->file_region();
}

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
    add_child( new VmOpTypeNode( mExpr->opr_type() ) );
    for (ymuint i = 0; i < mExpr->operand_num(); ++ i) {
      add_child("Operand", mExpr->operand(i));
    }
    break;

  case kVpiConstant:
    add_child( new ConstTypeNode( mExpr->const_type() ) );
    break;

  case kVpiFuncCall:
    add_child("vpiFunction", mExpr->function()->full_name());
    for (ymuint i = 0; i < mExpr->argument_num(); ++ i) {
      add_child("vpiArgument", mExpr->argument(i));
    }
    break;
    
  case kPtSysFuncCallExpr:
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
      add_child("scope obj", mExpr->scop_obj()->full_name());
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
