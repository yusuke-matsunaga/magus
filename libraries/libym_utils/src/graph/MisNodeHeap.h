#ifndef MCNODEHEAP_H
#define MCNODEHEAP_H

/// @file MisNodeHeap.h
/// @brief MisNodeHeap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MisNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MisNodeHeap MisNodeHeap.h "MisNodeHeap.h"
/// @brief MisNode のヒープ木
//////////////////////////////////////////////////////////////////////
class MisNodeHeap
{
public:

  /// @brief コンストラクタ
  /// @param[in] num ノード数
  MisNodeHeap(ymuint num);

  /// @brief デストラクタ
  ~MisNodeHeap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保されているノード数を返す．
  ymuint
  node_size() const;

  /// @brief ノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_size() )
  MisNode*
  node(ymuint pos);

  /// @brief ノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_size() )
  const MisNode*
  node(ymuint pos) const;

  /// @brief ヒープが空の時 true を返す．
  bool
  empty() const;

  /// @brief ノードを追加する．
  void
  put_node(MisNode* node);

  /// @brief ノードを取り去る．
  void
  delete_node(MisNode* node);

  /// @brief 値が最小の要素を取り出す．
  /// そのノードはヒープから取り除かれる．
  MisNode*
  get_min();

  /// @brief ノードの値の変更に伴ってヒープ構造を更新する．
  /// @param[in] node 値が変更されたノード
  void
  update(MisNode* node);

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
  move_down(MisNode* node);

  /// @brief ノードを適当な位置まで浮かび上がらせる．
  /// @param[in] node 対象のノード
  void
  move_up(MisNode* node);

  /// @brief ノードの比較を行う．
  /// @param[in] node1, node2 対象のノード
  /// @retval 負の値 node1 が node2 より前にある．
  /// @retval 0 node1 と node2 は同じ
  /// @retval 正の値 node1 が node2 より後ろにある．
  static
  int
  compare(MisNode* node1,
	  MisNode* node2);

  /// @brief ノードをセットする．
  /// @param[in] pos 位置
  /// @param[in] node ノード
  void
  set(ymuint pos,
      MisNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeSize;

  // ノードの実体の配列
  // サイズは mNodeSize
  MisNode* mNodeChunk;

  // ノードのヒープ木
  MisNode** mNodeHeap;

  // ヒープ木中にあるノード数
  ymuint32 mNodeNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 確保されているノード数を返す．
inline
ymuint
MisNodeHeap::node_size() const
{
  return mNodeSize;
}

// @brief ノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < node_size() )
inline
MisNode*
MisNodeHeap::node(ymuint pos)
{
  return &mNodeChunk[pos];
}

// @brief ノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < node_size() )
inline
const MisNode*
MisNodeHeap::node(ymuint pos) const
{
  return &mNodeChunk[pos];
}

// @brief ヒープが空の時 true を返す．
inline
bool
MisNodeHeap::empty() const
{
  return mNodeNum == 0;
}

// @brief ノードを追加する．
inline
void
MisNodeHeap::put_node(MisNode* node)
{
  ASSERT_COND( mNodeNum < mNodeSize );

  set(mNodeNum, node);
  ++ mNodeNum;
  move_up(node);
}

// @brief ノードを取り去る．
inline
void
MisNodeHeap::delete_node(MisNode* node)
{
  ASSERT_COND( !empty() );

  ymuint idx = node->mHeapIdx;

  ASSERT_COND( idx > 0 );

  node->mHeapIdx = 0;
  -- mNodeNum;
  MisNode* last = mNodeHeap[mNodeNum];
  if ( last != node ) {
    -- idx;
    set(idx, last);
    move_down(last);
  }
}

// @brief 値が最小の要素を取り出す．
// そのノードはヒープから取り除かれる．
inline
MisNode*
MisNodeHeap::get_min()
{
  ASSERT_COND( !empty() );

  MisNode* node = mNodeHeap[0];
  node->mHeapIdx = 0;
  -- mNodeNum;
  if ( mNodeNum > 0 ) {
    MisNode* last = mNodeHeap[mNodeNum];
    set(0, last);
    move_down(last);
  }
  return node;
}

// @brief ノードの値の変更に伴ってヒープ構造を更新する．
// @param[in] node 値が変更されたノード
inline
void
MisNodeHeap::update(MisNode* node)
{
  ymuint idx = node->mHeapIdx;
  ASSERT_COND( idx > 0 );

  -- idx;
  ASSERT_COND( mNodeHeap[idx] == node );

  if ( idx > 0 ) {
    ymuint p_idx = (idx - 1) / 2;
    MisNode* p_node = mNodeHeap[p_idx];
    if ( compare(p_node, node) > 0 ) {
      move_up(node);
      goto end;
    }
  }
  move_down(node);
 end:
  ;
}

// @brief 要素の比較を行う．
// @param[in] node1, node2 対象の要素
// @retval 負の値 node1 が node2 より前にある．
// @retval 0 node1 と node2 は同じ
// @retval 正の値 node1 が node2 より後ろにある．
inline
int
MisNodeHeap::compare(MisNode* node1,
		    MisNode* node2)
{
  return node1->adj_num() - node2->adj_num();
}

// @brief 要素をセットする．
// @param[in] pos 位置
// @param[in] node 要素
inline
void
MisNodeHeap::set(ymuint pos,
		MisNode* node)
{
  mNodeHeap[pos] = node;
  node->mHeapIdx = pos + 1;
}

END_NAMESPACE_YM

#endif // MCNODEHEAP_H
