
/// @file BmNode.cc
/// @brief BmNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "BmNode.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス BmNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BmNode::BmNode()
{
}

// @brief デストラクタ
BmNode::~BmNode()
{
}

// @brief 内容をセットする．
// @param[in] type 種類 ( 0 or 1 )
// @param[in] id ID番号
void
BmNode::set(ymuint type,
	    ymuint id)
{
  mIdType = (id << 1) | (type & 1U);
}

// @brief 種類を返す．
ymuint
BmNode::type() const
{
  return mIdType & 1U;
}

// @brief ID番号を返す．
ymuint
BmNode::id() const
{
  return mIdType >> 1;
}

// @brief 接続している枝を返す．
ymuint
BmNode::edge_num() const
{
  return mEdgeList.size();
}

// @brief 枝を返す．
// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
BmEdge*
BmNode::edge(ymuint pos)
{
  assert_cond( pos < mEdgeList.size(), __FILE__, __LINE__);
  return mEdgeList[pos];
}

END_NAMESPACE_YM_IGF
