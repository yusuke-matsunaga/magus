
/// @file McNodeHeap.cc
/// @brief McNodeHeap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "McNodeHeap.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス McNodeHeap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num ノード数
McNodeHeap::McNodeHeap(ymuint num)
{
  mNodeSize = num;
  mNodeChunk = new McNode[num];
  mNodeHeap = new McNode*[num];
  mNodeNum = 0;
}

// @brief デストラクタ
McNodeHeap::~McNodeHeap()
{
  delete [] mNodeChunk;
  delete [] mNodeHeap;
}

// @brief ノードを適当な位置まで沈める．
// @param[in] node 対象のノード
void
McNodeHeap::move_down(McNode* node)
{
  ymuint idx = node->mHeapIdx;
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
    McNode* p_node = mNodeHeap[idx];
    McNode* l_node = mNodeHeap[l_idx];
    if ( r_idx == mNodeNum ) {
      // 右の子供を持たない時
      if ( compare(p_node, l_node) > 0 ) {
	// 逆転
	set(l_idx, p_node);
	set(idx, l_node);
      }
      // これ以上子供はいない．
      break;
    }
    else {
      // 左右の子供がいる場合
      McNode* r_node = mNodeHeap[r_idx];
      if ( compare(p_node, l_node) > 0 &&
	   compare(l_node, r_node) <= 0 ) {
	// 左の子供と入れ替える．
	// 次は左の子供に対して同じ事をする．
	set(l_idx, p_node);
	set(idx, l_node);
	idx = l_idx;
      }
      else if ( compare(p_node, r_node) > 0 &&
		compare(r_node, l_node) < 0 ) {
	// 右の子供と入れ替える．
	// 次は右の子供に対して同じ事をする．
	set(r_idx, p_node);
	set(idx, r_node);
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
McNodeHeap::move_up(McNode* node)
{
  ymuint idx = node->mHeapIdx;
  if ( idx == 0 ) {
    // node はヒープに含まれない．
    return;
  }

  -- idx;
  while ( idx > 0 ) {
    McNode* node = mNodeHeap[idx];
    ymuint p_idx = (idx - 1) / 2;
    McNode* p_node = mNodeHeap[p_idx];
    if ( compare(p_node, node) > 0 ) {
      set(p_idx, node);
      set(idx, p_node);
      idx = p_idx;
    }
    else {
      break;
    }
  }
}

// @brief 内容を出力する．
void
McNodeHeap::print(ostream& s)
{
  s << "*** McNodeHeap ***" << endl
    << " node_size() = " << node_size() << endl;
  for (ymuint i = 0; i < node_size(); ++ i) {
    McNode* node1 = node(i);
    s << "Node#" << i << ": id = " << node1->id();
    if ( node1->deleted() ) {
      s << "*";
    }
    else {
      s << " ";
    }
    s << " adj_num = " << node1->adj_num() << endl;
  }
  s << endl;

  s << " heap_size = " << mNodeNum << endl;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    McNode* node1 = mNodeHeap[i];

    ASSERT_COND( node1->mHeapIdx - 1 == i );

    s << " Node#" << i << ": id = " << node1->id()
      << " value = " << node1->adj_num() << endl;
  }
  s << endl;
}

END_NAMESPACE_YM
