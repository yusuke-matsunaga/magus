#ifndef LIBYM_TECHMAP_PATGRAPH_H
#define LIBYM_TECHMAP_PATGRAPH_H

/// @file libym_techmap/PatGraph.h
/// @brief PatGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class PatEdge PatGraph.h "PatGraph.h"
/// @brief パタングラフの枝を表すクラス
//////////////////////////////////////////////////////////////////////
class PatEdge
{
  friend class PatGraph;

private:

  /// @brief コンストラクタ
  /// @note PatGraph のみが生成できる．
  PatEdge();


public:

  /// @brief 入力元のノード番号を返す．
  ymuint
  from_id() const;

  /// @brief 出力先のノード番号を返す．
  ymuint
  to_id() const;

  /// @brief ファンインの位置を返す．
  /// @note 2入力ノードだけなので 0 か 1
  ymuint
  fanin_pos() const;

  /// @brief 枝の極性を返す．
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  inverted() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全てのデータをパックしたデータ
  // from_id (15bit) + to_id (15bit) + fanin_pos (1bit) + inverted (1bit)
  ymuint32 mData;

};


//////////////////////////////////////////////////////////////////////
/// @class PatGraph PatGraph.h "PatGraph.h"
/// @brief パタングラフを表すクラス
//////////////////////////////////////////////////////////////////////
class PatGraph
{
public:

  /// @brief ノードの種類
  enum tType {
    kInput,
    kAnd,
    kXor,
  };

public:

  /// @brief コンストラクタ
  PatGraph();

  /// @brief デストラクタ
  ~PatGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノードの種類を返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  tType
  node_type(ymuint id) const;

  /// @brief 枝の数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝を返す．
  /// @param[in] id 枝番号 ( 0 <= id < edge_num() )
  const PatEdge&
  edge(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を設定し，mNodeTypeArray を確保する．
  /// @param[in] n ノード数
  void
  set_node_num(ymuint n);

  /// @brief ノードの種類を設定する．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  /// @param[in] type 設定するノードの種類
  void
  set_node_type(ymuint id,
		tType type);

  /// @brief 枝の数を設定し，mEdgeArray を確保する．
  /// @param[in] n 枝の数
  void
  set_edge_num(ymuint n);

  /// @brief 枝を設定する．
  /// @param[in] id 枝の番号 ( 0 <= id < edge_num() )
  /// @param[in] from_id 入力元のノード番号
  /// @param[in] to_id 出力先のノード番号
  /// @param[in] fanin_pos ファンインの位置 ( 0  or 1 )
  /// @param[in] inverted 極性
  void
  set_edge(ymuint id,
	   ymuint from_id,
	   ymuint to_id,
	   ymuint fanin_pos,
	   bool inverted);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // ノードの種類を納めた配列
  tType* mNodeTypeArray;

  // 枝数
  ymuint32 mEdgeNum;

  // 枝の配列
  PatEdge* mEdgeArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @note PatGraph のみが生成できる．
inline
PatEdge::PatEdge()
{
}

// @brief 入力元のノード番号を返す．
inline
ymuint
PatEdge::from_id() const
{
  return (mData >> 2) & 0x7FFFU;
}

// @brief 出力先のノード番号を返す．
inline
ymuint
PatEdge::to_id() const
{
  return (mData >> 17) & 0x7FFFFU;
}

// @brief ファンインの位置を返す．
// @note 2入力ノードだけなので 0 か 1
inline
ymuint
PatEdge::fanin_pos() const
{
  return (mData >> 1) & 1U;
}

// @brief 枝の極性を返す．
// @retval true 反転あり
// @retval false 反転なし
inline
bool
PatEdge::inverted() const
{
  return static_cast<bool>(mData & 1U);
}

// @brief ノード数を返す．
inline
ymuint
PatGraph::node_num() const
{
  return mNodeNum;
}

// @brief ノードの種類を返す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
inline
PatGraph::tType
PatGraph::node_type(ymuint id) const
{
  return mNodeTypeArray[id];
}

// @brief 枝の数を返す．
inline
ymuint
PatGraph::edge_num() const
{
  return mEdgeNum;
}

// @brief 枝を返す．
// @param[in] id 枝番号 ( 0 <= id < edge_num() )
inline
const PatEdge&
PatGraph::edge(ymuint id) const
{
  return mEdgeArray[id];
}

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_PATGRAPH_H
