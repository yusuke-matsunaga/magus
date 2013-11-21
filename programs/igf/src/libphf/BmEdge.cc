
/// @file BmEdge.cc
/// @brief BmEdge の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "BmEdge.h"

BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス BmEdge
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] v1 V1 側のノード
// @param[in] v2 V2 側のノード
BmEdge::BmEdge(BmNode* v1,
	       BmNode* v2) :
  mV1(v1),
  mV2(v2)
{
}

// @brief デストラクタ
BmEdge::~BmEdge()
{
}

// @brief V1 側のノードを返す．
BmNode*
BmEdge::v1()
{
  return mV1;
}

// @brief V2 側のノードを返す．
BmNode*
BmEdge::v2()
{
  return mV2;
}

// @brief flow を設定する．
void
BmEdge::set_flow(ymuint flow)
{
  mFlow = flow;
}

// @brief flow を返す．
// @note 答えは 0 か 1
ymuint
BmEdge::flow()
{
  return mFlow;
}

// @brief residual を返す．
// @note 答えは 0 か 1
ymuint
BmEdge::residual()
{
  return 1 - mFlow;
}

END_NAMESPACE_YM_IGF
