#ifndef NLGRAPH_H
#define NLGRAPH_H

/// @file NlGraph.h
/// @brief NlGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

class NlNode;
class NlEdge;

//////////////////////////////////////////////////////////////////////
/// @class NlGraph NlGraph.h "NlGraph.h"
/// @brief number link の問題を表すグラフ
//////////////////////////////////////////////////////////////////////
class NlGraph
{
public:

  /// @brief コンストラクタ
  NlGraph();

  /// @brief デストラクタ
  ~NlGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 問題を設定する．
  /// @param[in] problem 問題
  void
  set_problem(const NlProblem& problem);

  /// @brief 盤面の幅を返す．
  ymuint
  width() const;

  /// @brief 盤面の高さを返す．
  ymuint
  height() const;

  /// @brief 線分数を返す．
  ymuint
  num() const;

  /// @brief 始点を返す．
  /// @param[in] idx 番号 ( 0 <= idx < num() )
  const NlNode*
  start_node(ymuint idx) const;

  /// @brief 終点を返す．
  /// @param[in] idx 番号 ( 0 <= idx < num() )
  const NlNode*
  end_node(ymuint idx) const;

  /// @brief ノード番号の最大値を返す．
  ///
  /// 正確には最大値+1を返す．
  ymuint
  max_node_id() const;

  /// @brief 枝番号の最大値を返す．
  ///
  /// 正確には最大値+1を返す．
  ymuint
  max_edge_id() const;

  /// @brief ノードを返す．
  /// @param[in] x, y 座標
  const NlNode*
  node(ymuint x,
       ymuint y) const;

  /// @brief ノードを返す．
  /// @param[in] id ID番号
  const NlNode*
  node(ymuint id) const;

  /// @brief 枝を返す．
  const NlEdge*
  edge(ymuint id) const;

  /// @brief 枝番号から枝を表す文字列を返す．
  /// @param[in] edge_id 枝番号
  string
  edge_str(ymuint edge_id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いいられるデータ構造
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを返す．
  /// @param[in] x, y 座標
  NlNode*
  _node(ymuint x,
	ymuint y);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 幅
  ymuint mWidth;

  // 高さ
  ymuint mHeight;

  // 線分数
  ymuint mNum;

  // 水平枝のベース番号
  ymuint mHbase;

  // 垂直枝のベース番号
  ymuint mVbase;

  // 枝数
  ymuint mMaxEdgeId;

  // ノード数
  ymuint mMaxNodeId;

  // ノードの配列
  vector<NlNode*> mNodeArray;

  // 端点の配列
  // サイズは mNum * 2 で
  // [0] と [1] に最初の線分の始点と終点が入る．
  vector<const NlNode*> mTermArray;

  // 枝の配列
  vector<NlEdge*> mEdgeArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 盤面の幅を返す．
inline
ymuint
NlGraph::width() const
{
  return mWidth;
}

// @brief 盤面の高さを返す．
inline
ymuint
NlGraph::height() const
{
  return mHeight;
}

// @brief 線分数を返す．
inline
ymuint
NlGraph::num() const
{
  return mNum;
}

// @brief 始点を返す．
// @param[in] idx 番号 ( 0 <= idx < num() )
inline
const NlNode*
NlGraph::start_node(ymuint idx) const
{
  ASSERT_COND( idx < mNum );

  return mTermArray[idx * 2 + 0];
}

// @brief 終点を返す．
// @param[in] idx 番号 ( 0 <= idx < num() )
inline
const NlNode*
NlGraph::end_node(ymuint idx) const
{
  ASSERT_COND( idx < mNum );

  return mTermArray[idx * 2 + 1];
}

// @brief ノード番号の最大値を返す．
//
// 正確には最大値+1を返す．
inline
ymuint
NlGraph::max_node_id() const
{
  return mMaxNodeId;
}

// @brief 枝番号の最大値を返す．
//
// 正確には最大値+1を返す．
inline
ymuint
NlGraph::max_edge_id() const
{
  return mMaxEdgeId;
}

// @brief ノードを得る．
// @param[in] x, y 座標
inline
const NlNode*
NlGraph::node(ymuint x,
	      ymuint y) const
{
  ASSERT_COND( x >= 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y >= 0 );
  ASSERT_COND( y < mHeight );

  return node(x * mHeight + y);
}

// @brief ノードを返す．
// @param[in] id ID番号
inline
const NlNode*
NlGraph::node(ymuint id) const
{
  ASSERT_COND( id < mMaxNodeId );

  return mNodeArray[id];
}


END_NAMESPACE_YM_NLINK

#endif // NLGRAPH_H
