
/// @file BtgHeapTree.cc
/// @brief BtgHeapTree の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtgHeapTree.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス BtgHeapTree
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_size 予想される最大要素数
BtgHeapTree::BtgHeapTree(ymuint max_size)
{
  mMaxSize = max_size;
  mNum = 0;
  mArray = new BtgNode*[mMaxSize];
}

// @brief デストラクタ
BtgHeapTree::~BtgHeapTree()
{
  delete [] mArray;
}

// @brief 要素を適当な位置まで沈めていく関数
// @param[in] node 対象の要素
void
BtgHeapTree::move_down(BtgNode* node)
{
  ymuint pos = node->mPos;
  if ( pos == 0 ) {
    // node はヒープに含まれない．
    return;
  }

  -- pos;
  for ( ; ; ) {
    // ヒープ木の性質から親の位置から子の位置が分かる．
    ymuint l_pos = pos * 2 + 1;
    ymuint r_pos = l_pos + 1;
    if ( r_pos > mNum ) {
      // 左右の子供を持たない時
      break;
    }
    BtgNode* p_node = mArray[pos];
    BtgNode* l_node = mArray[l_pos];
    if ( r_pos == mNum ) {
      // 右の子供を持たない時
      if ( compare(p_node, l_node) > 0 ) {
	// 逆転
	set(l_pos, p_node);
	set(pos, l_node);
	// これ以上子供はいない．
	break;
      }
    }
    else {
      // 左右の子供がいる場合
      BtgNode* r_node = mArray[r_pos];
      if ( compare(p_node, l_node) > 0 &&
	   compare(l_node, r_node) <= 0 ) {
	// 左の子供と入れ替える．
	// 次は左の子供に対して同じ事をする．
	set(l_pos, p_node);
	set(pos, l_node);
	pos = l_pos;
      }
      else if ( compare(p_node, r_node) > 0 &&
		compare(r_node, l_node) < 0 ) {
	// 右の子供と入れ替える．
	// 次は右の子供に対して同じ事をする．
	set(r_pos, p_node);
	set(pos, r_node);
	pos = r_pos;
      }
      else {
	break;
      }
    }
  }
}

// @brief 要素を適当な位置まで浮かび上がらせる関数
// @param[in] node 対象の要素
void
BtgHeapTree::move_up(BtgNode* node)
{
  ymuint pos = node->mPos;
  if ( pos == 0 ) {
    // node はヒープに含まれない．
    return;
  }

  -- pos;
  while ( pos > 0 ) {
    BtgNode* node = mArray[pos];
    ymuint p_pos = (pos - 1) / 2;
    BtgNode* p_node = mArray[p_pos];
    if ( compare(p_node, node) > 0 ) {
      set(p_pos, node);
      set(pos, p_node);
      pos = p_pos;
    }
    else {
      break;
    }
  }
}

END_NAMESPACE_YM
