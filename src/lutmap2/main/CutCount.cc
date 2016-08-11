
/// @file CutCount.cc
/// @brief CutCount の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "CutCount.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_YM_LUTMAP2

//////////////////////////////////////////////////////////////////////
// クラス CutCount
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CutCount::CutCount()
{
}

// @brief デストラクタ
CutCount::~CutCount()
{
}

// @brief 理論上の下界を計算する．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] limit カットサイズ
double
CutCount::lower_bound(const SbjGraph& sbjgraph,
		      ymuint limit)
{
  enum_cut(sbjgraph, limit);

  double ans = 0.0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    ymuint n = mCutSizeArray[i];
    if ( n == 0 ) {
      continue;
    }
    double l = 1.0 / n;
    ans += l;
  }

  return ans;
}

// @brief 処理の最初に呼ばれる関数
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
void
CutCount::all_init(const SbjGraph& sbjgraph,
		   ymuint limit)
{
  mNodeNum = sbjgraph.max_node_id();
  mCutSizeArray.clear();
  mCutSizeArray.resize(mNodeNum, 0);

  mMark.clear();
  mMark.resize(mNodeNum, false);
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
// @param[in] node 根のノード
// @param[in] cur_pos node の処理順
void
CutCount::node_init(const SbjNode* node,
		    ymuint cur_pos)
{
  mCutSizeArray[node->id()] = 0;
}

// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
// @param[in] root 根のノード
void
CutCount::found(const SbjNode* root)
{
  // なにもしない．
}

// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
// @param[in] root 根のノード
// @param[in] ni 入力数
// @param[in] inputs 入力ノードの配列
void
CutCount::found(const SbjNode* root,
		ymuint ni,
		const SbjNode* inputs[])
{
  // このカットがカバーするノードを求める．
  // ただし inputs[] のノードは含まない．
  // root は含む．
  vector<const SbjNode*> node_list;
  node_list.push_back(root);
  mMark[root->id()] = true;
  // inputs[] のノードに印をつけておく．
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = inputs[i];
    mMark[node->id()] = true;
  }
  for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
    const SbjNode* node = node_list[rpos];
    ASSERT_COND( node->is_logic() );

    const SbjNode* inode0 = node->fanin(0);
    if ( !mMark[inode0->id()] ) {
      mMark[inode0->id()] = true;
      node_list.push_back(inode0);
    }

    const SbjNode* inode1 = node->fanin(1);
    if ( !mMark[inode1->id()] ) {
      mMark[inode1->id()] = true;
      node_list.push_back(inode1);
    }
  }
  // このノード数をカットのサイズとする．
  ymuint cut_size = node_list.size();
  for (ymuint i = 0; i < node_list.size(); ++ i) {
    // 各ノードの最大カットサイズを記録する．
    const SbjNode* node = node_list[i];
    if ( mCutSizeArray[node->id()] < cut_size ) {
      mCutSizeArray[node->id()] = cut_size;
    }
  }
  // マークを消しておく．
  for (ymuint i = 0; i < node_list.size(); ++ i) {
    const SbjNode* node = node_list[i];
    mMark[node->id()] = false;
  }
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = inputs[i];
    mMark[node->id()] = false;
  }
}

// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
// @param[in] node 根のノード
// @param[in] cur_pos node の処理順
// @param[in] ncuts 列挙されたカット数
void
CutCount::node_end(const SbjNode* node,
		   ymuint cur_pos,
		   ymuint ncuts)
{
}

// @brief 処理の最後に呼ばれる関数
void
CutCount::all_end(const SbjGraph& sbjgraph,
		  ymuint limit)
{
}

END_NAMESPACE_YM_LUTMAP2
