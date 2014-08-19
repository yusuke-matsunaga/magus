
/// @file BNodeMap.cc
/// @brief BNodeMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "BNodeMap.h"
#include "ImpNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス BNodeMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BNodeMap::BNodeMap()
{
}

// @brief デストラクタ
BNodeMap::~BNodeMap()
{
}

// @brief BNode 番号の最大値 + 1 を得る．
ymuint
BNodeMap::max_bnode_id() const
{
  return mBNodeMap.size();
}

// @brief BNode 番号に対応するハンドルを得る．
// @param[in] bnode_id BNode のノード番号 ( 0 <= id < max_bnode_id() )
// @note 場合によっては NULL のハンドルが返ることもある．
ImpNodeHandle
BNodeMap::bnode_handle(ymuint bnode_id) const
{
  assert_cond( bnode_id < max_bnode_id(), __FILE__, __LINE__);
  return mBNodeMap[bnode_id];
}

// @brief ImpNode に対応する BNode 番号を得る．
// @param[in] id ImpNode の番号
// @param[out] bnode_id 対応する BNode 番号
// @param[out] inv 反転属性
// @retval true 対応する BNode があった．
// @retval false 対応する BNode がなかった．
bool
BNodeMap::get_bnode(ymuint id,
		    ymuint& bnode_id,
		    bool& inv) const
{
  if ( mImpNodeMap.size() <= id ) {
    return false;
  }

  ymuint tmp = mImpNodeMap[id];
  if ( tmp == 0xFFFFFFFF ) {
    return false;
  }
  else {
    bnode_id = tmp >> 1;
    inv = static_cast<bool>(tmp & 1U);
    return true;
  }
}

// @brief 内容を空にする．
void
BNodeMap::clear()
{
  mBNodeMap.clear();
  mImpNodeMap.clear();
}

// @brief BNode 番号の最大値を設定する．
// @param[in] size BNode 番号の最大値 + 1
void
BNodeMap::set_bnode_size(ymuint size)
{
  mBNodeMap.resize(size);
}

// @brief BNode と ImpNode の対応付けを行なう．
// @param[in] bnode_id BNode 番号
// @param[in] node 対応する ImpNode
// @param[in] inv 反転属性
void
BNodeMap::bind(ymuint bnode_id,
	       ImpNode* node,
	       bool inv)
{
  assert_cond( bnode_id < max_bnode_id(), __FILE__, __LINE__);
  mBNodeMap[bnode_id] = ImpNodeHandle(node, inv);
  if ( node != NULL ) {
    ymuint id = node->id();
    if ( mImpNodeMap.size() <= id ) {
      mImpNodeMap.resize(id + 1, 0xFFFFFFFF);
      mImpNodeMap[id] = (node->id() << 1) | static_cast<ymuint>(inv);
    }
  }
}

END_NAMESPACE_YM_NETWORKS
