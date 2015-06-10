#ifndef FRGNODE_H
#define FRGNODE_H

/// @file FrgNode.h
/// @brief FrgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

class FrgEdge;

//////////////////////////////////////////////////////////////////////
/// @class FrgNode FrgNode.h "FrgNode.h"
/// @brief 問題のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class FrgNode
{
  friend class FrgGraph;

public:

  /// @brief コンストラクタ
  FrgNode();

  /// @brief デストラクタ
  ~FrgNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号を返す．
  ymuint
  id() const;

  /// @brief X 座標を返す．
  ymuint
  x() const;

  /// @brief Y 座標を返す．
  ymuint
  y() const;

  /// @brief 接続する枝のリストを返す．
  const vector<FrgEdge*>&
  edge_list() const;

  /// @brief 端子番号を返す．
  ymuint
  terminal_id() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint mId;

  // X 座標
  ymuint mX;

  // Y 座標
  ymuint mY;

  // 接続する枝のリスト
  vector<FrgEdge*> mEdgeList;

  // 端子番号
  ymuint mTermId;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
FrgNode::FrgNode()
{
}

// @brief デストラクタ
inline
FrgNode::~FrgNode()
{
}

// @brief ノード番号を返す．
inline
ymuint
FrgNode::id() const
{
  return mId;
}

// @brief X 座標を返す．
inline
ymuint
FrgNode::x() const
{
  return mX;
}

// @brief Y 座標を返す．
inline
ymuint
FrgNode::y() const
{
  return mY;
}

// @brief 接続する枝のリストを返す．
inline
const vector<FrgEdge*>&
FrgNode::edge_list() const
{
  return mEdgeList;
}

// @brief 端子番号を返す．
inline
ymuint
FrgNode::terminal_id() const
{
  return mTermId;
}

END_NAMESPACE_YM_NLINK

#endif // FRGNODE_H
