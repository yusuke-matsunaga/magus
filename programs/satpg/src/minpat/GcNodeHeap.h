#ifndef GCNODEHEAP_H
#define GCNODEHEAP_H

/// @file GcNodeHeap.h
/// @brief GcNodeHeap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GcNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class GcNodeHeap GcNodeHeap.h "GcNodeHeap.h"
/// @brief GcNode のヒープ木
//////////////////////////////////////////////////////////////////////
class GcNodeHeap
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size 最大の要素数
  GcNodeHeap(ymuint max_size);

  /// @brief デストラクタ
  ~GcNodeHeap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ヒープが空の時 true を返す．
  bool
  empty() const;

  /// @brief ノードを追加する．
  void
  put_node(GcNode* node);

  /// @brief ノードを取り去る．
  void
  delete_node(GcNode* node);

  /// @brief 値が最小の要素を取り出す．
  /// そのノードはヒープから取り除かれる．
  GcNode*
  get_min();

  /// @brief ノードの値の変更に伴ってヒープ構造を更新する．
  /// @param[in] node 値が変更されたノード
  void
  update(GcNode* node);

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
  move_down(GcNode* node);

  /// @brief ノードを適当な位置まで浮かび上がらせる．
  /// @param[in] node 対象のノード
  void
  move_up(GcNode* node);

  /// @brief ノードをヒープ上にセットする．
  /// @param[in] node ノード
  /// @param[in] pos 位置
  void
  locate_node(GcNode* node,
	      ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヒープサイズ
  ymuint32 mHeapSize;

  // ノードのヒープ木
  GcNode** mNodeHeap;

  // ヒープ木中にあるノード数
  ymuint32 mNodeNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ヒープが空の時 true を返す．
inline
bool
GcNodeHeap::empty() const
{
  return mNodeNum == 0;
}

// @brief ノードを追加する．
inline
void
GcNodeHeap::put_node(GcNode* node)
{
  ASSERT_COND( mNodeNum < mHeapSize );

  locate_node(node, mNodeNum);
  ++ mNodeNum;
  move_up(node);
}

// @brief ノードを取り去る．
inline
void
GcNodeHeap::delete_node(GcNode* node)
{
  ASSERT_COND( !empty() );

  ymuint idx = node->heap_location();

  ASSERT_COND( idx > 0 );

  node->set_heap_location(0);
  -- mNodeNum;
  GcNode* last = mNodeHeap[mNodeNum];
  if ( last != node ) {
    -- idx;
    locate_node(last, idx);
    move_down(last);
  }
}

// @brief 値が最小の要素を取り出す．
// そのノードはヒープから取り除かれる．
inline
GcNode*
GcNodeHeap::get_min()
{
  ASSERT_COND( !empty() );

  GcNode* node = mNodeHeap[0];
  node->set_heap_location(0);
  -- mNodeNum;
  if ( mNodeNum > 0 ) {
    GcNode* last = mNodeHeap[mNodeNum];
    locate_node(last, 0);
    move_down(last);
  }
  return node;
}

// @brief ノードの値の変更に伴ってヒープ構造を更新する．
// @param[in] node 値が変更されたノード
inline
void
GcNodeHeap::update(GcNode* node)
{
  ymuint idx = node->heap_location();
  ASSERT_COND( idx > 0 );

  -- idx;
  ASSERT_COND( mNodeHeap[idx] == node );

  move_up(node);
  move_down(node);
}

// @brief ヒープ上にノードを置く
// @param[in] node 要素
// @param[in] pos 位置
inline
void
GcNodeHeap::locate_node(GcNode* node,
			ymuint pos)
{
  mNodeHeap[pos] = node;
  node->set_heap_location(pos + 1);
}

END_NAMESPACE_YM_SATPG

#endif // GCNODEHEAP_H
