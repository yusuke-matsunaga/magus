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

//////////////////////////////////////////////////////////////////////
/// @class NlNode NlGraph.h "NlGraph.h"
/// @brief NlGraph のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class NlNode
{
  friend class NlGraph;

private:

  /// @brief コンストラクタ
  NlNode();

  /// @brief デストラクタ
  ~NlNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief X座標を得る．
  ymuint
  x() const;

  /// @brief Y座標を得る．
  ymuint
  y() const;

  /// @brief 端点番号を得る．
  ///
  /// 端点でない場合は 0 を返す．
  ymuint
  terminal_id() const;

  /// @brief 接続している枝番号のリストを返す．
  const vector<ymuint>&
  edge_list() const;

  /// @brief 左の枝番号を返す．
  ///
  /// なければ 0 を返す．
  ymuint
  left_edge() const;

  /// @brief 右の枝番号を返す．
  ///
  /// なければ 0 を返す．
  ymuint
  right_edge() const;

  /// @brief 上の枝番号を返す．
  ///
  /// なければ 0 を返す．
  ymuint
  upper_edge() const;

  /// @brief 下の枝番号を返す．
  ///
  /// なければ 0 を返す．
  ymuint
  lower_edge() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // X座標
  ymuint mX;

  // Y座標
  ymuint mY;

  // 端点番号
  // 0 で無印
  ymuint mTermId;

  // 接続する枝番号のリスト
  vector<ymuint> mEdgeList;

  // 左の枝
  ymuint mLeftEdge;

  // 右の枝
  ymuint mRightEdge;

  // 上の枝
  ymuint mUpperEdge;

  // 下の枝
  ymuint mLowerEdge;

};


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

  /// @brief 左の枝番号を返す．
  /// @param[in] x, y 座標
  ymuint
  left_edge(ymuint x,
	    ymuint y) const;

  /// @brief 右の枝番号を返す．
  /// @param[in] x, y 座標
  ymuint
  right_edge(ymuint x,
	     ymuint y) const;

  /// @brief 上の枝番号を返す．
  /// @param[in] x, y 座標
  ymuint
  upper_edge(ymuint x,
	     ymuint y) const;

  /// @brief 下の枝番号を返す．
  /// @param[in] x, y 座標
  ymuint
  lower_edge(ymuint x,
	     ymuint y) const;


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
  node(ymuint x,
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

};

END_NAMESPACE_YM_NLINK

#endif // NLGRAPH_H
