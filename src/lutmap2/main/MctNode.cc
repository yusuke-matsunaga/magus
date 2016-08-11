
/// @file MctNode.cc
/// @brief MctNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MctNode.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_YM_LUTMAP2

//////////////////////////////////////////////////////////////////////
// クラス MctNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のノード
// @param[in] root 選択されたカットの根のノード
// @param[in] cand_list カット候補のリスト
MctNode::MctNode(MctNode* parent,
		 const SbjNode* root,
		 const vector<const SbjNode*>& cand_list) :
  mParent(parent),
  mRoot(root)
{
  for (ymuint i = 0; i < cand_list.size(); ++ i) {
    const SbjNode* node = cand_list[i];
    if ( root == nullptr || node->id() > root->id() ) {
      mUnexpandedList.push_back(node);
    }
  }
  mNextPos = 0;

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
const SbjNode*
MctNode::expand_child()
{
  if ( is_expanded() ) {
    return nullptr;
  }
  const SbjNode* root = mUnexpandedList[mNextPos];
  ++ mNextPos;

  return root;
}

// @brief 子供ノードを並び替える．
// @param[in] n_all_ln トータルの試行回数の ln
void
MctNode::reorder(double n_all_ln)
{
  ymuint n = mChildList.size();
  for (ymuint i = n - 1; i > 0; -- i) {
    MctNode* node1 = mChildList[i];
    double val1 = node1->UCB1(n_all_ln);
    for (ymuint j = 1; j <= i; ++ j) {
      ymuint idx = i - j;
      MctNode* node2 = mChildList[i - j];
      double val2 = node2->UCB1(n_all_ln);
      if ( val2 > val1 ) {
	mChildList[idx + 1] = node2;
	mChildList[idx] = node1;
      }
      else {
	break;
      }
    }
  }
  if ( false ) {
    cout << "reorder" << endl;
    for (ymuint i = 0; i < n; ++ i) {
      MctNode* node = mChildList[n - i - 1];
      cout << " " << node->UCB1(n_all_ln)
	   << "(" << node->mMean << ":" << node->mNum << ")";
    }
    cout << endl;
  }
}

END_NAMESPACE_YM_LUTMAP2
