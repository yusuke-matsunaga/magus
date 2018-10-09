
/// @file MctNode.cc
/// @brief MctNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MctNode.h"


BEGIN_NAMESPACE_LUTMAP_MCT2

//////////////////////////////////////////////////////////////////////
// クラス MctNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のノード
// @param[in] index ファンアウトノードのインデックス
// @param[in] select 境界ノードとして選ぶ時 true にするフラグ
MctNode::MctNode(MctNode* parent,
		 ymuint index,
		 bool select) :
  mParent(parent),
  mIndex(index),
  mSelected(select)
{
  mUnexpandedNum = 2;

  mSum = 0.0;
  mNum = 0;
  mMean = 0.0;
}

// @brief デストラクタ
MctNode::~MctNode()
{
}

// @brief 未展開の子供を取り出す．
//
// is_expanded() == false のときのみ意味を持つ．
// 取り出されたノードは展開済みとなる．
MctNode*
MctNode::expand_child()
{
  if ( is_expanded() ) {
    return nullptr;
  }
  bool select = (mUnexpandedNum == 1);
  MctNode* child = new MctNode(this, mIndex + 1, select);
  if ( mUnexpandedNum == 2 ) {
    mChildList[0] = child;
  }
  else if ( mUnexpandedNum == 1 ) {
    MctNode* child0 = mChildList[0];
    mChildList[0] = child;
    mChildList[1] = child0;
  }
  -- mUnexpandedNum;

  return child;
}

// @brief 子供ノードを並び替える．
// @param[in] n_all_ln トータルの試行回数の ln
// @param[in] cp 調整パラメータ
void
MctNode::reorder(double n_all_ln,
		 double cp)
{
  if ( mUnexpandedNum == 0 ) {
    MctNode* node0 = mChildList[0];
    MctNode* node1 = mChildList[1];
    double val0 = node0->UCB1(n_all_ln, cp);
    double val1 = node1->UCB1(n_all_ln, cp);
    if ( val0 < val1 ) {
      mChildList[0] = node1;
      mChildList[1] = node0;
    }
  }
}

END_NAMESPACE_LUTMAP_MCT2
