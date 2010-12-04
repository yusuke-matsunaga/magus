
/// @file libym_mvn/verilog/CfgMgr.cc
/// @brief CfgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CfgMgr.h"
#include "CfgNode.h"
#include "ym_verilog/vl/VlStmt.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス CfgNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt 対応するステートメント
CfgNode::CfgNode(const VlStmt* stmt) :
  mStmt(stmt),
  mNext(NULL),
  mThen(NULL),
  mElse(NULL),
  mLast(false)
{
}

// @brief デストラクタ
CfgNode::~CfgNode()
{
}


//////////////////////////////////////////////////////////////////////
// クラス CfgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CfgMgr::CfgMgr() :
  mAlloc(sizeof(CfgNode), 1024)
{
}

// @brief デストラクタ
CfgMgr::~CfgMgr()
{
}

// @brief ノード(ステートメント)を追加する．
pair<CfgNode*, CfgNode*>
CfgMgr::add_stmt(const VlStmt* stmt)
{
  using namespace nsVerilog;

  switch ( stmt->type() ) {
  case kVpiDisable:
    // 今は未対応
    cerr << "disable statemet is not handled" << endl;
    break;

  case kVpiIf:
    {
      CfgNode* node = new_node(stmt);
      const VlStmt* stmt1 = stmt->body_stmt();
      pair<CfgNode*, CfgNode*> tb = add_stmt(stmt1);
      node->mThen = tb.first;
      CfgNode* bottom = tb.second;
      bottom->mNext = node;
      bottom->mLast = true;
      return make_pair(node, node);
    }
    break;

  case kVpiIfElse:
    {
      CfgNode* node = new_node(stmt);

      const VlStmt* stmt1 = stmt->body_stmt();
      pair<CfgNode*, CfgNode*> tb1 = add_stmt(stmt1);
      node->mThen = tb1.first;
      CfgNode* bottom1 = tb1.second;
      bottom1->mNext = node;
      bottom1->mLast = true;

      const VlStmt* stmt2 = stmt->else_stmt();
      pair<CfgNode*, CfgNode*> tb2 = add_stmt(stmt2);
      node->mElse = tb2.first;
      CfgNode* bottom2 = tb2.second;
      bottom2->mNext = node;
      bottom2->mLast = true;
      return make_pair(node, node);
    }
    break;

  case kVpiCase:
    break;

  case kVpiBegin:
  case kVpiNamedBegin:
    {
      ymuint n = stmt->child_stmt_num();
      CfgNode* top = NULL;
      CfgNode* bottom = NULL;
      for (ymuint i = 0; i < n; ++ i) {
	const VlStmt* stmt1 = stmt->child_stmt(i);
	pair<CfgNode*, CfgNode*> tb = add_stmt(stmt1);
	if ( i == 0 ) {
	  top = tb.first;
	}
	else {
	  assert_cond( bottom != NULL, __FILE__, __LINE__);
	  bottom->mNext = tb.first;
	}
	bottom = tb.second;
      }
      return make_pair(top, bottom);
    }
    break;

  case kVpiAssignment:
  case kVpiNullStmt:
    {
      CfgNode* node = new_node(stmt);
      return make_pair(node, node);
    }
    break;

  default:
    cerr << "unexpected statement type" << endl;
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  // ダミー
  return pair<CfgNode*, CfgNode*>(NULL, NULL);
}

// @brief ノードを生成する．
// @param[in] stmt ノードに対応するステートメント
CfgNode*
CfgMgr::new_node(const VlStmt* stmt)
{
  void* p = mAlloc.get_memory(sizeof(CfgNode));
  CfgNode* node = new (p) CfgNode(stmt);
  return node;
}

END_NAMESPACE_YM_MVN
