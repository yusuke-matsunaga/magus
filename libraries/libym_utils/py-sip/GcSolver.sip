
/// @file GcSolver.sip
/// @brief GcSolver の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class GcSolver {
%TypeHeaderCode
#include "utils/GcSolver.h"
using nsYm::GcSolver;
%End

public:

  /// @brief コンストラクタ
  GcSolver();

  /// @brief ノード数を指定して初期化する．
  void
  init(unsigned int num);

  /// @brief ノード数を得る．
  unsigned int
  node_num() const;

  /// @brief 2つのノードを接続する．
  /// @param[in] id1, id2 2つのノードの番号 ( 0 <= id1, id2 < node_num() )
  void
  connect(unsigned int id1,
	  unsigned int id2);

  /// @brief 彩色する．
  /// @param[in] color_group 同じ色のノード番号のリストの配列
  /// @return 彩色数を返す．
  SIP_PYLIST
  coloring();
  %MethodCode
  using std::vector;
  vector<vector<ymuint> > color_group;
  ymuint n = sipCpp->coloring(color_group);
  sipRes = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const vector<ymuint>& g_list = color_group[i];
    ymuint ng = g_list.size();
    PyObject* obj_list = PyList_New(ng);
    for (ymuint j = 0; j < ng; ++ j) {
      PyObject* obj = sipBuildResult(NULL, "i", g_list[j]);
      Py_INCREF(obj);
      PyList_SetItem(obj_list, j, obj);
    }
    PyList_SetItem(sipRes, i, obj_list);
  }
  %End

};
