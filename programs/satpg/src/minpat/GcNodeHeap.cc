
/// @file GcNodeHeap.cc
/// @brief GcNodeHeap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GcNodeHeap.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// @brief ノードの比較関数
int
compare(GcNode* node1,
	GcNode* node2)
{
  if ( node1->is_selected() && !node2->is_selected() ) {
    return -1;
  }
  if ( !node1->is_selected() && node2->is_selected() ) {
    return 1;
  }
  if ( node1->sat_degree() < node2->sat_degree() ) {
    return 1;
  }
  if ( node1->sat_degree() > node2->sat_degree() ) {
    return -1;
  }
  if ( node1->adj_degree() < node2->adj_degree() ) {
    return 1;
  }
  if ( node1->adj_degree() > node2->adj_degree() ) {
    return -1;
  }
  return 0;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GcNodeHeap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num ノード数
GcNodeHeap::GcNodeHeap(ymuint num)
{
  mHeapSize = num;
  mNodeHeap = new GcNode*[num];
  mNodeNum = 0;
}

// @brief デストラクタ
GcNodeHeap::~GcNodeHeap()
{
  delete [] mNodeHeap;
}

// @brief ノードを適当な位置まで沈める．
// @param[in] node 対象のノード
void
GcNodeHeap::move_down(GcNode* node)
{
  ymuint idx = node->heap_location();
  if ( idx == 0 ) {
    // node はヒープに含まれない．
    return;
  }

  -- idx;
  for ( ; ; ) {
    // ヒープ木の性質から親の位置から子の位置が分かる．
    ymuint l_idx = idx * 2 + 1;
    ymuint r_idx = l_idx + 1;
    if ( r_idx > mNodeNum ) {
      // 左右の子供を持たない時
      break;
    }
    GcNode* p_node = mNodeHeap[idx];
    GcNode* l_node = mNodeHeap[l_idx];
    if ( r_idx == mNodeNum ) {
      // 右の子供を持たない時
      if ( compare(p_node, l_node) > 0 ) {
	// 逆転
	locate_node(p_node, l_idx);
	locate_node(l_node, idx);
      }
      // これ以上子供はいない．
      break;
    }
    else {
      // 左右の子供がいる場合
      GcNode* r_node = mNodeHeap[r_idx];
      if ( compare(p_node, l_node) > 0 &&
	   compare(l_node, r_node) <= 0 ) {
	// 左の子供と入れ替える．
	// 次は左の子供に対して同じ事をする．
	locate_node(p_node, l_idx);
	locate_node(l_node, idx);
	idx = l_idx;
      }
      else if ( compare(p_node, r_node) > 0 &&
		compare(r_node, l_node) < 0 ) {
	// 右の子供と入れ替える．
	// 次は右の子供に対して同じ事をする．
	locate_node(p_node, r_idx);
	locate_node(r_node, idx);
	idx = r_idx;
      }
      else {
	break;
      }
    }
  }
}

// @brief ノードを適当な位置まで浮かび上がらせる．
// @param[in] node 対象のノード
void
GcNodeHeap::move_up(GcNode* node)
{
  ymuint idx = node->heap_location();
  if ( idx == 0 ) {
    // node はヒープに含まれない．
    return;
  }

  -- idx;
  while ( idx > 0 ) {
    GcNode* node = mNodeHeap[idx];
    ymuint p_idx = (idx - 1) / 2;
    GcNode* p_node = mNodeHeap[p_idx];
    if ( compare(p_node, node) > 0 ) {
      locate_node(node, p_idx);
      locate_node(p_node, idx);
      idx = p_idx;
    }
    else {
      break;
    }
  }
}

// @brief 内容を出力する．
void
GcNodeHeap::print(ostream& s) const
{
  s << " heap_size = " << mHeapSize << endl;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    const GcNode* node1 = mNodeHeap[i];

    ASSERT_COND( node1->heap_location() - 1 == i );

    s << " GcNode#" << i << ": id = " << node1->id()
      << " SAT = " << node1->sat_degree()
      << " ADJ = " << node1->adj_degree() << endl;
  }
  s << endl;
}

END_NAMESPACE_YM_SATPG
