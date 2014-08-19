
/// @file FlowNodeHeap.cc
/// @brief FlowNodeHeap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "FlowNodeHeap.h"
#include "FlowNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FlowNodeHeap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FlowNodeHeap::FlowNodeHeap()
{
}

// @brief デストラクタ
FlowNodeHeap::~FlowNodeHeap()
{
}

// @brief 空にする．
void
FlowNodeHeap::clear()
{
  mArray.clear();
}

// @brief 空の時に true を返す．
bool
FlowNodeHeap::empty() const
{
  return mArray.empty();
}

// @brief 要素を追加する．
void
FlowNodeHeap::put(FlowNode* node)
{
  ymuint pos = mArray.size();
  mArray.push_back(node);
  move_up(pos);
}

// @brief 最小要素を取り出す．
FlowNode*
FlowNodeHeap::getmin()
{
  ymuint n = mArray.size();

  if ( n == 0 ) {
    return NULL;
  }

  FlowNode* node = mArray[0];

  if ( n > 1 ) {
    set_pos(mArray.back(), 0);
    mArray.pop_back();
    move_down(0);
  }

  return node;
}

// @brief 要素を適切な位置に移動する．
void
FlowNodeHeap::move(ymuint pos)
{
  assert_cond( pos < mArray.size(), __FILE__, __LINE__);
  move_up(pos);
  move_down(pos);
}

// @brief 引数の位置の要素を下に移動する．
void
FlowNodeHeap::move_down(ymuint pos)
{
  ymuint p_pos = pos;
  for ( ; ; ) {
    ymuint l_pos = p_pos + p_pos + 1;
    ymuint r_pos = l_pos + 1;
    if ( r_pos > mArray.size() ) {
      // 左右の子供をどちらも持たない場合
      break;
    }
    FlowNode* p_node = mArray[p_pos];
    FlowNode* l_node = mArray[l_pos];
    if ( r_pos == mArray.size() ) {
      // 右の子供を持たない場合
      if ( p_node->distance() > l_node->distance() ) {
	set_pos(l_node, p_pos);
	set_pos(p_node, l_pos);
      }
      break;
    }
    else {
      FlowNode* r_node = mArray[r_pos];
      if ( p_node->distance() > l_node->distance() &&
	   l_node->distance() <= r_node->distance() ) {
	// 左の子供と取り替える．次は左の子供で同じ処理をする．
	set_pos(l_node, p_pos);
	set_pos(p_node, l_pos);
	p_pos = l_pos;
      }
      else if ( p_node->distance() > r_node->distance() &&
		r_node->distance() < l_node->distance() ) {
	// 右の子供と取り替える．次は左の子供で同じ処理をする．
	set_pos(r_node, p_pos);
	set_pos(p_node, r_pos);
	p_pos = r_pos;
      }
      else {
	break;
      }
    }
  }
}

// @brief 引数の位置の要素を上に移動する．
void
FlowNodeHeap::move_up(ymuint pos)
{
  while ( pos > 0 ) {
    FlowNode* node = mArray[pos];
    ymuint p_pos = (pos - 1) >> 1;
    FlowNode* p_node = mArray[p_pos];
    if ( p_node->distance() > node->distance() ) {
      set_pos(node, p_pos);
      set_pos(p_node, pos);
    }
    else {
      break;
    }
  }
}

// @brief 配列にセットする．
void
FlowNodeHeap::set_pos(FlowNode* node,
		      ymuint pos)
{
  mArray[pos] = node;
  node->mPos = pos;
}

END_NAMESPACE_YM
