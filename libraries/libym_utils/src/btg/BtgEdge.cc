
/// @file BtgEdge.cc
/// @brief BtgEdge の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtgEdge.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス BtgEdge
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] v1 V1 側のノード
// @param[in] v2 V2 側のノード
BtgEdge::BtgEdge(BtgNode* v1,
		 BtgNode* v2) :
  mV1(v1),
  mV2(v2)
{
}

// @brief デストラクタ
BtgEdge::~BtgEdge()
{
}

// @brief V1 側のノードを返す．
BtgNode*
BtgEdge::v1()
{
  return mV1;
}

// @brief V2 側のノードを返す．
BtgNode*
BtgEdge::v2()
{
  return mV2;
}

// @brief flow を設定する．
void
BtgEdge::set_flow(ymuint flow)
{
  mFlow = flow;
}

// @brief flow を返す．
// @note 答えは 0 か 1
ymuint
BtgEdge::flow()
{
  return mFlow;
}

// @brief residual を返す．
// @note 答えは 0 か 1
ymuint
BtgEdge::residual()
{
  return 1 - mFlow;
}

END_NAMESPACE_YM_IGF
