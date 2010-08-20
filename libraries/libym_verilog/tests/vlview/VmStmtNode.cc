
/// @file libym_verilog/tests/vlview/VmStmtNode.cc
/// @brief VmStmtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmStmtNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmStmtNode.h"
#include "VmDeclNode.h"
#include "VmExprNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/vl/VlStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

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
    add_child( new VmStmtNode("statement", *p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmStmtNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label
// @param[in] stmt ステートメント
VmStmtNode::VmStmtNode(const QString& label,
		       const SlStmt* stmt) :
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
      case kPtDisableStmt:       return "Disable";
      case kPtEnableStmt:        return "Enable";
      case kPtSysEnableStmt:     return "SysEnable";
      case kPtDcStmt:            return "ControlStmt";
      case kPtEcStmt:            return "ControlStmt";
      case kPtAssignStmt:        return "Assign";
      case kPtNbAssignStmt:      return "NonBlockingAssign";
      case kPtEventStmt:         return "EventTrigger";
      case kPtNullStmt:          return "NullStmt";
      case kPtIfStmt:            return "If";
      case kPtCaseStmt:          return "Case";
      case kPtCaseXStmt:         return "Casex";
      case kPtCaseZStmt:         return "Casez";
      case kPtWaitStmt:          return "Wait";
      case kPtForeverStmt:       return "Forever";
      case kPtRepeatStmt:        return "Repeat";
      case kPtWhileStmt:         return "While";
      case kPtForStmt:           return "For";
      case kPtPcAssignStmt:      return "PcAssign";
      case kPtDeassignStmt:      return "Deassign";
      case kPtForceStmt:         return "Force";
      case kPtReleaseStmt:       return "Release";
      case kPtParBlockStmt:      return "Fork";
      case kPtSeqBlockStmt:      return "Begin";
      case kPtNamedParBlockStmt: return "Fork";
      case kPtNamedSeqBlockStmt: return "Begin";
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
  ymuint32 n = 0;
  switch ( mStmt->type() ) {
  case kPtDisableStmt:
    n = 1;
    if ( mStmt->namebranch_array().size() > 0 ) {
      ++ n;
    }
    mChildren.reserve(n);
    if ( mStmt->namebranch_array().size() > 0 ) {
      mChildren.push_back( new NameBranchListNode(mStmt->namebranch_array()) );
    }
    mChildren.push_back( new StrNode("Name", mStmt->name()) );
    break;

  case kPtEnableStmt:
  case kPtSysEnableStmt:
    n = 2;
    if ( mStmt->namebranch_array().size() > 0 ) {
      ++ n;
    }
    mChildren.reserve(n);
    if ( mStmt->namebranch_array().size() > 0 ) {
      mChildren.push_back( new NameBranchListNode(mStmt->namebranch_array()) );
    }
    mChildren.push_back( new StrNode("Name", mStmt->name()) );
    mChildren.push_back( new ArgListNode(mStmt) );
    break;

  case kPtDcStmt:
  case kPtEcStmt:
    mChildren.resize(2);
    mChildren[0] = new ControlNode(mStmt->control());
    mChildren[1] = new VmStmtNode("Body", mStmt->body());
    break;

  case kPtWaitStmt:
    mChildren.resize(2);
    mChildren[0] = new ExprNode("Condition", mStmt->expr());
    mChildren[1] = new VmStmtNode("Body", mStmt->body());
    break;

  case kPtAssignStmt:
  case kPtNbAssignStmt:
  case kPtPcAssignStmt:
  case kPtForceStmt:
  case kPtDeassignStmt:
  case kPtReleaseStmt:
    n = 2;
    if ( mStmt->control() ) {
      ++ n;
    }
    mChildren.resize(n);
    mChildren[0] = new ExprNode("Lhs", mStmt->lhs());
    mChildren[1] = new ExprNode("Rhs", mStmt->rhs());
    if ( mStmt->control() ) {
      mChildren[2] = new ControlNode(mStmt->control());
    }
    break;

  case kPtEventStmt:
    mChildren.resize(1);
    mChildren[0] = new ExprNode("Event Name", mStmt->primary());
    break;

  case kPtNullStmt:
    break;

  case kPtIfStmt:
    n = 2;
    if ( mStmt->else_body() ) {
      ++ n;
    }
    mChildren.resize(n);
    mChildren[0] = new ExprNode("Condition", mStmt->expr());
    mChildren[1] = new VmStmtNode("Then body", mStmt->body());
    if ( mStmt->else_body() ) {
      mChildren[2] = new VmStmtNode("Else Body", mStmt->else_body());
    }
    break;

  case kPtCaseStmt:
  case kPtCaseXStmt:
  case kPtCaseZStmt:
    mChildren.resize(2);
    mChildren[0] = new ExprNode("Condition", mStmt->expr());
    mChildren[1] = new CaseItemListNode(mStmt);
    break;

  case kPtForeverStmt:
    mChildren.resize(1);
    mChildren[0] = new VmStmtNode("Body", mStmt->body());
    break;
    
  case kPtRepeatStmt:
  case kPtWhileStmt:
    mChildren.resize(2);
    mChildren[0] = new ExprNode("Condition", mStmt->expr());
    mChildren[1] = new VmStmtNode("Body", mStmt->body());
    break;
    
  case kPtForStmt:
    mChildren.resize(4);
    mChildren[0] = new VmStmtNode("Initial Stmt", mStmt->init_stmt());
    mChildren[1] = new ExprNode("Condition", mStmt->expr());
    mChildren[2] = new VmStmtNode("Increent Stmt", mStmt->next_stmt());
    mChildren[3] = new VmStmtNode("Body", mStmt->body());
    break;
    
  case kPtNamedParBlockStmt:
  case kPtNamedSeqBlockStmt:
    n = 1;
    if ( mStmt->declhead_array().size() > 0 ) {
      ++ n;
    }
    if ( mStmt->stmt_array().size() > 0 ) {
      ++ n;
    }
    mChildren.reserve(n);
    mChildren.push_back( new StrNode("Name", mStmt->name()) );
    if ( mStmt->declhead_array().size() > 0 ) {
      mChildren.push_back( new DeclHeadListNode("Decl List",
						mStmt->declhead_array()) );
    }
    if ( mStmt->stmt_array().size() > 0 ) {
      mChildren.push_back( new VmStmtListNode("Stmt List",
					    mStmt->stmt_array()) );
    }
    break;

  case kPtParBlockStmt:
  case kPtSeqBlockStmt:
    n = 0;
    if ( mStmt->declhead_array().size() > 0 ) {
      ++ n;
    }
    if ( mStmt->stmt_array().size() > 0 ) {
      ++ n;
    }
    mChildren.reserve(n);
    if ( mStmt->declhead_array().size() > 0 ) {
      mChildren.push_back( new DeclHeadListNode("Decl List",
						mStmt->declhead_array()) );
    }
    if ( mStmt->stmt_array().size() > 0 ) {
      mChildren.push_back( new VmStmtListNode("Stmt List",
					    mStmt->stmt_array()) );
    }
    break;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ArgListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt 親の enable 文
ArgListNode::ArgListNode(const PtStmt* stmt) :
  mStmt(stmt)
{
}

// @brief デストラクタ
ArgListNode::~ArgListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ArgListNode::data(int column,
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
ArgListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
ArgListNode::expand() const
{
  ymuint32 n = mStmt->arg_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new ExprNode("Argument", mStmt->arg(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス CaseItemListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt 親の case 文
CaseItemListNode::CaseItemListNode(const PtStmt* stmt) :
  mStmt(stmt)
{
}

// @brief デストラクタ
CaseItemListNode::~CaseItemListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
CaseItemListNode::data(int column,
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
CaseItemListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
CaseItemListNode::expand() const
{
  ymuint32 n = mStmt->caseitem_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new CaseItemNode(mStmt->caseitem(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス CaseItemNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] caseitem caseitem 文
CaseItemNode::CaseItemNode(const PtCaseItem* caseitem) :
  mCaseItem(caseitem)
{
}

// @brief デストラクタ
CaseItemNode::~CaseItemNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
CaseItemNode::data(int column,
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
CaseItemNode::loc() const
{
  return mCaseItem->file_region();
}

// @brief 子供の配列を作る．
void
CaseItemNode::expand() const
{
  ymuint32 n = mCaseItem->label_num();
  ymuint32 m = n + 1;
  if ( n == 0 ) {
    m = 2;
  }
  mChildren.reserve(m);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren.push_back( new ExprNode("Label", mCaseItem->label(i)) );
  }
  if ( n == 0 ) {
    mChildren.push_back( new StrNode("Label", "Default") );
  }
  mChildren.push_back( new VmStmtNode("Body", mCaseItem->body()) );
}

END_NAMESPACE_YM_VERILOG
