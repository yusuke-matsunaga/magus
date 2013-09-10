
/// @file pyutils_FileIDO.cc
/// @brief FileIDO の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/FileIDO.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// FileIDO を表す型
struct FileIDOObject
{
  // Python のお約束
  PyObject_HEAD

  // FileIDO の本体
  FileIDO mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// FileIDOObject の生成関数
FileIDOObject*
FileIDO_new(PyTypeObject* type)
{
  FileIDOObject* self = PyObject_New(FileIDOObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // FileIDO の最低限の初期化を行う．
  new (&self->mBody) FileIDO();

  return self;
}

// FileIDOObject を開放する関数
void
FileIDO_dealloc(FileIDOObject* self)
{
  // FileIDO の後始末を行う．
  self->mBody.close();

  PyObject_Del(self);
}

// 初期化関数
int
FileIDO_init(FileIDOObject* self,
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
FileIDO_ok(FileIDOObject* self,
	    PyObject* args)
{
  // 奇妙な文
  bool stat = self->mBody;

  return PyObject_FromBool(stat);
}

// open 関数
PyObject*
FileIDO_open(FileIDOObject* self,
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
FileIDO_close(FileIDOObject* self,
	       PyObject* args)
{
  self->mBody.close();

  Py_INCREF(Py_None);
  return Py_None;
}

// read_8 関数
PyObject*
FileIDO_read_8(FileIDOObject* self,
		PyObject* args)
{
  ymuint8 val = self->mBody.read_8();
  return PyObject_FromYmuint8(val);
}

// read_16 関数
PyObject*
FileIDO_read_16(FileIDOObject* self,
		 PyObject* args)
{
  ymuint16 val = self->mBody.read_16();
  return PyObject_FromYmuint16(val);
}

// read_32 関数
PyObject*
FileIDO_read_32(FileIDOObject* self,
		 PyObject* args)
{
  ymuint32 val = self->mBody.read_32();
  return PyObject_FromYmuint32(val);
}

// read_64 関数
PyObject*
FileIDO_read_64(FileIDOObject* self,
		 PyObject* args)
{
  ymuint64 val = self->mBody.read_64();
  return PyObject_FromYmuint64(val);
}

// read_float 関数
PyObject*
FileIDO_read_float(FileIDOObject* self,
		    PyObject* args)
{
  float val = self->mBody.read_float();
  return PyObject_FromFloat(val);
}

// read_double 関数
PyObject*
FileIDO_read_double(FileIDOObject* self,
		     PyObject* args)
{
  double val = self->mBody.read_double();
  return PyObject_FromDouble(val);
}

// read_str 関数
PyObject*
FileIDO_read_str(FileIDOObject* self,
		  PyObject* args)
{
  string val = self->mBody.read_str();
  return PyObject_FromString(val);
}


//////////////////////////////////////////////////////////////////////
// FileIDOObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef FileIDO_methods[] = {
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
  {"ok", (PyCFunction)FileIDO_ok, METH_NOARGS,
   PyDoc_STR("check if writable (NONE)")},
  {"open", (PyCFunction)FileIDO_open, METH_VARARGS,
   PyDoc_STR("open file (str)")},
  {"close", (PyCFunction)FileIDO_close, METH_NOARGS,
   PyDoc_STR("close file (NONE)")},
  {"read_8", (PyCFunction)FileIDO_read_8, METH_NOARGS,
   PyDoc_STR("read 8bit data (NONE)")},
  {"read_16", (PyCFunction)FileIDO_read_16, METH_NOARGS,
   PyDoc_STR("read 16bit data (NONE)")},
  {"read_32", (PyCFunction)FileIDO_read_32, METH_NOARGS,
   PyDoc_STR("read 32bit data (NONE)")},
  {"read_64", (PyCFunction)FileIDO_read_64, METH_NOARGS,
   PyDoc_STR("read 8bit data (NONE)")},
  {"read_float", (PyCFunction)FileIDO_read_float, METH_NOARGS,
   PyDoc_STR("read float data (NONE)")},
  {"read_double", (PyCFunction)FileIDO_read_double, METH_NOARGS,
   PyDoc_STR("read double data (NONE)")},
  {"read_str", (PyCFunction)FileIDO_read_str, METH_NOARGS,
   PyDoc_STR("read string data (NONE)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// FileIDOObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyFileIDO_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.FileIDO",                // tp_name
  sizeof(FileIDOObject),          // tp_basicsize
  (int)0,                          // tp_itemsize

  // Methods to implement standard operations

  (destructor)FileIDO_dealloc,    // tp_dealloc
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
  FileIDO_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)FileIDO_init,         // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)FileIDO_new,           // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と FileIDO の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から FileIDO へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return FileIDO へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
FileIDO*
PyFileIDO_AsFileIDOPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyFileIDO_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.FileIDO is expected");
    return NULL;
  }

  // 強制的にキャスト
  FileIDOObject* my_obj = (FileIDOObject*)py_obj;
  return &my_obj->mBody;
}

// @brief 引数をパースして FileIDO を取り出す．
// @param[in] args 引数オブジェクト
// @return FileIDO のポインタを返す．
// @note エラーが起きたら NULL を返す．
FileIDO*
parse_FileIDO(PyObject* args)
{
  PyObject* obj;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyFileIDO_Type, &obj) ) {
    return NULL;
  }

  FileIDO* bp = PyFileIDO_AsFileIDOPtr(obj);

  return bp;
}

// FileIDOObject 関係の初期化を行う．
void
FileIDOObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyFileIDO_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "FileIDO", (PyObject*)&PyFileIDO_Type);
}

END_NAMESPACE_YM
