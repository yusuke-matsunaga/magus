
/// @file RandSampler.cc
/// @brief RandSampler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/RandSampler.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
// @param[in] weight_array 重みの配列
RandSampler::RandSampler(const vector<ymuint>& weight_array)
{
  mNum = static_cast<ymuint>(weight_array.size());
  mWeightArray = new ymuint[mNum];
  for (ymuint i = 0; i < mNum; ++ i) {
    mWeightArray[i] = weight_array[i];
  }
  init();
}

// @brief コンストラクタ
// @param[in] num 要素数
// @param[in] weight_array 重みの配列
RandSampler::RandSampler(ymuint num,
			 ymuint weight_array[])
{
  mNum = num;
  mWeightArray = new ymuint[mNum];
  for (ymuint i = 0; i < mNum; ++ i) {
    mWeightArray[i] = weight_array[i];
  }
  init();
}

// @brief コンストラクタの共通処理
void
RandSampler::init()
{
  mNodeArray = new RsNode[mNum * 2 - 1];
  RsNode** node_heap = new RsNode*[mNum];
  ymuint heap_end = 0;
  for (ymuint i = 0; i < mNum; ++ i) {
    RsNode* node = &mNodeArray[i];
    node->mIndex = i;
    node->mWeight = mWeightArray[i];
    node->mLeft = NULL;
    node->mRight = NULL;
    node_heap[heap_end] = node;
    move_up(node_heap, heap_end);
    ++ heap_end;
  }

  ymuint next_pos = mNum;
  while ( heap_end > 1 ) {
    RsNode* node1 = node_heap[0];
    -- heap_end;
    node_heap[0] = node_heap[heap_end];
    move_down(node_heap, heap_end, 0);
    RsNode* node2 = node_heap[0];

    RsNode* new_node = &mNodeArray[next_pos];
    new_node->mIndex = next_pos;
    ++ next_pos;
    new_node->mWeight = node1->mWeight + node2->mWeight;
    new_node->mLeft = node1;
    new_node->mRight = node2;
    node_heap[0] = new_node;
    move_down(node_heap, heap_end, 0);
  }

  mRoot = node_heap[0];

  delete [] node_heap;
}

// @brief デストラクタ
RandSampler::~RandSampler()
{
  delete [] mWeightArray;
  delete [] mNodeArray;
}

// @brief 要素数を返す．
ymuint
RandSampler::num() const
{
  return mNum;
}

// @brief 要素の重みを返す．
// @param[in] pos 位置番号 ( 0 <= pos < num() )
ymuint
RandSampler::weight(ymuint pos) const
{
  ASSERT_COND( pos < num() );

  return mWeightArray[pos];
}

// @brief サンプリングを行う．
// @param[in] randgen 乱数発生器
// @return サンプリング結果を返す．
ymuint
RandSampler::get_sample(RandGen& randgen)
{
  ASSERT_COND( mRoot != NULL );

  ymuint total_weight = mRoot->mWeight;
  ymuint val = randgen.int32() % total_weight;

  for (RsNode* node = mRoot; ; ) {
    RsNode* l_node = node->mLeft;
    if ( l_node == NULL ) {
      return node->mIndex;
    }
    if ( l_node->mWeight > val ) {
      node = l_node;
    }
    else {
      val -= l_node->mWeight;
      node = node->mRight;
    }
  }
}

// @brief 要素を上に移動する．
// @param[in] heap ヒープ木の配列
// @param[in] pos 対象の位置
void
RandSampler::move_up(RsNode** heap,
		     ymuint pos)
{
  while ( pos > 0 ) {
    RsNode* node = heap[pos];
    ymuint p_pos = (pos - 1) >> 1;
    RsNode* p_node = heap[p_pos];
    if ( p_node->mWeight > node->mWeight ) {
      heap[p_pos] = node;
      heap[pos] = p_node;
      pos = p_pos;
    }
    else {
      break;
    }
  }
}

// @brief 要素を下に移動する．
// @param[in] heap ヒープ木の配列
// @param[in] heap_size ヒープ木のサイズ
// @param[in] pos 対象の位置
void
RandSampler::move_down(RsNode** heap,
		       ymuint heap_size,
		       ymuint pos)
{
  ymuint p_pos = pos;
  for ( ; ; ) {
    ymuint l_pos = p_pos + p_pos + 1;
    ymuint r_pos = l_pos + 1;
    if ( r_pos > heap_size ) {
      // 左右の子供を持たない
      break;
    }
    // 少なくとも左の子供はいる．
    RsNode* p_node = heap[p_pos];
    RsNode* l_node = heap[l_pos];
    if ( r_pos == heap_size ) {
      // 右の子供を持たない．
      if ( p_node->mWeight > l_node->mWeight ) {
	// 逆転
	heap[p_pos] = l_node;
	heap[l_pos] = p_node;
      }
      break;
    }
    else {
      RsNode* r_node = heap[r_pos];
      if ( p_node->mWeight > l_node->mWeight &&
	   l_node->mWeight <= r_node->mWeight ) {
	// 左の子供と取り替える．次は左の子供で繰り返す．
	heap[p_pos] = l_node;
	heap[l_pos] = p_node;
	p_pos = l_pos;
      }
      else if ( p_node->mWeight > r_node->mWeight &&
		r_node->mWeight < l_node->mWeight ) {
	// 右の子供と取り替える．次は右の子供で繰り返す．
	heap[p_pos] = r_node;
	heap[r_pos] = p_node;
	p_pos = r_pos;
      }
      else {
	break;
      }
    }
  }
}

END_NAMESPACE_YM
