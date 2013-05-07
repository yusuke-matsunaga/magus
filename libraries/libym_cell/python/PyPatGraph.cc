
/// @file PyPatGraph.cc
/// @brief PyPatGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyPatGraph.h"
#include "ym_cell/CellPatGraph.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyPatGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyPatGraph::PyPatGraph(const CellPatGraph* graph) :
  mPatGraph(graph)
{
  ymuint ne = graph->edge_num();
  mEdgeList = PyList_New(ne);
  for (ymuint i = 0; i < ne; ++ i) {
    PyObject* obj = PyObject_FromYmuint32(graph->edge(i));
    PyList_SetItem(mEdgeList, i, obj);
  }
}

// @brief デストラクタ
PyPatGraph::~PyPatGraph()
{
  Py_DECREF(mEdgeList);
}

// @brief 代表関数番号を返す．
PyObject*
PyPatGraph::rep_id()
{
  return PyObject_FromYmuint32(pat_graph()->rep_id());
}

// @brief 根のノード番号と反転属性のタプルを返す．
PyObject*
PyPatGraph::root_info()
{
  return Py_BuildValue("(Ii)", pat_graph()->root_id(), pat_graph()->root_inv());
}

END_NAMESPACE_YM
