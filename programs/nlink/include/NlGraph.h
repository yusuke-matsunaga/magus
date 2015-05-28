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

END_NAMESPACE_YM_NLINK

#endif // NLGRAPH_H
