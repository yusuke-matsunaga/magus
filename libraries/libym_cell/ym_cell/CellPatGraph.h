#ifndef YM_CELL_CELLPATGRAPH_H
#define YM_CELL_CELLPATGRAPH_H

/// @file ym_cell/CellPatGraph.h
/// @brief CellPatGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellPatGraph CellPatGraph.h "ym_cell/CellPatGraph.h"
/// @brief パタングラフを表すクラス
///
/// 実際には根の反転属性と枝のリストしか持っていない．
/// その他の情報は CellPatMgr から補完する．
//////////////////////////////////////////////////////////////////////
class CellPatGraph
{
  friend class CellPatMgr;

public:

  /// @brief コンストラクタ
  CellPatGraph();

  /// @brief デストラクタ
  ~CellPatGraph();


public:

  /// @brief 代表関数番号を返す．
  ymuint
  rep_id() const;

  /// @brief 根のノード番号を返す．
  ymuint
  root_id() const;

  /// @brief 根の反転属性を返す．
  bool
  root_inv() const;

  /// @brief 入力数を返す．
  ymuint
  input_num() const;

  /// @brief 枝数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝(の番号)を返す．
  /// @param[in] pos 位置 ( 0 <= pos < edge_num() )
  ymuint
  edge(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 代表関数番号
  ymuint32 mRepId;

  // 入力数 + 根の反転属性
  ymuint32 mInputNum;

  // 枝数
  ymuint32 mEdgeNum;

  // 枝番号の配列
  ymuint32* mEdgeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 代表関数番号を返す．
inline
ymuint
CellPatGraph::rep_id() const
{
  return mRepId;
}

// @brief 根のノード番号を返す．
inline
ymuint
CellPatGraph::root_id() const
{
  // 枝の番号を2で割ればファンアウト先のノード番号
  return mEdgeList[0] / 2;
}

// @brief 根の反転属性を返す．
inline
bool
CellPatGraph::root_inv() const
{
  return static_cast<bool>(mInputNum & 1U);
}

// @brief 入力数を返す．
inline
ymuint
CellPatGraph::input_num() const
{
  return (mInputNum >> 1);
}

// @brief 枝数を返す．
inline
ymuint
CellPatGraph::edge_num() const
{
  return mEdgeNum;
}

// @brief 枝(の番号)を返す．
// @param[in] pos 位置 ( 0 <= pos < edge_num() )
inline
ymuint
CellPatGraph::edge(ymuint pos) const
{
  return mEdgeList[pos];
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLPATGRAPH_H
