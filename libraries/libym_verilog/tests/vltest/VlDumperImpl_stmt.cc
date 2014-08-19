
/// @file VlDumperImpl_stmt.cc
/// @brief VlDumperImpl の実装ファイル (stmt担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlDumperImpl.h"

#include "YmVerilog/VlMgr.h"
#include "YmVerilog/vl/VlDecl.h"
#include "YmVerilog/vl/VlProcess.h"
#include "YmVerilog/vl/VlStmt.h"
#include "YmVerilog/vl/VlControl.h"
#include "YmVerilog/vl/VlModule.h"
#include "YmVerilog/vl/VlTaskFunc.h"
#include "YmVerilog/vl/VlUserSystf.h"
#include "YmVerilog/vl/VlExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// initial/always の内容を出力する関数
// IEEE 1364-2001 p.656
void
VlDumperImpl::put_process(const char* label,
			  const VlMgr& mgr,
			  const VlProcess* process)
{
  const char* nm = NULL;
  switch ( process->type() ) {
  case kVpiInitial: nm = "Initial"; break;
  case kVpiAlways:  nm = "Always"; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", process->file_region() );
  put("vpiModule", process->parent()->parent_module()->full_name() );
  put_stmt("vpiStmt", mgr, process->stmt() );
}

// @brief initial/always のリストの内容を出力する関数
void
VlDumperImpl::put_process_list(const char* label,
			       const VlMgr& mgr,
			       const vector<const VlProcess*>& process_list)
{
  VlDumpHeader x(this, label, "ProcessList");

  for (vector<const VlProcess*>::const_iterator p = process_list.begin();
       p != process_list.end(); ++ p) {
    put_process(label, mgr, *p);
  }
}

// statement の内容を出力する関数
void
VlDumperImpl::put_stmt(const char* label,
		       const VlMgr& mgr,
		       const VlStmt* stmt)
{
  const char* nm = NULL;
  switch ( stmt->type() ) {
  case kVpiBegin:           nm = "Begin"; break;
  case kVpiFork:            nm = "Fork"; break;
  case kVpiNamedBegin:      nm = "NamedBegin"; break;
  case kVpiNamedFork:       nm = "NamedFork"; break;
  case kVpiNullStmt:        nm = "NullStmt"; break;
  case kVpiEventStmt:       nm = "EventStmt"; break;
  case kVpiAssignment:      nm = "Assignment"; break;
  case kVpiWhile:           nm = "While"; break;
  case kVpiRepeat:          nm = "Repeat"; break;
  case kVpiWait:            nm = "Wait"; break;
  case kVpiFor:             nm = "For"; break;
  case kVpiForever:         nm = "Forever"; break;
  case kVpiIf:              nm = "If"; break;
  case kVpiIfElse:          nm = "IfElse"; break;
  case kVpiCase:            nm = "Case"; break;
  case kVpiForce:           nm = "Force"; break;
  case kVpiAssignStmt:      nm = "AssignStmt"; break;
  case kVpiDeassign:        nm = "Deassign"; break;
  case kVpiRelease:         nm = "Release"; break;
  case kVpiTaskCall:        nm = "TaskCall"; break;
  case kVpiSysTaskCall:     nm = "SysTaskCall"; break;
  case kVpiDisable:         nm = "Disable"; break;
  case kVpiDelayControl:    nm = "DelayControl"; break;
  case kVpiEventControl:    nm = "EventControl"; break;
  default: assert_not_reached( __FILE__, __LINE__);
  }

  VlDumpHeader x(this, label, nm);

  put("FileRegion", stmt->file_region());
  put("vpiScope", stmt->parent()->full_name() );

  switch ( stmt->type() ) {
  case kVpiNamedBegin:
  case kVpiNamedFork:
    // スコープとしての内容は別に出力されている．
  case kVpiBegin:
  case kVpiFork:
    put_child_stmt_list("vpiStmt", mgr, stmt);
    break;

  case kVpiEventStmt:
    put("vpiNamedEvent", stmt->named_event()->decompile());
    break;

  case kVpiAssignment:
    put("vpiBlocking", stmt->is_blocking() );
    put_lhs("vpiLhs", mgr, stmt->lhs() );
    put_expr("vpiRhs", mgr, stmt->rhs() );
    put_control("control", mgr, stmt->control() );
    break;

  case kVpiWhile:
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case kVpiRepeat:
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case kVpiWait:
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case kVpiFor:
    put_stmt("vpiForInitStmt", mgr, stmt->init_stmt() );
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiForIncStmt", mgr, stmt->inc_stmt() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case kVpiForever:
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case kVpiIf:
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  case kVpiIfElse:
    put_expr("vpiCondition", mgr, stmt->expr() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    put_stmt("vpiElseStmt", mgr, stmt->else_stmt() );
    break;

  case kVpiCase:
    put("vpiCaseType", stmt->case_type() );
    put_expr("vpiCondition", mgr, stmt->expr() );
    for (ymuint i = 0; i < stmt->caseitem_num(); ++ i) {
      VlDumpHeader x(this, "vpiCaseItem", "CaseItem");
      const VlCaseItem* ci = stmt->caseitem(i);
      put("FileRegion", ci->file_region() );
      for (ymuint j = 0; j < ci->expr_num(); ++ j) {
	put_expr("vpiExpr", mgr, ci->expr(j) );
      }
      put_stmt("vpiStmt", mgr, ci->body_stmt() );
    }
    break;

  case kVpiAssignStmt:
  case kVpiForce:
    put_lhs("vpiLhs", mgr, stmt->lhs() );
    put_expr("vpiRhs", mgr, stmt->rhs() );
    break;

  case kVpiDeassign:
  case kVpiRelease:
    put_lhs("vpiLhs", mgr, stmt->lhs() );
    break;

  case kVpiTaskCall:
    put("vpiTask", stmt->task()->full_name() );
    put_argument_list("vpiArgument", mgr, stmt);
    break;

  case kVpiSysTaskCall:
#if 0
    put("vpiUserDefn", handle.get_bool(vpiUserDefn));
#else
#warning "TODO: SystemTaskCall の userDefn"
#endif
    put("vpiUserSystf", stmt->user_systf()->name() );
    put_argument_list("vpiArgument", mgr, stmt);
    break;

  case kVpiDisable:
    put("vpiExpr", stmt->scope()->full_name() );
    break;

  case kVpiDelayControl:
  case kVpiEventControl:
    put_control("control", mgr, stmt->control() );
    put_stmt("vpiStmt", mgr, stmt->body_stmt() );
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief statement のリストの内容を出力する関数
void
VlDumperImpl::put_stmt_list(const char* label,
			    const VlMgr& mgr,
			    const vector<const VlStmt*>& stmt_list)
{
  VlDumpHeader x(this, label, "StmtList");

  for (vector<const VlStmt*>::const_iterator p = stmt_list.begin();
       p != stmt_list.end(); ++ p) {
    put_stmt(label, mgr, *p);
  }
}

// @brief delay/event/repeat control の内容を出力する関数
void
VlDumperImpl::put_control(const char* label,
			  const VlMgr& mgr,
			  const VlControl* control)
{
  if ( control == NULL ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  if ( control->delay() ) {
    VlDumpHeader x(this, label, "DelayControl");
    put("FileRegion", control->file_region() );
    put("vpiDelay", control->delay() );
  }
  else if ( control->expr() ) {
    VlDumpHeader x(this, label, "RepeatControl");
    put("FileRegion", control->file_region() );
    put_expr("vpiExpr", mgr, control->expr() );
    for (ymuint i = 0; i < control->event_num(); ++ i) {
      put_expr("vpiCondition", mgr, control->event(i) );
    }
  }
  else {
    VlDumpHeader x(this, label, "EventControl");
    put("FileRegion", control->file_region() );
    for (ymuint i = 0; i < control->event_num(); ++ i) {
      put_expr("vpiCondition", mgr, control->event(i) );
    }
  }
}

// @brief 子供のステートメントの内容を出力する関数
void
VlDumperImpl::put_child_stmt_list(const char* label,
				  const VlMgr& mgr,
				  const VlStmt* stmt)
{
  ymuint n = stmt->child_stmt_num();
  for (ymuint i = 0; i < n; ++ i) {
    put_stmt(label, mgr, stmt->child_stmt(i) );
  }
}

// @brief 引数のリストを出力する関数
void
VlDumperImpl::put_argument_list(const char* label,
				const VlMgr& mgr,
				const VlStmt* stmt)
{
  ymuint n = stmt->arg_num();
  for (ymuint i = 0; i < n; ++ i) {
    put_expr(label, mgr, stmt->arg(i) );
  }
}

END_NAMESPACE_YM_VERILOG
