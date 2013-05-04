
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
  mRepId = PyObject_FromYmuint32(graph->rep_id());
  mRootInfo = Py_BuildValue("(Ii)", graph->root_id(), graph->root_inv());
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
  Py_DECREF(mRepId);
  Py_DECREF(mRootInfo);
  Py_DECREF(mEdgeList);
}

END_NAMESPACE_YM
