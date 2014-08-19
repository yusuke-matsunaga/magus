#ifndef FLOWNODE_H
#define FLOWNODE_H

/// @file FlowNode.h
/// @brief FlowNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

class FlowEdge;

//////////////////////////////////////////////////////////////////////
/// @class FlowNode FlowNode.h "FlowNode.h"
/// @brief フローグラフのノードを表すクラス
//////////////////////////////////////////////////////////////////////
class FlowNode
{
  friend class FlowGraph;
  friend class FlowNodeHeap;

public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  FlowNode(ymuint id);

  /// @brief デストラクタ
  ~FlowNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 入枝のリストを返す．
  const vector<FlowEdge*>&
  inedge_list() const;

  /// @brief 出枝のリストを返す．
  const vector<FlowEdge*>&
  outedge_list() const;

  /// @brief 距離を返す．
  ymint32
  distance() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 入枝のリスト
  vector<FlowEdge*> mInEdgeList;

  // 選択されている入枝
  FlowEdge* mCurInEdge;

  // 出枝のリスト
  vector<FlowEdge*> mOutEdgeList;

  // 選択されている出枝
  FlowEdge* mCurOutEdge;

  // mSource からの距離
  ymint32 mDistance;

  // 選ばれている枝
  FlowEdge* mParent;

  // ヒープ上の位置
  ymuint32 mPos;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
inline
FlowNode::FlowNode(ymuint id) :
  mId(id)
{
}

// @brief デストラクタ
inline
FlowNode::~FlowNode()
{
}

// @brief ID番号を返す．
inline
ymuint
FlowNode::id() const
{
  return mId;
}

// @brief 入枝のリストを返す．
inline
const vector<FlowEdge*>&
FlowNode::inedge_list() const
{
  return mInEdgeList;
}

// @brief 出枝のリストを返す．
inline
const vector<FlowEdge*>&
FlowNode::outedge_list() const
{
  return mOutEdgeList;
}

// @brief 距離を返す．
inline
ymint32
FlowNode::distance() const
{
  return mDistance;
}

END_NAMESPACE_YM

#endif // FLOWNODE_H
