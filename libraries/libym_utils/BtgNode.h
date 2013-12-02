#ifndef BTGNODE_H
#define BTGNODE_H

/// @file BtgNode.h
/// @brief BtgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
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

  /// @brief 接続している枝を返す．
  ymuint
  edge_num() const;

  /// @brief 枝を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
  BtgEdge*
  edge(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号と種類(1bit)をパックしたもの
  ymuint32 mIdType;

  // 枝のリスト
  vector<BtgEdge*> mEdgeList;

  // 今選択されている枝
  BtgEdge* mCurEdge;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
BtgNode::BtgNode()
{
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

// @brief 接続している枝を返す．
inline
ymuint
BtgNode::edge_num() const
{
  return mEdgeList.size();
}

// @brief 枝を返す．
// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
inline
BtgEdge*
BtgNode::edge(ymuint pos)
{
  assert_cond( pos < mEdgeList.size(), __FILE__, __LINE__);
  return mEdgeList[pos];
}

END_NAMESPACE_YM

#endif // BTGNODE_H
