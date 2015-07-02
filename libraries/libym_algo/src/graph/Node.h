#ifndef NODE_H
#define NODE_H

/// @file Node.h
/// @brief Node のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Node Node.h "Node.h"
/// @brief NodeHeap で用いられるクラス
//////////////////////////////////////////////////////////////////////
class Node
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  /// @param[in] id ノード番号
  void
  set(ymuint id);

  /// @brief 隣接ノードの情報を設定する．
  void
  set_adj_link(ymuint adj_num,
	       Node** adj_link);

  /// @brief ノード番号を返す．
  ymuint
  id() const;

  /// @brief 削除済みフラグを返す．
  bool
  deleted() const;

  /// @brief 隣接するノード数を返す．
  ymuint
  adj_size() const;

  /// @brief 隣接するノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < adj_size() )
  Node*
  adj_node(ymuint pos) const;

  /// @brief 有効な隣接ノード数を返す．
  ymuint
  adj_num() const;

  /// @brief adj_num を1減らす
  void
  dec_adj_num();

  /// @brief ヒープ上の位置(+1)を返す．
  ///
  /// ヒープになければ 0 を返す．
  ymuint
  heap_location() const;

  /// @brief ヒープ上の位置を設定する．
  void
  set_heap_location(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号
  ymuint32 mId;

  // 隣接するノードのポインタ配列
  Node** mAdjLink;

  // mAdjLink のサイズ
  ymuint32 mAdjSize;

  // mAdjLink 中の有効な要素数
  ymuint32 mNum;

  // ヒープ上のインデックス
  ymuint32 mHeapIdx;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 内容を初期化する．
// @param[in] id ノード番号
inline
void
Node::set(ymuint id)
{
  mId = id;
  mAdjLink = NULL;
  mAdjSize = 0;
  mNum = 0;
  mHeapIdx = 0;
}

// @brief 隣接ノードの情報を設定する．
inline
void
Node::set_adj_link(ymuint adj_num,
		     Node** adj_link)
{
  mAdjLink = adj_link;
  mAdjSize = adj_num;
  mNum = adj_num;
}

// @brief ノード番号を返す．
inline
ymuint
Node::id() const
{
  return mId;
}

// @brief 削除済みフラグを返す．
inline
bool
Node::deleted() const
{
  return mHeapIdx == 0;
}

// @brief 隣接するノード数を返す．
inline
ymuint
Node::adj_size() const
{
  return mAdjSize;
}

// @brief 隣接するノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < adj_size() )
inline
Node*
Node::adj_node(ymuint pos) const
{
  return mAdjLink[pos];
}

// @brief 有効な隣接ノード数を返す．
inline
ymuint
Node::adj_num() const
{
  return mNum;
}

// @brief adj_num を１減らす
inline
void
Node::dec_adj_num()
{
  -- mNum;
}

// @brief ヒープ上の位置(+1)を返す．
//
// ヒープになければ 0 を返す．
inline
ymuint
Node::heap_location() const
{
  return mHeapIdx;
}

// @brief ヒープ上の位置を設定する．
inline
void
Node::set_heap_location(ymuint pos)
{
  mHeapIdx = pos;
}

END_NAMESPACE_YM

#endif // NODE_H
