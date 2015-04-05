
/// @file NodeSet.cc
/// @brief NodeSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "NodeSet.h"

#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
NodeSet::NodeSet()
{
}

// @brief デストラクタ
NodeSet::~NodeSet()
{
}

BEGIN_NONAMESPACE

struct Lt
{
  bool
  operator()(const TpgNode* left,
	     const TpgNode* right)
  {
    return left->output_id2() < right->output_id2();
  }
};

END_NONAMESPACE

// @brief 故障位置を与えてその TFO の TFI リストを作る．
// @param[in] max_node_id ノード番号の最大値
// @param[in] fnode_list 故障位置のノードのリスト
//
// 結果は mTfoList に格納される．
// 故障位置の TFO が mTfoList の [0: mTfoEnd1 - 1] に格納される．
void
NodeSet::mark_region(ymuint max_node_id,
		     const vector<const TpgNode*>& fnode_list)
{
  mMaxNodeId = max_node_id;

  mMarkArray.clear();
  mMarkArray.resize(max_node_id, 0U);

  mTfoList.clear();
  mTfoList.reserve(max_node_id);

  mInputList.clear();
  mOutputList.clear();

  // 故障のあるノードの TFO を mTfoList に入れる．
  // TFO の TFI のノードを mTfiList に入れる．
  ymuint nf = fnode_list.size();
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgNode* fnode = fnode_list[i];
    if ( !tfo_mark(fnode) ) {
      set_tfo_mark(fnode);
      if ( fnode->is_input() ) {
	mInputList.push_back(fnode);
      }
    }
  }

  for (ymuint rpos = 0; rpos < mTfoList.size(); ++ rpos) {
    const TpgNode* node = mTfoList[rpos];
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      const TpgNode* fonode = node->active_fanout(i);
      if ( !tfo_mark(fonode) ) {
	set_tfo_mark(fonode);
      }
    }
  }

  mTfoEnd = mTfoList.size();
  for (ymuint rpos = 0; rpos < mTfoList.size(); ++ rpos) {
    const TpgNode* node = mTfoList[rpos];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* finode = node->fanin(i);
      if ( !tfo_mark(finode) && !tfi_mark(finode) ) {
	set_tfi_mark(finode);
      }
    }
  }

  sort(mOutputList.begin(), mOutputList.end(), Lt());
}

END_NAMESPACE_YM_SATPG
