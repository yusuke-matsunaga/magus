
/// @file lutmap/CrHeap.cc
/// @brief CrHeap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CrHeap.h"
#include "CrNode.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス CrHeap
//////////////////////////////////////////////////////////////////////

// ヒープを初期化する．
void
CrHeap::init(
  SizeType max_n
)
{
  mHeap.resize(max_n);
  mNum = 0;
}

// ヒープに追加する．
void
CrHeap::put(
  CrNode* node
)
{
  set(mNum, node);
  ++ mNum;
  move_up(node->heap_index());
}

// ヒープの先頭要素を抜き出す．
CrNode*
CrHeap::get()
{
  if ( mNum == 0 ) {
    return nullptr;
  }

  auto node = get(0);
  remove(node);
  return node;
}

// ヒープから削除する．
void
CrHeap::remove(
  CrNode* node
)
{
  if ( node->in_heap() ) {
    SizeType p0 = node->heap_index();
    node->set_heap_index(-1);
    -- mNum;
    if ( mNum > p0 ) {
      auto node1 = get(mNum);
      set(p0, node1);
      move_down(p0);
      move_up(p0);
    }
  }
}

// 値の更新に伴ってヒープの再構築を行う．
void
CrHeap::update(
  CrNode* node,
  SizeType new_gain
)
{
  node->mCurGain = new_gain;
  if ( node->in_heap() ) {
    SizeType pos = node->heap_index();
    move_down(pos);
    move_up(pos);
  }
}

// 引数の位置にある要素を適当な位置まで沈めてゆく
void
CrHeap::move_down(
  SizeType pos
)
{
  for ( ; ; ) {
    // ヒープ木の性質から親から子の位置がわかる
    SizeType left = pos + pos + 1;
    SizeType right = left + 1;
    if ( right > mNum ) {
      // 左右の子どもを持たない場合
      break;
    }
    auto node_p = get(pos);
    auto node_l = get(left);
    if ( right == mNum ) {
      // 右の子どもを持たない場合
      if ( compare(node_l, node_p) ) {
	// 逆転
	set(left, node_p);
	set(pos, node_l);
      }
      break;
    }

    // 左右の子供を持つ場合
    auto node_r = get(right);
    if ( compare(node_l, node_r) ) {
      if ( compare(node_l, node_p) ) {
	// 左の子どもと取り替える．次は左の子で同じ処理をする
	set(left, node_p);
	set(pos, node_l);
	pos = left;
      }
      else {
	break;
      }
    }
    else {
      if ( compare(node_r, node_p) ) {
	// 右の子どもと取り替える．次は右の子で同じ処理をする
	set(right, node_p);
	set(pos, node_r);
	pos = right;
      }
      else {
	break;
      }
    }
  }
}

// 引数の位置にある要素を適当な位置まで上げてゆく
void
CrHeap::move_up(
  SizeType pos
)
{
  while ( pos > 0 ) {
    auto node = get(pos);
    SizeType parent = (pos - 1) >> 1;
    auto node_p = get(parent);
    if ( compare(node, node_p) ) {
      set(parent, node);
      set(pos, node_p);
      pos = parent;
    }
    else {
      break;
    }
  }
}

// pos の位置に node を置く
void
CrHeap::set(
  SizeType pos,
  CrNode* node
)
{
  mHeap[pos] = node;
  node->set_heap_index(pos);
}

// pos の位置の要素を返す．
CrNode*
CrHeap::get(
  SizeType pos
)
{
  return mHeap[pos];
}

END_NAMESPACE_LUTMAP
