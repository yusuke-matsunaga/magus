
/// @file VlDumperImpl.cc
/// @brief VlDumperImpl の実装ファイル (item担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlDumperImpl.h"

#include "verilog/VlMgr.h"
#include "verilog/vl/VlUdp.h"
#include "verilog/vl/VlPrimitive.h"
#include "verilog/vl/VlTaskFunc.h"
#include "verilog/vl/VlContAssign.h"
#include "verilog/vl/VlModule.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief primitive array のリストの内容を出力する関数
void
VlDumperImpl::put_primarray_list(const char* label,
				 const VlMgr& mgr,
				 vector<const VlPrimArray*>& primarray_list)
{
  VlDumpHeader x(this, label, "PrimitiveArrayList");

  for (vector<const VlPrimArray*>::const_iterator p = primarray_list.begin();
       p != primarray_list.end(); ++ p) {
    const VlPrimArray* primarray = *p;

    const char* nm = NULL;
    switch ( primarray->type() ) {
    case kVpiGateArray:   nm = "GateArray"; break;
    case kVpiSwitchArray: nm = "SwitchArray"; break;
    case kVpiUdpArray:    nm = "UdpArray"; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    VlDumpHeader x(this, label, nm);

    put("FileRegion", primarray->file_region() );
    put("vpiFullName", primarray->full_name() );
    put("vpiSize", primarray->elem_num() );
    put("vpiLeftRange", primarray->left_range_val() );
    put("vpiRightRange", primarray->right_range_val() );
    put_delay("vpiDelay", mgr, primarray->delay() );

    ymuint n = primarray->elem_num();
    for (ymuint i = 0; i < n; ++ i) {
      const VlPrimitive* primitive = primarray->elem_by_offset(i);
      put_primitive("vpiPrimitive", mgr, primitive);
    }
  }
}

// primitive の内容を出力する関数
// IEEE 1364-2001 p.646
void
VlDumperImpl::put_primitive(const char* label,
			    const VlMgr& mgr,
			    const VlPrimitive* prim)
{
  const char* nm = NULL;
  switch ( prim->type() ) {
  case kVpiGate:   nm = "Gate"; break;
  case kVpiSwitch: nm = "Switch"; break;
  case kVpiUdp:    nm = "Udp"; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", prim->file_region() );
  put("vpiFullName", prim->full_name() );
  put("vpiDefName", prim->def_name() );
  put("vpiPrimType", prim->prim_type() );
#if 0
  put("vpiArray", handle.get_bool(vpiArray));
  if ( handle.get_bool(vpiArray) ) {
    put_str("vpiPrimitiveArray", handle.get_handle(vpiPrimitiveArray),
	    vpiFullName);
    put("vpiIndex", handle.get_handle(vpiIndex));
  }
#else
#warning "TODO: primitive の primitive_array/index"
#endif

  put("vpiModule", prim->parent_module()->full_name() );
  put("vpiScope", prim->parent()->full_name() );

  put("vpiStrength0", prim->drive0() );
  put("vpiStrength1", prim->drive1() );
  put("vpiDelay", prim->delay() );

  if ( prim->type() == kVpiUdp ) {
    put("vpiUdpDefn", prim->udp_defn()->def_name() );
  }

#if 0
  put("vpiSize", handle.get_int(vpiSize));
#else
  put("port_num", prim->port_num() );
#endif

  ymuint n = prim->port_num();
  for (ymuint i = 0; i < n; ++ i) {
    put_primterm("vpiPrimTerm", mgr, prim->prim_term(i) );
  }
}

// @brief primitive のリスト内容を出力する関数
void
VlDumperImpl::put_primitive_list(const char* label,
				 const VlMgr& mgr,
				 const vector<const VlPrimitive*>& primitive_list)
{
  VlDumpHeader x(this, label, "PrimitiveList");

  for (vector<const VlPrimitive*>::const_iterator p = primitive_list.begin();
       p != primitive_list.end(); ++ p) {
    put_primitive(label, mgr, *p);
  }
}

// prim term の内容を出力する関数
// IEEE 1364-2001 p.646
void
VlDumperImpl::put_primterm(const char* label,
			   const VlMgr& mgr,
			   const VlPrimTerm* primterm)
{
  VlDumpHeader x(this, label, "PrimTerm");

  put("FileRegion", primterm->file_region() );

  put("vpiPrimitive", primterm->primitive()->full_name() );
  put("vpiTermIndex", primterm->term_index() );
  put("vpiDirection", primterm->direction() );
  put_expr("vpiExpr", mgr, primterm->expr() );
}

// udp defn の内容を出力する関数
// IEEE 1364-2001 p.647
void
VlDumperImpl::put_udp_defn(const char* label,
			   const VlMgr& mgr,
			   const VlUdpDefn* udp)
{
  VlDumpHeader x(this, label, "UdpDefn");

  put("FileRegion", udp->file_region() );

  put("vpiDefName", udp->def_name() );
  put("vpiPrimType", udp->prim_type() );
  put("vpiProtected", udp->is_protected() );
  put("port_num", udp->port_num() );

  ymuint n = udp->port_num() - 1;
  for (ymuint i = 0; i < n; ++ i) {
    put_iodecl("vpiIODecl", mgr, udp->input(i));
  }
  put_iodecl("vpiIODecl", mgr, udp->output());

  put("vpiInitial", udp->init_val_string());

  {
    VlDumpHeader x(this, "vpiTableEntry", "Iterator");
    ymuint n = udp->table_size();
    for (ymuint i = 0; i < n; ++ i) {
      const VlTableEntry* entry = udp->table_entry(i);
      put("TableEntry", entry->str());
#if 0
      s_vpi_value s_value;
      s_value.format = vpiStringVal;
      handle1.get_value(&s_value);
      string str = s_value.value.str;
      put("TableEntry", str);
#endif
    }
  }
}

// task の内容を出力する関数
// IEEE 1364-2001 p.649
void
VlDumperImpl::put_task(const char* label,
		       const VlMgr& mgr,
		       const VlTaskFunc* task)
{
  VlDumpHeader x(this, label, "Task");

  put("FileRegion", task->file_region() );
  put("vpiFullName", task->full_name() );

  ymuint n = task->io_num();
  for (ymuint i = 0; i < n; ++ i) {
    put_iodecl("vpiIODecl", mgr, task->io(i) );
  }

  put_scope_sub(mgr, task);

  put_stmt("vpiStmt", mgr, task->stmt() );
}

// function の内容を出力する関数
// IEEE 1364-2001 p.649
void
VlDumperImpl::put_function(const char* label,
			   const VlMgr& mgr,
			   const VlTaskFunc* func)
{
  VlDumpHeader x(this, label, "Function");

  put("FileRegion", func->file_region() );
  put("vpiFullName", func->full_name() );

  put("vpiFuncType", func->func_type() );
  put("vpiSigned", func->is_signed() );
  put("vpiSize", func->bit_size() );
  put("vpiLeftRange", func->left_range_val() );
  put("vpiRightRange", func->right_range_val() );

  ymuint n = func->io_num();
  for (ymuint i = 0; i < n; ++ i) {
    put_iodecl("vpiIODecl", mgr, func->io(i) );
  }

  put_scope_sub(mgr, func);

  put_stmt("vpiStmt", mgr, func->stmt() );
}

#if 0
// frame の内容を出力する関数
// IEEE 1364-2001 p.651
void
VlDumperImpl::put_frame(const char* label,
		    const VpiHandle& handle)
{
  VlDumpHeader x(this, label, "Frame");
  // 今は省略
}
#endif

// continuous assignment の内容を出力する関数
// IEEE 1364-2001 p.652
void
VlDumperImpl::put_contassign(const char* label,
			     const VlMgr& mgr,
			     const VlContAssign* ca)
{
  VlDumpHeader x(this, label, "ContAssign");

  put("FileRegion", ca->file_region() );

  put("vpiNetDeclAssign", ca->has_net_decl_assign() );

  put("vpiModule", ca->module()->full_name() );
#if 0
  put("vpiScope", ca->scope()->full_name() );
#endif

  put("vpiStrength0", ca->drive0() );
  put("vpiStrength1", ca->drive1() );
  put("vpiDelay", ca->delay() );

  put_lhs("vpiLhs", mgr, ca->lhs() );

  put_expr("vpiRhs", mgr, ca->rhs() );

  put("vpiSize", ca->bit_size() );
#if 0
  if ( mBitExpandMode ) {
    put("vpiBit", handle.get_iterate(vpiBit));
  }
#else
#warning "TODO: ContAssign のビット展開"
#endif
}

// @brief continuous assignment のリストの内容を出力する関数
void
VlDumperImpl::put_contassign_list(const char* label,
				  const VlMgr& mgr,
				  const vector<const VlContAssign*>& ca_list)
{
  VlDumpHeader x(this, label, "ContAssignList");

  for (vector<const VlContAssign*>::const_iterator p = ca_list.begin();
       p != ca_list.end(); ++ p) {
    put_contassign(label, mgr, *p);
  }
}

#if 0
// continuous assignment bit の内容を出力する関数
// IEEE 1364-2001 p.652
void
VlDumperImpl::put_cont_assign_bit(const char* label,
			       const VpiHandle& handle)
{
  VlDumpHeader x(this, label, "ContAssignBit");

  put("FileRegion", handle.file_region());

  put("vpiNetDeclAssign", handle.get_bool(vpiNetDeclAssign));

  put_str("vpiModule", handle.get_handle(vpiModule), vpiFullName);
  put_str("vpiScope", handle.get_handle(vpiScope), vpiFullName);

  put("vpiStrength0",
      static_cast<tVpiStrength>(handle.get_int(vpiStrength0)));
  put("vpiStrength1",
      static_cast<tVpiStrength>(handle.get_int(vpiStrength1)));
  put("vpiDelay", handle.get_handle(vpiDelay));

  put("vpiLhs", handle.get_handle(vpiLhs));
  put("vpiRhs", handle.get_handle(vpiRhs));

  // handle.parent() の情報は出力できない．

  put("vpiOffset", handle.get_int(vpiOffset));
}
#endif

END_NAMESPACE_YM_VERILOG
