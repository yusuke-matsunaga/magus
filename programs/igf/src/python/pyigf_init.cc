
/// @file pyigf_init.cc
/// @brief pyigf の初期化関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.



//////////////////////////////////////////////////////////////////////
// pyigf モジュール
//////////////////////////////////////////////////////////////////////

#include "pyigf.h"

BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

// メソッドテーブル
PyMethodDef igf_methods[] = {
  // end marker
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE

extern
void
FuncVectObject_init(PyObject* m);

extern
void
PhfGenObject_init(PyObject* m);

extern
void
RegVectObject_init(PyObject* m);

extern
void
RvMgrObject_init(PyObject* m);

END_NAMESPACE_YM_IGF


PyMODINIT_FUNC
igf_init()
{
  using namespace nsYm::nsIgf;

  //////////////////////////////////////////////////////////////////////
  // モジュールオブジェクトの生成
  //////////////////////////////////////////////////////////////////////
  PyDoc_STRVAR(module_doc,
	       "Module for igf");
  PyObject* m = Py_InitModule3("igf", igf_methods, module_doc);
  if ( m == NULL ) {
    return;
  }


  //////////////////////////////////////////////////////////////////////
  // タイプオブジェクトの登録
  //////////////////////////////////////////////////////////////////////

  FuncVectObject_init(m);

  PhfGenObject_init(m);

  RegVectObject_init(m);

  RvMgrObject_init(m);

}

