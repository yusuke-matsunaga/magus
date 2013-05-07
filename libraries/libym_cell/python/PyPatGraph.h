#ifndef PYPATGRAPH_H
#define PYPATGRAPH_H

/// @file PyPatGraph.h
/// @brief PyPatGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyPatGraph PyPatGraph.h "PyPatGraph.h"
/// @brief PatGraph のための補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyPatGraph
{
public:

  /// @brief コンストラクタ
  PyPatGraph(const CellPatGraph* graph);

  /// @brief デストラクタ
  ~PyPatGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief CellPatGraph を返す．
  const CellPatGraph*
  pat_graph();

  /// @brief 代表関数番号を返す．
  PyObject*
  rep_id();

  /// @brief 根のノード番号と反転属性のタプルを返す．
  PyObject*
  root_info();

  /// @brief 枝番号のリストを返す．
  PyObject*
  edge_list();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CellPatGraph
  const CellPatGraph* mPatGraph;

  // 枝番号のリスト
  PyObject* mEdgeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief CellPatGraph を返す．
inline
const CellPatGraph*
PyPatGraph::pat_graph()
{
  return mPatGraph;
}

// @brief 枝番号のリストを返す．
inline
PyObject*
PyPatGraph::edge_list()
{
  return mEdgeList;
}

END_NAMESPACE_YM

#endif // PYPATGRAPH_H
