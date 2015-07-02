#ifndef NODEHEAP_H
#define NODEHEAP_H

/// @file NodeHeap.h
/// @brief NodeHeap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Node.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class NodeHeap NodeHeap.h "NodeHeap.h"
/// @brief Node のヒープ木
///
/// 比較関数 compare() を実装した継承クラスを作る必要がある．
//////////////////////////////////////////////////////////////////////
template <typename NodeClass,
	  typename CompFuncClass>
class NodeHeap
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size 最大の要素数
  NodeHeap(ymuint max_size);

  /// @brief デストラクタ
  ~NodeHeap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ヒープが空の時 true を返す．
  bool
  empty() const;

  /// @brief ノードを追加する．
  void
  put_node(NodeClass* node);

  /// @brief ノードを取り去る．
  void
  delete_node(NodeClass* node);

  /// @brief 値が最小の要素を取り出す．
  /// そのノードはヒープから取り除かれる．
  NodeClass*
  get_min();

  /// @brief ノードの値の変更に伴ってヒープ構造を更新する．
  /// @param[in] node 値が変更されたノード
  void
  update(NodeClass* node);

  /// @brief 内容を出力する．
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを適当な位置まで沈める．
  /// @param[in] node 対象のノード
  void
  move_down(NodeClass* node);

  /// @brief ノードを適当な位置まで浮かび上がらせる．
  /// @param[in] node 対象のノード
  void
  move_up(NodeClass* node);

  /// @brief ノードをヒープ上にセットする．
  /// @param[in] node ノード
  /// @param[in] pos 位置
  void
  locate_node(NodeClass* node,
	      ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヒープサイズ
  ymuint32 mHeapSize;

  // ノードのヒープ木
  NodeClass** mNodeHeap;

  // ヒープ木中にあるノード数
  ymuint32 mNodeNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num ノード数
template <typename NodeClass,
	  typename CompFuncClass>
inline
NodeHeap<NodeClass, CompFuncClass>::NodeHeap(ymuint num)
{
  mHeapSize = num;
  mNodeHeap = new NodeClass*[num];
  mNodeNum = 0;
}

// @brief デストラクタ
template <typename NodeClass,
	  typename CompFuncClass>
inline
NodeHeap<NodeClass, CompFuncClass>::~NodeHeap()
{
  delete [] mNodeHeap;
}

// @brief ヒープが空の時 true を返す．
template <typename NodeClass,
	  typename CompFuncClass>
inline
bool
NodeHeap<NodeClass, CompFuncClass>::empty() const
{
  return mNodeNum == 0;
}

// @brief ノードを追加する．
template <typename NodeClass,
	  typename CompFuncClass>
inline
void
NodeHeap<NodeClass, CompFuncClass>::put_node(NodeClass* node)
{
  ASSERT_COND( mNodeNum < mHeapSize );

  locate_node(node, mNodeNum);
  ++ mNodeNum;
  move_up(node);
}

// @brief ノードを取り去る．
template <typename NodeClass,
	  typename CompFuncClass>
inline
void
NodeHeap<NodeClass, CompFuncClass>::delete_node(NodeClass* node)
{
  ASSERT_COND( !empty() );

  ymuint idx = node->heap_location();

  ASSERT_COND( idx > 0 );

  node->set_heap_location(0);
  -- mNodeNum;
  NodeClass* last = mNodeHeap[mNodeNum];
  if ( last != node ) {
    -- idx;
    locate_node(last, idx);
    move_down(last);
  }
}

// @brief 値が最小の要素を取り出す．
// そのノードはヒープから取り除かれる．
template <typename NodeClass,
	  typename CompFuncClass>
inline
NodeClass*
NodeHeap<NodeClass, CompFuncClass>::get_min()
{
  ASSERT_COND( !empty() );

  NodeClass* node = mNodeHeap[0];
  node->set_heap_location(0);
  -- mNodeNum;
  if ( mNodeNum > 0 ) {
    NodeClass* last = mNodeHeap[mNodeNum];
    locate_node(last, 0);
    move_down(last);
  }
  return node;
}

// @brief ノードの値の変更に伴ってヒープ構造を更新する．
// @param[in] node 値が変更されたノード
template <typename NodeClass,
	  typename CompFuncClass>
inline
void
NodeHeap<NodeClass, CompFuncClass>::update(NodeClass* node)
{
  ymuint idx = node->heap_location();
  ASSERT_COND( idx > 0 );

  -- idx;
  ASSERT_COND( mNodeHeap[idx] == node );

  move_up(node);
  move_down(node);
}

// @brief ノードを適当な位置まで沈める．
// @param[in] node 対象のノード
template <typename NodeClass,
	  typename CompFuncClass>
inline
void
NodeHeap<NodeClass, CompFuncClass>::move_down(NodeClass* node)
{
  ymuint idx = node->heap_location();
  if ( idx == 0 ) {
    // node はヒープに含まれない．
    return;
  }

  CompFuncClass compare;

  -- idx;
  for ( ; ; ) {
    // ヒープ木の性質から親の位置から子の位置が分かる．
    ymuint l_idx = idx * 2 + 1;
    ymuint r_idx = l_idx + 1;
    if ( r_idx > mNodeNum ) {
      // 左右の子供を持たない時
      break;
    }
    NodeClass* p_node = mNodeHeap[idx];
    NodeClass* l_node = mNodeHeap[l_idx];
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
      NodeClass* r_node = mNodeHeap[r_idx];
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
template <typename NodeClass,
	  typename CompFuncClass>
inline
void
NodeHeap<NodeClass, CompFuncClass>::move_up(NodeClass* node)
{
  ymuint idx = node->heap_location();
  if ( idx == 0 ) {
    // node はヒープに含まれない．
    return;
  }

  CompFuncClass compare;

  -- idx;
  while ( idx > 0 ) {
    NodeClass* node = mNodeHeap[idx];
    ymuint p_idx = (idx - 1) / 2;
    NodeClass* p_node = mNodeHeap[p_idx];
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
template <typename NodeClass,
	  typename CompFuncClass>
inline
void
NodeHeap<NodeClass, CompFuncClass>::print(ostream& s) const
{
  s << " heap_size = " << mHeapSize << endl;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    const NodeClass* node1 = mNodeHeap[i];

    ASSERT_COND( node1->heap_location() - 1 == i );

    s << " Node#" << i << ": id = " << node1->id()
      << " value = " << node1->adj_num() << endl;
  }
  s << endl;
}

// @brief ヒープ上にノードを置く
// @param[in] node 要素
// @param[in] pos 位置
template <typename NodeClass,
	  typename CompFuncClass>
inline
void
NodeHeap<NodeClass, CompFuncClass>::locate_node(NodeClass* node,
						ymuint pos)
{
  mNodeHeap[pos] = node;
  node->set_heap_location(pos + 1);
}

END_NAMESPACE_YM

#endif // NODEHEAP_H
