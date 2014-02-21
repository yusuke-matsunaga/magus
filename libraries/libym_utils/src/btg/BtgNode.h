#ifndef BTGNODE_H
#define BTGNODE_H

/// @file BtgNode.h
/// @brief BtgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

class BtgEdge;

//////////////////////////////////////////////////////////////////////
/// @class BtgNode BtgNode.h "BtgNode.h"
/// @brief BtgMatch のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BtgNode
{
  friend class BtgMatch;
  friend class BtgHeapTree;

public:

  /// @brief コンストラクタ
  BtgNode();

  /// @brief デストラクタ
  ~BtgNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をセットする．
  /// @param[in] type 種類 ( 0 or 1 )
  /// @param[in] id ID番号
  void
  set(ymuint type,
      ymuint id);

  /// @brief 種類を返す．( 0 or 1 )
  ymuint
  type() const;

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 接続している枝の先頭を返す．
  BtgEdge*
  edge_top() const;

  /// @brief 選ばれている枝を返す．
  BtgEdge*
  cur_edge() const;

  /// @brief マッチングの枝を設定する．
  void
  set_cur_edge(BtgEdge* edge);

  /// @brief 重みの和を得る．
  ymint32
  weight() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号と種類(1bit)をパックしたもの
  ymuint32 mIdType;

  // 枝の先頭
  BtgEdge* mEdgeTop;

  // 今選択されている枝
  BtgEdge* mCurEdge;

  // 重みの和
  ymint32 mWeight;

  // ヒープ上の位置
  ymuint32 mPos;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
BtgNode::BtgNode()
{
  mEdgeTop = NULL;
  mCurEdge = NULL;
}

// @brief デストラクタ
inline
BtgNode::~BtgNode()
{
}

// @brief 内容をセットする．
// @param[in] type 種類 ( 0 or 1 )
// @param[in] id ID番号
inline
void
BtgNode::set(ymuint type,
	     ymuint id)
{
  mIdType = (id << 1) | (type & 1U);
}

// @brief 種類を返す．
inline
ymuint
BtgNode::type() const
{
  return mIdType & 1U;
}

// @brief ID番号を返す．
inline
ymuint
BtgNode::id() const
{
  return mIdType >> 1;
}

// @brief 接続している枝の先頭を返す．
inline
BtgEdge*
BtgNode::edge_top() const
{
  return mEdgeTop;
}

// @brief 選ばれている枝を返す．
inline
BtgEdge*
BtgNode::cur_edge() const
{
  return mCurEdge;
}

// @brief マッチングの枝を設定する．
inline
void
BtgNode::set_cur_edge(BtgEdge* edge)
{
  mCurEdge = edge;
}

// @brief 重みの和を得る．
inline
ymint32
BtgNode::weight() const
{
  return mWeight;
}

END_NAMESPACE_YM

#endif // BTGNODE_H
