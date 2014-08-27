#ifndef BTGHEAPTREE_H
#define BTGHEAPTREE_H

/// @file BtgHeapTree.h
/// @brief BtgHeapTree のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "btg_nsdef.h"
#include "BtgNode.h"


BEGIN_NAMESPACE_YM_BTG

//////////////////////////////////////////////////////////////////////
/// @class BtgHeapTree BtgHeapTree.h "BtgHeapTree.h"
/// @brief BtgNode のヒープ木
///
/// BtgNode::weight() の大きい順に取り出す．
/// BtgNode::mPos にヒープ中の位置を持たせる．
//////////////////////////////////////////////////////////////////////
class BtgHeapTree
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size 予想される最大要素数
  BtgHeapTree(ymuint max_size);

  /// @brief デストラクタ
  ~BtgHeapTree();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 空にする．
  void
  clear();

  /// @brief 空のときに true を返す．
  bool
  empty() const;

  /// @brief 要素を追加する．
  void
  put(BtgNode* node);

  /// @brief 要素を取り出す．
  BtgNode*
  get();

  /// @brief 要素を適当な位置に動かす．
  /// @param[in] node 対象のノード
  /// @note node の値が変更されたときに用いる．
  void
  update(BtgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を適当な位置まで沈めていく関数
  /// @param[in] node 対象の要素
  void
  move_down(BtgNode* node);

  /// @brief 要素を適当な位置まで浮かび上がらせる関数
  /// @param[in] node 対象の要素
  void
  move_up(BtgNode* node);

  /// @brief 要素の比較を行う．
  /// @param[in] node1, node2 対象の要素
  /// @retval 負の値 node1 が node2 より前にある．
  /// @retval 0 node1 と node2 は同じ
  /// @retval 正の値 node1 が node2 より後ろにある．
  static
  int
  compare(BtgNode* node1,
	  BtgNode* node2);

  /// @brief 要素をセットする．
  /// @param[in] pos 位置
  /// @param[in] node 要素
  void
  set(ymuint pos,
      BtgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大要素数
  ymuint32 mMaxSize;

  /// @brief 実際の要素数
  ymuint32 mNum;

  /// @brief 配列
  BtgNode** mArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空にする．
inline
void
BtgHeapTree::clear()
{
  mNum = 0;
}

// @brief 空のときに true を返す．
inline
bool
BtgHeapTree::empty() const
{
  return mNum == 0;
}

// @brief 要素を追加する．
inline
void
BtgHeapTree::put(BtgNode* node)
{
  ASSERT_COND( mNum < mMaxSize );

  set(mNum, node);
  ++ mNum;
  move_up(node);
}

// @brief 要素を取り出す．
inline
BtgNode*
BtgHeapTree::get()
{
  if ( mNum == 0 ) {
    return NULL;
  }

  BtgNode* node = mArray[0];
  -- mNum;
  BtgNode* last = mArray[mNum];
  set(0, last);
  move_down(last);
  return node;
}

// @brief 要素を適当な位置に動かす．
// @param[in] node 対象のノード
// @note node の値が変更されたときに用いる．
inline
void
BtgHeapTree::update(BtgNode* node)
{
  ymuint pos = node->mPos;
  if ( pos == 0 ) {
    // node はヒープに含まれない．
    return;
  }
  -- pos;
  if ( pos > 0 ) {
    ymuint p_pos = (pos - 1) / 2;
    BtgNode* p_node = mArray[p_pos];
    if ( compare(p_node, node) > 0 ) {
      move_up(node);
      return;
    }
  }
  move_down(node);
}

// @brief 要素の比較を行う．
// @param[in] node1, node2 対象の要素
// @retval 負の値 node1 が node2 より前にある．
// @retval 0 node1 と node2 は同じ
// @retval 正の値 node1 が node2 より後ろにある．
inline
int
BtgHeapTree::compare(BtgNode* node1,
		     BtgNode* node2)
{
  return node2->weight() - node1->weight();
}

// @brief 要素をセットする．
// @param[in] pos 位置
// @param[in] node 要素
inline
void
BtgHeapTree::set(ymuint pos,
		 BtgNode* node)
{
  mArray[pos] = node;
  node->mPos = pos + 1;
}

END_NAMESPACE_YM_BTG

#endif // BTGHEAPTREE_H
