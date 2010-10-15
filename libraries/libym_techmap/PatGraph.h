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
    kXor
  };

public:

  /// @brief コンストラクタ
  /// @param[in] data 内容を表すエンコードされたデータ
  PatGraph(const ymuint32* data);

  /// @brief デストラクタ
  ~PatGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  ymuint
  node_num() const;

  /// @brief 入力ノード数を返す．
  ymuint
  input_num() const;

  /// @brief 根のノード番号を返す．
  ymuint
  root_id() const;

  /// @brief ノードの種類を返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  /// @note 0 <= id < input_num() のノードは入力ノード
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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // 入力数
  ymuint32 mInputNum;

  // 根のノード番号
  ymuint32 mRootId;

  // ノードの種類を納めた配列
  tType* mNodeTypeArray;

  // 枝数
  ymuint32 mEdgeNum;

  // 枝の配列
  PatEdge* mEdgeArray;

};


/// @relates PatGraph
/// @brief PatGraph の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] patgraph パタングラフ
void
dump(ostream& s,
     const PatGraph& patgraph);


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

// @brief 入力ノード数を返す．
inline
ymuint
PatGraph::input_num() const
{
  return mInputNum;
}

// @brief 根のノード番号を返す．
inline
ymuint
PatGraph::root_id() const
{
  return mRootId;
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
