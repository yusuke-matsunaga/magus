
/// @file BtgNode.cc
/// @brief BtgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtgNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス BtgNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BtgNode::BtgNode()
{
  mCurEdge = NULL;
}

// @brief デストラクタ
BtgNode::~BtgNode()
{
}

// @brief 内容をセットする．
// @param[in] type 種類 ( 0 or 1 )
// @param[in] id ID番号
void
BtgNode::set(ymuint type,
	     ymuint id)
{
  mIdType = (id << 1) | (type & 1U);
}

// @brief 種類を返す．
ymuint
BtgNode::type() const
{
  return mIdType & 1U;
}

// @brief ID番号を返す．
ymuint
BtgNode::id() const
{
  return mIdType >> 1;
}

// @brief 接続している枝を返す．
ymuint
BtgNode::edge_num() const
{
  return mEdgeList.size();
}

// @brief 枝を返す．
// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
BtgEdge*
BtgNode::edge(ymuint pos)
{
  assert_cond( pos < mEdgeList.size(), __FILE__, __LINE__);
  return mEdgeList[pos];
}

END_NAMESPACE_YM_IGF
