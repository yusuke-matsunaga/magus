
/// @file pyutils_FileBinI.cc
/// @brief FileBinI の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/FileBinI.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// FileBinI を表す型
struct FileBinIObject
{
  // Python のお約束
  PyObject_HEAD

  // FileBinI の本体
  FileBinI mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// FileBinIObject の生成関数
FileBinIObject*
FileBinI_new(PyTypeObject* type)
{
  FileBinIObject* self = PyObject_New(FileBinIObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // FileBinI の最低限の初期化を行う．
  new (&self->mBody) FileBinI();

  return self;
}

// FileBinIObject を開放する関数
void
FileBinI_dealloc(FileBinIObject* self)
{
  // FileBinI の後始末を行う．
  self->mBody.close();

  PyObject_Del(self);
}

// 初期化関数
int
FileBinI_init(FileBinIObject* self,
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
FileBinI_ok(FileBinIObject* self,
	    PyObject* args)
{
  // 奇妙な文
  bool stat = self->mBody;

  return PyObject_FromBool(stat);
}

// open 関数
PyObject*
FileBinI_open(FileBinIObject* self,
	      PyObject* args)
{
  char* filename = NULL;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return NULL;
  }

  self->mBody.open(filename);

  Py_INCREF(Py_None);
  return Py_None;
}

// close 関数
PyObject*
FileBinI_close(FileBinIObject* self,
	       PyObject* args)
{
  self->mBody.close();

  Py_INCREF(Py_None);
  return Py_None;
}

// read_8 関数
PyObject*
FileBinI_read_8(FileBinIObject* self,
		PyObject* args)
{
  ymuint8 val = self->mBody.read_8();
  return PyObject_FromYmuint8(val);
}

// read_16 関数
PyObject*
FileBinI_read_16(FileBinIObject* self,
		 PyObject* args)
{
  ymuint16 val = self->mBody.read_16();
  return PyObject_FromYmuint16(val);
}

// read_32 関数
PyObject*
FileBinI_read_32(FileBinIObject* self,
		 PyObject* args)
{
  ymuint32 val = self->mBody.read_32();
  return PyObject_FromYmuint32(val);
}

// read_64 関数
PyObject*
FileBinI_read_64(FileBinIObject* self,
		 PyObject* args)
{
  ymuint64 val = self->mBody.read_64();
  return PyObject_FromYmuint64(val);
}

// read_float 関数
PyObject*
FileBinI_read_float(FileBinIObject* self,
		    PyObject* args)
{
  float val = self->mBody.read_float();
  return PyObject_FromFloat(val);
}

// read_double 関数
PyObject*
FileBinI_read_double(FileBinIObject* self,
		     PyObject* args)
{
  double val = self->mBody.read_double();
  return PyObject_FromDouble(val);
}

// read_str 関数
PyObject*
FileBinI_read_str(FileBinIObject* self,
		  PyObject* args)
{
  string val = self->mBody.read_str();
  return PyObject_FromString(val);
}


//////////////////////////////////////////////////////////////////////
// FileBinIObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef FileBinI_methods[] = {
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
  {"ok", (PyCFunction)FileBinI_ok, METH_NOARGS,
   PyDoc_STR("check if writable (NONE)")},
  {"open", (PyCFunction)FileBinI_open, METH_VARARGS,
   PyDoc_STR("open file (str)")},
  {"close", (PyCFunction)FileBinI_close, METH_NOARGS,
   PyDoc_STR("close file (NONE)")},
  {"read_8", (PyCFunction)FileBinI_read_8, METH_NOARGS,
   PyDoc_STR("read 8bit data (NONE)")},
  {"read_16", (PyCFunction)FileBinI_read_16, METH_NOARGS,
   PyDoc_STR("read 16bit data (NONE)")},
  {"read_32", (PyCFunction)FileBinI_read_32, METH_NOARGS,
   PyDoc_STR("read 32bit data (NONE)")},
  {"read_64", (PyCFunction)FileBinI_read_64, METH_NOARGS,
   PyDoc_STR("read 8bit data (NONE)")},
  {"read_float", (PyCFunction)FileBinI_read_float, METH_NOARGS,
   PyDoc_STR("read float data (NONE)")},
  {"read_double", (PyCFunction)FileBinI_read_double, METH_NOARGS,
   PyDoc_STR("read double data (NONE)")},
  {"read_str", (PyCFunction)FileBinI_read_str, METH_NOARGS,
   PyDoc_STR("read string data (NONE)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// FileBinIObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyFileBinI_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.FileBinI",                // tp_name
  sizeof(FileBinIObject),          // tp_basicsize
  (int)0,                          // tp_itemsize

  // Methods to implement standard operations

  (destructor)FileBinI_dealloc,    // tp_dealloc
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
  "Binary File Input",            // tp_doc

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
  FileBinI_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)FileBinI_init,         // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)FileBinI_new,           // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と FileBinI の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から FileBinI へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return FileBinI へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
FileBinI*
PyFileBinI_AsFileBinIPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyFileBinI_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.FileBinI is expected");
    return NULL;
  }

  // 強制的にキャスト
  FileBinIObject* my_obj = (FileBinIObject*)py_obj;
  return &my_obj->mBody;
}

// @brief 引数をパースして FileBinI を取り出す．
// @param[in] args 引数オブジェクト
// @return FileBinI のポインタを返す．
// @note エラーが起きたら NULL を返す．
FileBinI*
parse_FileBinI(PyObject* args)
{
  PyObject* obj;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyFileBinI_Type, &obj) ) {
    return NULL;
  }

  FileBinI* bp = PyFileBinI_AsFileBinIPtr(obj);

  return bp;
}

// FileBinIObject 関係の初期化を行う．
void
FileBinIObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyFileBinI_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "FileBinI", (PyObject*)&PyFileBinI_Type);
}

END_NAMESPACE_YM
