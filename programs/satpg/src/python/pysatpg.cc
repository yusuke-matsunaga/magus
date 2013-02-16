
/// @file pysatpg.cc
/// @brief pysatpg のメイン関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// pysatpg モジュール
//////////////////////////////////////////////////////////////////////

#include "pysatpg.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// メソッドテーブル
PyMethodDef satpg_methods[] = {
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE

void
Val3Object_init(PyObject* module);

void
FaultStatusObject_init(PyObject* module);

void
SaFaultObject_init(PyObject* module);

void
FaultMgrObject_init(PyObject* module);

void
TestVectorObject_init(PyObject* module);

void
TvMgrObject_init(PyObject* module);

END_NAMESPACE_YM_SATPG


PyMODINIT_FUNC
satpg_init()
{
  using namespace nsYm::nsSatpg;

  //////////////////////////////////////////////////////////////////////
  // モジュールオブジェクトの生成
  //////////////////////////////////////////////////////////////////////
  PyDoc_STRVAR(module_doc,
	       "Module for satpg");
  PyObject* m = Py_InitModule3("satpg", satpg_methods, module_doc);
  if ( m == NULL ) {
    return;
  }

  //////////////////////////////////////////////////////////////////////
  // タイプオブジェクトの登録
  //////////////////////////////////////////////////////////////////////

  Val3Object_init(m);

  FaultStatusObject_init(m);

  SaFaultObject_init(m);

  FaultMgrObject_init(m);

  TestVectorObject_init(m);

  TvMgrObject_init(m);

}


//////////////////////////////////////////////////////////////////////
// メイン関数
//////////////////////////////////////////////////////////////////////

int
main(int argc,
     char** argv)
{
  // プログラム名をセットする．
  Py_SetProgramName(argv[0]);

  // Python インタプリタを初期化する．
  Py_Initialize();


  //////////////////////////////////////////////////////////////////////
  // satpg モジュールの初期化
  //////////////////////////////////////////////////////////////////////

  satpg_init();

  //////////////////////////////////////////////////////////////////////
  // メインループ
  //////////////////////////////////////////////////////////////////////

  int stat = Py_Main(argc, argv);

  Py_Finalize();

  return stat;
}
