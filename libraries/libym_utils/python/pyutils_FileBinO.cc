
/// @file pyutils_FileBinO.cc
/// @brief FileBinO の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/FileBinO.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// FileBinO を表す型
struct FileBinOObject
{
  // Python のお約束
  PyObject_HEAD

  // FileBinO の本体
  FileBinO mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// FileBinOObject の生成関数
FileBinOObject*
FileBinO_new(PyTypeObject* type)
{
  FileBinOObject* self = PyObject_New(FileBinOObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // FileBinO の最低限の初期化を行う．
  new (&self->mBody) FileBinO();

  return self;
}

// FileBinOObject を開放する関数
void
FileBinO_dealloc(FileBinOObject* self)
{
  // FileBinO の後始末を行う．
  self->mBody.close();

  PyObject_Del(self);
}

// 初期化関数
int
FileBinO_init(FileBinOObject* self,
	      PyObject* args)
{
  // 引数の形式は
  // - ()
  // - (str) : ファイル名
  char* filename = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &filename) ) {
    return -1;
  }

  if ( filename != NULL ) {
    self->mBody.open(filename);
  }

  // 正常に終了したら 0 を返す．
  return 0;
}

// ok 関数
PyObject*
FileBinO_ok(FileBinOObject* self,
	    PyObject* args)
{
  // 奇妙な文
  bool stat = self->mBody;

  return conv_to_pyobject(stat);
}

// open 関数
PyObject*
FileBinO_open(FileBinOObject* self,
	      PyObject* args)
{
  char* filename = NULL;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return -1;
  }

  self->mBody.open(filename);

  Py_INCREF(Py_None);
  return Py_None;
}

// close 関数
PyObject*
FileBinO_close(FileBinOObject* self,
	       PyObject* args)
{
  self->mBody.close();

  Py_INCREF(Py_None);
  return Py_None;
}

// write_8 関数
PyObject*
FileBinO_write_8(FileBinOObject* self,
		 PyObject* args)
{
  ymuint8 val;
  if ( !PyArg_ParseTuple(args, "k", &val) ) {
    return NULL;
  }

  self->mBody.write_8(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_16 関数
PyObject*
FileBinO_write_16(FileBinOObject* self,
		  PyObject* args)
{
  ymuint16 val;
  if ( !PyArg_ParseTuple(args, "k", &val) ) {
    return NULL;
  }

  self->mBody.write_16(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_32 関数
PyObject*
FileBinO_write_32(FileBinOObject* self,
		  PyObject* args)
{
  ymuint32 val;
  if ( !PyArg_ParseTuple(args, "k", &val) ) {
    return NULL;
  }

  self->mBody.write_32(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_64 関数
PyObject*
FileBinO_write_64(FileBinOObject* self,
		  PyObject* args)
{
  ymuint64 val;
  if ( !PyArg_ParseTuple(args, "K", &val) ) {
    return NULL;
  }

  self->mBody.write_64(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_float 関数
PyObject*
FileBinO_write_float(FileBinOObject* self,
		     PyObject* args)
{
  float val;
  if ( !PyArg_ParseTuple(args, "f", &val) ) {
    return NULL;
  }

  self->mBody.write_float(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_double 関数
PyObject*
FileBinO_write_double(FileBinOObject* self,
		      PyObject* args)
{
  double val;
  if ( !PyArg_ParseTuple(args, "d", &val) ) {
    return NULL;
  }

  self->mBody.write_double(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_str 関数
PyObject*
FileBinO_write_8(FileBinOObject* self,
		 PyObject* args)
{
  char* str;
  if ( !PyArg_ParseTuple(args, "s", &val) ) {
    return NULL;
  }

  self->mBody.write_str(val);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// FileBinOObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef FileBinO_methods[] = {
  // PyMethodDef のフィールド
  //   char*       ml_name;
  //   PyCFunction ml_meth;
  //   int         ml_flags;
  //   char*       ml_doc;
  // ml_flags で使用可能なマクロは以下の通り
  //  - METH_VARARGS
  //  - METH_KEYWORDS
  //  - METH_NOARGS
  //  - METH_O
  //  - METH_OLDARGS (obsolete)
  //  - METH_CLASS
  //  - METH_STATIC
  //  - METH_COEXIST
  {"ok", (PyCFunction)FileBinO_ok, METH_NOARGS,
   PyDoc_STR("check if writable (NONE)")},
  {"open", (PyCFunction)FileBinO_open, METH_VARARGS,
   PyDoc_STR("open file (str)")},
  {"close", (PyCFunction)FileBinO_close, METH_NOARGS,
   PyDoc_STR("close file (NONE)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// FileBinOObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject FileBinOType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.FileBinO",                // tp_name
  sizeof(FileBinOObject),          // tp_basicsize
  (int)0,                          // tp_itemsize

  // Methods to implement standard operations

  (destructor)FileBinO_dealloc,    // tp_dealloc
  (printfunc)0,                    // tp_print
  (getattrfunc)0,                  // tp_getattr
  (setattrfunc)0,                  // tp_setattr
  (cmpfunc)0,                      // tp_compare
  (reprfunc)0,                     // tp_repr

  // Method suites for standard classes
  0,                               // tp_as_number
  0,                               // tp_as_sequence
  0,                               // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                     // tp_hash
  (ternaryfunc)0,                  // tp_call
  (reprfunc)0,                     // tp_str
  (getattrofunc)0,                 // tp_getattro
  (setattrofunc)0,                 // tp_setattro

  // Functions to access object as input/output buffer
  0,                               // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,              // tp_flags

  // Documentation string
  "Binary File Output",            // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                 // tp_traverse

  // delete references to contained objects
  (inquiry)0,                      // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                  // tp_richcompare

  // weak reference enabler
  (long)0,                         // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                  // tp_iter
  (iternextfunc)0,                 // tp_iternext

  // Attribute descriptor and subclassing stuff
  FileBinO_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)FileBinO_init,         // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)FileBinO_new,           // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と FileBinO の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から FileBinO を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj FileBinO を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が FileBinOObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   FileBinO& obj)
{
  // 型のチェック
  if ( !FileBinOObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  FileBinOObject* my_obj = (FileBinOObject*)py_obj;

  obj = *my_obj->mBody;

  return true;
}

// FileBinOObject 関係の初期化を行う．
void
FileBinOObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&FileBinOType) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "FileBinO", (PyObject*)&FileBinOType);
}

END_NAMESPACE_YM_PYTHON
