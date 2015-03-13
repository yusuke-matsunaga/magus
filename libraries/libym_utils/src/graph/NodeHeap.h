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
class NodeHeap
{
public:

  /// @brief コンストラクタ
  /// @param[in] num ノード数
  NodeHeap(ymuint num);

  /// @brief デストラクタ
  ~NodeHeap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保されているノード数を返す．
  ymuint
  node_size() const;

  /// @brief ノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_size() )
  Node*
  node(ymuint pos);

  /// @brief ノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_size() )
  const Node*
  node(ymuint pos) const;

  /// @brief ヒープが空の時 true を返す．
  bool
  empty() const;

  /// @brief ノードを追加する．
  void
  put_node(Node* node);

  /// @brief ノードを取り去る．
  void
  delete_node(Node* node);

  /// @brief 値が最小の要素を取り出す．
  /// そのノードはヒープから取り除かれる．
  Node*
  get_min();

  /// @brief ノードの値の変更に伴ってヒープ構造を更新する．
  /// @param[in] node 値が変更されたノード
  void
  update(Node* node);

  /// @brief 内容を出力する．
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 比較関数
  /// @param[in] node1, node2 対象のノード
  /// @retval 負の値 node1 が node2 より前にある．
  /// @retval 0 node1 と node2 は同じ
  /// @retval 正の値 node1 が node2 より後ろにある．
  virtual
  int
  compare(Node* node1,
	  Node* node2) = 0;

  /// @brief ノードを適当な位置まで沈める．
  /// @param[in] node 対象のノード
  void
  move_down(Node* node);

  /// @brief ノードを適当な位置まで浮かび上がらせる．
  /// @param[in] node 対象のノード
  void
  move_up(Node* node);

  /// @brief ノードをセットする．
  /// @param[in] pos 位置
  /// @param[in] node ノード
  void
  set(ymuint pos,
      Node* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeSize;

  // ノードの実体の配列
  // サイズは mNodeSize
  Node* mNodeChunk;

  // ノードのヒープ木
  Node** mNodeHeap;

  // ヒープ木中にあるノード数
  ymuint32 mNodeNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 確保されているノード数を返す．
inline
ymuint
NodeHeap::node_size() const
{
  return mNodeSize;
}

// @brief ノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < node_size() )
inline
Node*
NodeHeap::node(ymuint pos)
{
  return &mNodeChunk[pos];
}

// @brief ノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < node_size() )
inline
const Node*
NodeHeap::node(ymuint pos) const
{
  return &mNodeChunk[pos];
}

// @brief ヒープが空の時 true を返す．
inline
bool
NodeHeap::empty() const
{
  return mNodeNum == 0;
}

// @brief ノードを追加する．
inline
void
NodeHeap::put_node(Node* node)
{
  ASSERT_COND( mNodeNum < mNodeSize );

  set(mNodeNum, node);
  ++ mNodeNum;
  move_up(node);
}

// @brief ノードを取り去る．
inline
void
NodeHeap::delete_node(Node* node)
{
  ASSERT_COND( !empty() );

  ymuint idx = node->mHeapIdx;

  ASSERT_COND( idx > 0 );

  node->mHeapIdx = 0;
  -- mNodeNum;
  Node* last = mNodeHeap[mNodeNum];
  if ( last != node ) {
    -- idx;
    set(idx, last);
    move_down(last);
  }
}

// @brief 値が最小の要素を取り出す．
// そのノードはヒープから取り除かれる．
inline
Node*
NodeHeap::get_min()
{
  ASSERT_COND( !empty() );

  Node* node = mNodeHeap[0];
  node->mHeapIdx = 0;
  -- mNodeNum;
  if ( mNodeNum > 0 ) {
    Node* last = mNodeHeap[mNodeNum];
    set(0, last);
    move_down(last);
  }
  return node;
}

// @brief ノードの値の変更に伴ってヒープ構造を更新する．
// @param[in] node 値が変更されたノード
inline
void
NodeHeap::update(Node* node)
{
  ymuint idx = node->mHeapIdx;
  ASSERT_COND( idx > 0 );

  -- idx;
  ASSERT_COND( mNodeHeap[idx] == node );

  if ( idx > 0 ) {
    ymuint p_idx = (idx - 1) / 2;
    Node* p_node = mNodeHeap[p_idx];
    if ( compare(p_node, node) > 0 ) {
      move_up(node);
      goto end;
    }
  }
  move_down(node);
 end:
  ;
}

// @brief 要素をセットする．
// @param[in] pos 位置
// @param[in] node 要素
inline
void
NodeHeap::set(ymuint pos,
	      Node* node)
{
  mNodeHeap[pos] = node;
  node->mHeapIdx = pos + 1;
}

END_NAMESPACE_YM

#endif // NODEHEAP_H
