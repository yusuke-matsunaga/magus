
/// @file libym_techmap/patgen/PgNode.cc
/// @brief PgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PgNode.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN


//////////////////////////////////////////////////////////////////////
// クラス PgNode
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
PgNode::PgNode() :
  mType(0U)
{
  mFanin[0] = NULL;
  mFanin[1] = NULL;
}

// @brief デストラクタ
PgNode::~PgNode()
{
}


//////////////////////////////////////////////////////////////////////
// クラス PgGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr PgNodeMgr
PgGraph::PgGraph(PgNodeMgr& mgr) :
  mMgr(mgr)
{
  mRoot = NULL;
  mRootInv = false;
}

// @brief デストラクタ
PgGraph::~PgGraph()
{
  for (vector<PgNode>::iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    mMgr.delete_node(*p);
  }
}

// @brief 入力ノードを作る．
PgNode*
PgGraph::new_input()
{
  ymuint id = mInputList.size();
  PgNode* node = mMgr.new_input(id);
  mInputList.push_back(node);
  mNodeList.push_back(node);
  return node;
}

// @brief AND ノードを作る．
// @param[in] fanin0, fanin1 ファンインのノード
// @param[in] inv0, inv1 ファンインの反転属性
PgNode*
PgGraph::new_and(PgNode* fanin0,
		 PgNode* fanin1,
		 bool inv0,
		 bool inv1)
{
  PgNode* node = mMgr.new_and(fanin0, fanin1, inv0, inv1);
  mNodeList.push_back(node);
  return node;
}

// @brief XOR ノードを作る．
// @param[in] fanin0, fanin1 ファンインのノード
PgNode*
PgGraph::new_xor(PgNode* fanin0,
		 PgNode* fanin1)
{
  PgNode* node = mMgr.new_xor(fanin0, fanin1);
  mNodeList.push_back(node);
  return node;
}

// @brief 根のノードを設定する．
// @param[in] node 根のノード
// @param[in] inv 反転属性
void
PgGraph::set_root(PgNode* node,
		  bool inv)
{
  mRoot = node;
  mRootInv = inv;
}


//////////////////////////////////////////////////////////////////////
// クラス PgNodeMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PgNodeMgr::PgNodeMgr() :
  mAlloc(sizeof(PgNode), 1024)
{
}

// @brief デストラクタ
PgNodeMgr::~PgNodeMgr()
{
}

// @brief 入力ノードを作る．
// @param[in] id 入力番号
PgNode*
PgNodeMgr::new_input(ymuint id)
{
  PgNode* node = new_node();
  node->mType = (id << 2);
  return node;
}

// @brief AND ノードを作る．
// @param[in] fanin0, fanin1 ファンインのノード
// @param[in] inv0, inv1 ファンインの反転属性
PgNode*
PgNodeMgr::new_and(PgNode* fanin0,
		   PgNode* fanin1,
		   bool inv0,
		   bool inv1)
{
  PgNode* node = new_node();
  node->mType = 2U;
  node->mFanin[0] = fanin0;
  node->mFanin[1] = fanin1;
  if ( inv0 ) {
    node->mType |= 4U;
  }
  if ( inv1 ) {
    node->mType |= 8U;
  }
  return node;
}

// @brief XOR ノードを作る．
// @param[in] fanin0, fanin1 ファンインのノード
PgNode*
PgNodeMgr::new_xor(PgNode* fanin0,
		   PgNode* fanin1)
{
  PgNode* node = new_node();
  node->mType = 3U;
  node->mFanin[0] = fanin0;
  node->mFanin[1] = fanin1;
  return node;
}

// @brief ノードを作る．
PgNode*
PgNodeMgr::new_node()
{
  void* p = mAlloc.get_memory(sizeof(PgNode));
  return new (p) PgNode();
}

// @brief ノードを削除する．
void
PgNodeMgr::delete_node(PgNode* node)
{
  node->~PgNode();
  mAlloc.put_memory(sizeof(PgNode), node);
}

END_NAMESPACE_YM_TECHMAP_PATGEN
