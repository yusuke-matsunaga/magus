
/// @file libym_techmap/patgen/PgNode.cc
/// @brief PgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PgNode.h"
#include "PgNodeMgr.h"


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
  while ( mInputList.size() <= id ) {
    mInputList.push_back(NULL);
  }
  PgNode* node = mInputList[id];
  if ( node == NULL ) {
    node = new_node();
    node->mType = (id << 2);
  }
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
  PgNode* node = new (p) PgNode();
  node->mId = mNodeList.size();
  mNodeList.push_back(node);
  return node;
}

// @brief ノードを削除する．
void
PgNodeMgr::delete_node(PgNode* node)
{
  node->~PgNode();
  mAlloc.put_memory(sizeof(PgNode), node);
}

END_NAMESPACE_YM_TECHMAP_PATGEN
