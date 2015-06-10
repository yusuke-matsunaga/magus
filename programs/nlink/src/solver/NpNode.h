#ifndef NPNODE_H
#define NPNODE_H

/// @file NpNode.h
/// @brief NpNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

class NpEdge;

//////////////////////////////////////////////////////////////////////
/// @class NpNode NpNode.h "NpNode.h"
/// @brief nonpolar equality graph のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class NpNode
{
  friend class NpGraph;
  friend class NpGraph2;
private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  NpNode(ymuint id);

  /// @brief デストラクタ
  ~NpNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 接続している枝のリストを返す．
  const vector<NpEdge*>&
  edge_list() const;

  /// @brief 隣接している節点のリストを返す．
  const vector<NpNode*>&
  adj_node_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // 枝のリスト
  vector<NpEdge*> mEdgeList;

  // 隣接している節点のリスト
  vector<NpNode*> mAdjNodeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
inline
NpNode::NpNode(ymuint id) :
  mId(id)
{
}

// @brief デストラクタ
inline
NpNode::~NpNode()
{
}

// @brief ID番号を返す．
inline
ymuint
NpNode::id() const
{
  return mId;
}

// @brief 接続している枝のリストを返す．
inline
const vector<NpEdge*>&
NpNode::edge_list() const
{
  return mEdgeList;
}

// @brief 隣接している節点のリストを返す．
inline
const vector<NpNode*>&
NpNode::adj_node_list() const
{
  return mAdjNodeList;
}

END_NAMESPACE_YM_NLINK

#endif // NPNODE_H
