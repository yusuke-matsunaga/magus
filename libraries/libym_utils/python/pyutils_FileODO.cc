
/// @file pyutils_FileODO.cc
/// @brief FileODO の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/FileODO.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// FileODO を表す型
struct FileODOObject
{
  // Python のお約束
  PyObject_HEAD

  // FileODO の本体
  FileODO* mBody;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// FileODOObject の生成関数
FileODOObject*
FileODO_new(PyTypeObject* type)
{
  FileODOObject* self = PyObject_New(FileODOObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mBody = NULL;

  return self;
}

// FileODOObject を開放する関数
void
FileODO_dealloc(FileODOObject* self)
{
  // FileODO の後始末を行う．
  delete self->mBody;

  PyObject_Del(self);
}

// 初期化関数
int
FileODO_init(FileODOObject* self,
	     PyObject* args)
{
  // 引数の形式は
  // - ()
  // - (str) : 種類
  char* type_str = NULL;
  if ( !PyArg_ParseTuple(args, "|s", &type_str) ) {
    return -1;
  }

  tCodecType type = kCodecThrough;
  if ( type_str != NULL ) {
    if ( strcmp(type_str, "through") == 0 ) {
      type = kCodecThrough;
    }
    else if ( strcmp(type_str, "compress") == 0 ) {
      type = kCodecZ;
    }
    else if ( strcmp(type_str, "gzip") == 0 ) {
      type = kCodecGzip;
    }
    else {
#warning "TODO: エラーメッセージを作る．"
      return -1;
    }
  }

  self->mBody = new FileODO(type);

  // 正常に終了したら 0 を返す．
  return 0;
}

// ok 関数
PyObject*
FileODO_ok(FileODOObject* self,
	   PyObject* args)
{
  // 奇妙な文
  bool stat = *(self->mBody);

  return PyObject_FromBool(stat);
}

// open 関数
PyObject*
FileODO_open(FileODOObject* self,
	     PyObject* args)
{
  char* filename = NULL;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return NULL;
  }

  self->mBody->open(filename);

  Py_INCREF(Py_None);
  return Py_None;
}

// close 関数
PyObject*
FileODO_close(FileODOObject* self,
	      PyObject* args)
{
  self->mBody->close();

  Py_INCREF(Py_None);
  return Py_None;
}

// write_8 関数
PyObject*
FileODO_write_8(FileODOObject* self,
		PyObject* args)
{
  ymuint8 val;
  if ( !PyArg_ParseTuple(args, "b", &val) ) {
    return NULL;
  }

  self->mBody->write_8(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_16 関数
PyObject*
FileODO_write_16(FileODOObject* self,
		 PyObject* args)
{
  ymuint16 val;
  if ( !PyArg_ParseTuple(args, "H", &val) ) {
    return NULL;
  }

  self->mBody->write_16(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_32 関数
PyObject*
FileODO_write_32(FileODOObject* self,
		 PyObject* args)
{
  ymuint32 val;
  if ( !PyArg_ParseTuple(args, "k", &val) ) {
    return NULL;
  }

  self->mBody->write_32(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_64 関数
PyObject*
FileODO_write_64(FileODOObject* self,
		 PyObject* args)
{
  ymuint64 val;
  if ( !PyArg_ParseTuple(args, "K", &val) ) {
    return NULL;
  }

  self->mBody->write_64(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_float 関数
PyObject*
FileODO_write_float(FileODOObject* self,
		    PyObject* args)
{
  float val;
  if ( !PyArg_ParseTuple(args, "f", &val) ) {
    return NULL;
  }

  self->mBody->write_float(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_double 関数
PyObject*
FileODO_write_double(FileODOObject* self,
		     PyObject* args)
{
  double val;
  if ( !PyArg_ParseTuple(args, "d", &val) ) {
    return NULL;
  }

  self->mBody->write_double(val);

  Py_INCREF(Py_None);
  return Py_None;
}

// write_str 関数
PyObject*
FileODO_write_str(FileODOObject* self,
		  PyObject* args)
{
  char* str;
  if ( !PyArg_ParseTuple(args, "s", &str) ) {
    return NULL;
  }

  self->mBody->write_str(str);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// FileODOObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef FileODO_methods[] = {
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
  {"ok", (PyCFunction)FileODO_ok, METH_NOARGS,
   PyDoc_STR("check if writable (NONE)")},
  {"open", (PyCFunction)FileODO_open, METH_VARARGS,
   PyDoc_STR("open file (str)")},
  {"close", (PyCFunction)FileODO_close, METH_NOARGS,
   PyDoc_STR("close file (NONE)")},
  {"write_8", (PyCFunction)FileODO_write_8, METH_VARARGS,
   PyDoc_STR("write 8bit data (uint)")},
  {"write_16", (PyCFunction)FileODO_write_16, METH_VARARGS,
   PyDoc_STR("write 16bit data (uint)")},
  {"write_32", (PyCFunction)FileODO_write_32, METH_VARARGS,
   PyDoc_STR("write 32bit data (uint)")},
  {"write_64", (PyCFunction)FileODO_write_64, METH_VARARGS,
   PyDoc_STR("write 64bit data (uint)")},
  {"write_float", (PyCFunction)FileODO_write_float, METH_VARARGS,
   PyDoc_STR("write float data (uint)")},
  {"write_double", (PyCFunction)FileODO_write_double, METH_VARARGS,
   PyDoc_STR("write double data (uint)")},
  {"write_str", (PyCFunction)FileODO_write_str, METH_VARARGS,
   PyDoc_STR("write string data (uint)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// FileODOObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyFileODO_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.FileODO",                // tp_name
  sizeof(FileODOObject),          // tp_basicsize
  (int)0,                          // tp_itemsize

  // Methods to implement standard operations

  (destructor)FileODO_dealloc,    // tp_dealloc
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
  FileODO_methods,                // tp_methods
  0,                               // tp_members
  0,                               // tp_getset
  (struct _typeobject*)0,          // tp_base
  (PyObject*)0,                    // tp_dict
  (descrgetfunc)0,                 // tp_descr_get
  (descrsetfunc)0,                 // tp_descr_set
  (long)0,                         // tp_dictoffset
  (initproc)FileODO_init,         // tp_init
  (allocfunc)0,                    // tp_alloc
  (newfunc)FileODO_new,           // tp_new
  (freefunc)0,                     // tp_free
  (inquiry)0,                      // tp_is_gc

  (PyObject*)0,                    // tp_bases
  (PyObject*)0,                    // tp_mro (method resolution order)
  (PyObject*)0,                    // tp_cache
  (PyObject*)0,                    // tp_subclasses
  (PyObject*)0                     // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と FileODO の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から FileODO へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return FileODO へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
FileODO*
PyFileODO_AsFileODOPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyFileODO_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.FileODO is expected");
    return NULL;
  }

  // 強制的にキャスト
  FileODOObject* my_obj = (FileODOObject*)py_obj;

  return my_obj->mBody;
}

// @brief 引数をパースして FileODO を取り出す．
// @param[in] args 引数オブジェクト
// @return FileODO のポインタを返す．
// @note エラーが起きたら NULL を返す．
FileODO*
parse_FileODO(PyObject* args)
{
  PyObject* obj;
  if ( !PyArg_ParseTuple(args, "O!", &PyFileODO_Type, &obj) ) {
    return NULL;
  }

  FileODO* bp = PyFileODO_AsFileODOPtr(obj);
  return bp;
}


// FileODOObject 関係の初期化を行う．
void
FileODOObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyFileODO_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "FileODO", (PyObject*)&PyFileODO_Type);
}

END_NAMESPACE_YM
