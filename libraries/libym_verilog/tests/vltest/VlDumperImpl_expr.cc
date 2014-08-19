
/// @file VlDumperImpl_expr.cc
/// @brief VlDumperImpl の実装ファイル (expr担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlDumperImpl.h"

#include "YmVerilog/VlMgr.h"
#include "YmVerilog/vl/VlRange.h"
#include "YmVerilog/vl/VlDelay.h"
#include "YmVerilog/vl/VlExpr.h"
#include "YmVerilog/vl/VlTaskFunc.h"
#include "YmVerilog/vl/VlUserSystf.h"
#include "YmVerilog/vl/VlDecl.h"
#include "YmVerilog/vl/VlDeclArray.h"
#include "YmVerilog/vl/VlPrimitive.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 式の内容の出力
//////////////////////////////////////////////////////////////////////

// @brief 式の内容を出力する関数
void
VlDumperImpl::put_expr(const char* label,
		       const VlMgr& mgr,
		       const VlExpr* expr)
{
  if ( expr == NULL ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  const char* nm = NULL;
  switch ( expr->type() ) {
  case kVpiOperation:              nm = "Operation"; break;
  case kVpiConstant:               nm = "Constant"; break;
  case kVpiFuncCall:               nm = "FuncCall"; break;
  case kVpiSysFuncCall:            nm = "SysFuncCall"; break;
  default:
    if ( expr->is_primary() ) {
      nm = "Reference";
    }
    else if ( expr->is_bitselect() ) {
      nm = "BitSelect";
    }
    else if ( expr->is_partselect() ) {
      nm = "PartSelect";
    }
    break;
  }

  VlDumpHeader x(this, label, nm);
  put("FileRegion", expr->file_region());

  switch ( expr->type() ) {
  case kVpiOperation:
    put("vpiOpType", expr->op_type());
    for (ymuint i = 0; i < expr->operand_num(); ++ i) {
      put_expr("vpiOperand", mgr, expr->operand(i));
    }
    break;

  case kVpiConstant:
    put("vpiConstType", expr->constant_type());
    put("vpiDecompile", expr->decompile()); // <= 手抜き
    break;

  case kVpiFuncCall:
    put("vpiFunction", expr->function()->full_name());
    for (ymuint i = 0; i < expr->argument_num(); ++ i) {
      put_expr("vpiArgument", mgr, expr->argument(i));
    }
    break;

  case kVpiSysFuncCall:
    put("vpiUserSysTf", expr->user_systf()->name());
    for (ymuint i = 0; i < expr->argument_num(); ++ i) {
      put_expr("vpiArgument", mgr, expr->argument(i));
    }
    break;

  default:
    if ( expr->is_bitselect() ) {
      put_primary(mgr, expr);
      if ( expr->is_constant_select() ) {
	put("vpiIndex", expr->index_val());
      }
      else {
	put_expr("vpiIndex", mgr, expr->index());
      }

    }
    else if ( expr->is_partselect() ) {
      put_primary(mgr, expr);
      const char* rm_str = NULL;
      switch ( expr->range_mode() ) {
      case kVpiConstRange: rm_str = "constant_range"; break;
      case kVpiPlusRange:  rm_str = "plus_range"; break;
      case kVpiMinusRange:  rm_str = "minus_range"; break;
      default: assert_not_reached(__FILE__, __LINE__);
      }
      put("range_mode", rm_str);
      if ( expr->is_constant_select() ) {
	put("vpiLeftRange", expr->left_range_val());
	put("vpiRightRange", expr->right_range_val());
      }
      else {
	put_expr("base", mgr, expr->base());
	put("range", expr->range_width());
      }
    }
    else if ( expr->is_primary() ) {
      put_primary(mgr, expr);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    break;
  }
}

// @brief 左辺式の内容を出力する関数
void
VlDumperImpl::put_lhs(const char* label,
		      const VlMgr& mgr,
		      const VlExpr* expr)
{
  put_expr(label, mgr, expr);
  ymuint n = expr->lhs_elem_num();
  if ( n > 1 ) {
    VlDumpHeader x1(this, "vpiLhs", "LhsElemArray");
    for (ymuint i = 0; i < n; ++ i) {
      put_expr("vpiExpr", mgr, expr->lhs_elem(i));
    }
  }
}

// @brief プライマリ式を出力する関数
void
VlDumperImpl::put_primary(const VlMgr& mgr,
			  const VlExpr* expr)
{
  if ( expr->decl_obj() ) {
    put("decl_obj", expr->decl_obj()->full_name());
  }
  else if ( expr->declarray_obj() ) {
    put("declarray_obj", expr->declarray_obj()->full_name());
    if ( expr->is_constant_select() ) {
      put("offset", expr->declarray_offset());
    }
    else {
      for (ymuint i = 0; i < expr->declarray_dimension(); ++ i) {
	put_expr("array_index", mgr, expr->declarray_index(i));
      }
    }
  }
  else if ( expr->scope_obj() ) {
    put("scope_obj", expr->scope_obj()->full_name());
  }
  else if ( expr->primitive_obj() ) {
    put("primitive_obj", expr->primitive_obj()->full_name());
  }
  else if( expr->parent_expr() ) {
    put_expr("parent", mgr, expr->parent_expr());
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief 遅延値を出力する関数
void
VlDumperImpl::put_delay(const char* label,
			const VlMgr& mgr,
			const VlDelay* delay)
{
  if ( delay == NULL ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  VlDumpHeader x(this, label, "Delay");

  put("expr", delay->decompile() );
}

// range の内容を出力する関数
// IEEE 1364-2001 p.644
void
VlDumperImpl::put_range(const char* label,
			const VlMgr& mgr,
			const VlRange* range)
{
  if ( range == NULL ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  VlDumpHeader x(this, label, "Range");

  put("FileRegion", range->file_region() );

  put("vpiSize", range->size() );

  put("vpiLeftRange", range->left_range_val() );
  put("vpiRightRange", range->right_range_val() );
}

END_NAMESPACE_YM_VERILOG
