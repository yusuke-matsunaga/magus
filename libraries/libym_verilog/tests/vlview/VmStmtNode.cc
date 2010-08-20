
/// @file libym_verilog/tests/vlview/VmStmtNode.cc
/// @brief VmStmtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmStmtNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmStmtNode.h"
#include "ym_verilog/vl/VlStmt.h"
#include "ym_verilog/vl/VlTaskFunc.h"
#include "ym_verilog/vl/VlUserSystf.h"
#include "ym_verilog/vl/VlControl.h"
#include "ym_verilog/vl/VlExpr.h"
#include "ym_verilog/vl/VlNamedObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmNode の関数
//////////////////////////////////////////////////////////////////////

// @brief StmtListNodeを追加する．
// @param[in] label ラベル
// @param[in] stmt_list ステートメントの配列
void
VmNode::add_child(const QString& label,
		  const vector<const VlStmt*>& stmt_list) const
{
  add_child( new VmStmtListNode(label, stmt_list) );
}

// @brief StmtNodeを追加する．
// @param[in] label ラベル
// @param[in] stmt ステートメント
void
VmNode::add_child(const QString& label,
		  const VlStmt* stmt) const
{
  add_child( new VmStmtNode(label, stmt) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmStmtListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] stmt_array ステートメントの配列
VmStmtListNode::VmStmtListNode(const QString& label,
			       const vector<const VlStmt*>& stmt_array) :
  mLabel(label),
  mStmtArray(stmt_array)
{
}

// @brief デストラクタ
VmStmtListNode::~VmStmtListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmStmtListNode::data(int column,
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
VmStmtListNode::loc() const
{
  if ( mStmtArray.empty() ) {
    return FileRegion();
  }
  else {
    FileRegion first = mStmtArray.front()->file_region();
    FileRegion last = mStmtArray.back()->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmStmtListNode::expand() const
{
  for (vector<const VlStmt*>::const_iterator p = mStmtArray.begin();
       p != mStmtArray.end(); ++ p) {
    add_child("statement", *p);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmStmtNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label
// @param[in] stmt ステートメント
VmStmtNode::VmStmtNode(const QString& label,
		       const VlStmt* stmt) :
  mLabel(label),
  mStmt(stmt)
{
}

// @brief デストラクタ
VmStmtNode::~VmStmtNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmStmtNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      switch ( mStmt->type() ) {
      case kVpiBegin:           return "Begin";
      case kVpiFork:            return "Fork";
      case kVpiNamedBegin:      return "NamedBegin";
      case kVpiNamedFork:       return "NamedFork";
      case kVpiNullStmt:        return "NullStmt";
      case kVpiEventStmt:       return "EventStmt";
      case kVpiAssignment:      return "Assignment";
      case kVpiWhile:           return "While";
      case kVpiRepeat:          return "Repeat";
      case kVpiWait:            return "Wait";
      case kVpiFor:             return "For";
      case kVpiForever:         return "Forever";
      case kVpiIf:              return "If";
      case kVpiIfElse:          return "IfElse";
      case kVpiCase:            return "Case";
      case kVpiForce:           return "Force";
      case kVpiAssignStmt:      return "AssignStmt";
      case kVpiDeassign:        return "Deassign";
      case kVpiRelease:         return "Release";
      case kVpiTaskCall:        return "TaskCall";
      case kVpiSysTaskCall:     return "SysTaskCall";
      case kVpiDisable:         return "Disable";
      case kVpiDelayControl:    return "DelayControl";
      case kVpiEventControl:    return "EventControl";
      default: assert_not_reached( __FILE__, __LINE__);
      }
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmStmtNode::loc() const
{
  return mStmt->file_region();
}

// @brief 子供の配列を作る．
void
VmStmtNode::expand() const
{
  add_child("vpiScope", mStmt->parent()->full_name());
  
  switch ( mStmt->type() ) {
  case kVpiNamedBegin:
  case kVpiNamedFork:
    {
      const VlNamedObj* scope = mStmt->scope();
      assert_cond( scope, __FILE__, __LINE__);
      add_child("vpiFullName", scope->full_name());
      //pus_scope_sub(scope);
    }
    // わざと次に続く
  case kVpiBegin:
  case kVpiFork:
    {
      ymuint n = mStmt->child_stmt_num();
      vector<const VlStmt*> stmt_array(n);
      for (ymuint i = 0; i < n; ++ i) {
	stmt_array[i] = mStmt->child_stmt(i);
      }
      add_child("vpiChildStmt", stmt_array);
    }
    break;

  case kVpiEventStmt:
    add_child("vpiNamedEvent", mStmt->named_event());
    break;

  case kVpiAssignment:
    add_child("vpiBlocking", mStmt->is_blocking());
    add_child("vpiLhs", mStmt->lhs());
    add_child("vpiRhs", mStmt->rhs());
    if ( mStmt->control() ) {
      add_child( new VmControlNode(mStmt->control()) );
    }
    break;

  case kVpiWhile:
  case kVpiRepeat:
  case kVpiWait:
    add_child("vpiCondition", mStmt->expr());
    add_child("vpiStmt", mStmt->body_stmt());
    break;

  case kVpiFor:
    add_child("vpiForInitStmt", mStmt->init_stmt());
    add_child("vpiCondition", mStmt->expr());
    add_child("vpiForIncStmt", mStmt->inc_stmt());
    add_child("vpiStmt", mStmt->body_stmt());
    break;

  case kVpiForever:
    add_child("vpiStmt", mStmt->body_stmt());
    break;

  case kVpiIf:
    add_child("vpiCondition", mStmt->expr());
    add_child("vpiStmt", mStmt->body_stmt());
    break;

  case kVpiIfElse:
    add_child("vpiCondition", mStmt->expr());
    add_child("vpiStmt", mStmt->body_stmt());
    add_child("vpiElseStmt", mStmt->else_stmt());
    break;

  case kVpiCase:
    switch ( mStmt->case_type() ) {
    case kVpiCaseExact:
      add_child("vpiCaseType", "vpiCaseExact");
      break;

    case kVpiCaseX:
      add_child("vpiCaseType", "vpiCaseX");
      break;

    case kVpiCaseZ:
      add_child("vpiCaseType", "vpiCaseZ");
      break;
    }
    add_child("vpiCondition", mStmt->expr());
    add_child( new VmCaseItemListNode(mStmt) );
    break;

  case kVpiAssignStmt:
  case kVpiForce:
    add_child("vpiLhs", mStmt->lhs());
    add_child("vpiRhs", mStmt->rhs());
    break;

  case kVpiDeassign:
  case kVpiRelease:
    add_child("vpiLhs", mStmt->lhs());
    break;

  case kVpiTaskCall:
    add_child("vpiTask", mStmt->task()->full_name());
    add_child( new VmArgListNode(mStmt) );
    break;

  case kVpiSysTaskCall:
    add_child("vpiUserSystf", mStmt->user_systf()->name());
    add_child( new VmArgListNode(mStmt) );
    break;
    
  case kVpiDisable:
    add_child("vpiExpr", mStmt->scope()->full_name());
    break;

  case kVpiDelayControl:
  case kVpiEventControl:
    add_child( new VmControlNode(mStmt->control()) );
    add_child("vpiStmt", mStmt->body_stmt());
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmArgListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt 親のステートメント
VmArgListNode::VmArgListNode(const VlStmt* stmt) :
  mStmt(stmt)
{
}

// @brief デストラクタ
VmArgListNode::~VmArgListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmArgListNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Argument List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmArgListNode::loc() const
{
  ymuint n = mStmt->arg_num();
  if ( n == 0 ) {
    return FileRegion();
  }
  else {
    FileRegion first = mStmt->arg(0)->file_region();
    FileRegion last = mStmt->arg(n - 1)->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmArgListNode::expand() const
{
  ymuint n = mStmt->arg_num();
  for (ymuint i = 0; i < n; ++ i) {
    add_child("vpiArgument", mStmt->arg(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmCaseItemListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt 親の case 文
VmCaseItemListNode::VmCaseItemListNode(const VlStmt* stmt) :
  mStmt(stmt)
{
}

// @brief デストラクタ
VmCaseItemListNode::~VmCaseItemListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmCaseItemListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Case Item List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmCaseItemListNode::loc() const
{
  ymuint n = mStmt->caseitem_num();
  if ( n == 0 ) {
    return FileRegion();
  }
  else {
    FileRegion first = mStmt->caseitem(0)->file_region();
    FileRegion last = mStmt->caseitem(n - 1)->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmCaseItemListNode::expand() const
{
  ymuint n = mStmt->caseitem_num();
  for (ymuint i = 0; i < n; ++ i) {
    add_child( new VmCaseItemNode(mStmt->caseitem(i)) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmCaseItemNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] caseitem caseitem 文
VmCaseItemNode::VmCaseItemNode(const VlCaseItem* caseitem) :
  mCaseItem(caseitem)
{
}

// @brief デストラクタ
VmCaseItemNode::~VmCaseItemNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmCaseItemNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Case Item";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmCaseItemNode::loc() const
{
  return mCaseItem->file_region();
}

// @brief 子供の配列を作る．
void
VmCaseItemNode::expand() const
{
  ymuint n = mCaseItem->expr_num();
  if ( n == 0 ) {
    add_child("vpiLabel", "Default");
  }
  else {
    for (ymuint i = 0; i < n; ++ i) {
      add_child("vpiExpr", mCaseItem->expr(i));
    }
  }
  add_child("vpiBody", mCaseItem->body_stmt());
}


//////////////////////////////////////////////////////////////////////
// クラス VmControlNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] control コントロールを表すオブジェクト
VmControlNode::VmControlNode(const VlControl* control) :
  mControl(control)
{
}

// @brief デストラクタ
VmControlNode::~VmControlNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmControlNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      if ( mControl->delay() ) {
	return "vpiDelayControl";
      }
      else if ( mControl->expr() ) {
	return "vpiRepeatControl";
      }
      else {
	return "vpiEventControl";
      }
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmControlNode::loc() const
{
  return mControl->file_region();
}

// @brief 子供の配列を作る．
void
VmControlNode::expand() const
{
  if ( mControl->delay() ) {
    add_child("vpiDelay", mControl->delay());
  }
  else {
    if ( mControl->expr() ) {
      add_child("vpiExpr", mControl->expr());
    }
    ymuint n = mControl->event_num();
    vector<const VlExpr*> event_list(n);
    for (ymuint i = 0; i < n; ++ i) {
      event_list[i] = mControl->event(i);
    }
    add_child("vpiCondition", event_list);
  }
}


END_NAMESPACE_YM_VERILOG
