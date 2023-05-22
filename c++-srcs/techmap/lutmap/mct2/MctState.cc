
/// @file MctState.cc
/// @brief MctState の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "mct2/MctState.h"
#include "SbjGraph.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_LUTMAP_MCT2

//////////////////////////////////////////////////////////////////////
// クラス MctState
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sbjgraph サブジェクトグラフ
MctState::MctState(const SbjGraph& sbjgraph)
{
  for (int i = 0; i < sbjgraph.output_num(); ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    mOutputList.push_back(node);
  }
}

// @brief デストラクタ
MctState::~MctState()
{
}

// @brief 初期化する．
void
MctState::init()
{
  mBoundaryList.clear();

  for (int i = 0; i < mOutputList.size(); ++ i) {
    const SbjNode* node = mOutputList[i];
    add_boundary(node);
  }

  mIndex = 0;
}

END_NAMESPACE_LUTMAP_MCT2
