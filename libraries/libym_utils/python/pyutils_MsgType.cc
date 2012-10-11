
/// @file pyutils_MsgType.cc
/// @brief tMsgType の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/msg_type.h"


BEGIN_NAMESPACE_YM_PYTHON


//////////////////////////////////////////////////////////////////////
// MsgTypeObject の外部変数
//////////////////////////////////////////////////////////////////////

// @brief kMsgError を表すオブジェクト
PyObject* Py_kMsgError;

// @brief kMsgWarning を表すオブジェクト
PyObject* Py_kMsgWarning;

// @brief kMsgFailure を表すオブジェクト
PyObject* Py_kMsgFailure;

// @brief kMsgInfo を表すオブジェクト
PyObject* Py_kMsgInfo;

// @brief kMsgDebug を表すオブジェクト
PyObject* Py_kMsgDebug;


BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// MsgType を表す型
struct MsgTypeObject
{
  // Python のお約束
  PyObject_HEAD

  // MsgType の本体
  tMsgType mType;

};

// Py_kMsgError の実体
MsgTypeObject Py_kMsgErrorStruct = {
  PyObject_HEAD_INIT(&MsgTypeType)
  kMsgError
};

// Py_kMsgWarning の実体
MsgTypeObject Py_kMsgWarningStruct = {
  PyObject_HEAD_INIT(&MsgTypeType)
  kMsgWarning
};

// Py_kMsgFailure の実体
MsgTypeObject Py_kMsgFailureStruct = {
  PyObject_HEAD_INIT(&MsgTypeType)
  kMsgFailure
};

// Py_kMsgInfo の実体
MsgTypeObject Py_kMsgInfoStruct = {
  PyObject_HEAD_INIT(&MsgTypeType)
  kMsgInfo
};

// Py_kMsgDebug の実体
MsgTypeObject Py_kMsgDebugStruct = {
  PyObject_HEAD_INIT(&MsgTypeType)
  kMsgDebug
};

// repr 用の文字列オブジェクト
PyObject* Py_kMsgErrorString = NULL;
PyObject* Py_kMsgWarningString = NULL;
PyObject* Py_kMsgFailureString = NULL;
PyObject* Py_kMsgInfoString = NULL;
PyObject* Py_kMsgDebugString = NULL;


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// MsgTypeObject の生成関数
// 実際には存在しているオブジェクトを返すだけ．
MsgTypeObject*
MsgType_new(PyTypeObject* type,
	    PyObject* args)
{
  // 引数は
  // - ()
  // - (str)  {error|warning|failure|info|debug}
  // - (uint) {1-5}
  ymuint n = PyTuple_GET_SIZE(args);
  if ( n == 0 ) {
    // デフォルトは kMsgError
    Py_INCREF(Py_kMsgError);
    return (MsgTypeObject*)Py_kMsgError;
  }
  if ( n == 1 ) {
    PyObject* obj = PyTuple_GET_ITEM(args, 0);
    if ( PyString_Check(obj) ) {
      char* str = PyString_AsString(obj);
      return (MsgTypeObject*)MsgType_FromString(str);
    }
    if ( PyInt_Check(obj) ) {
      ymlong val = PyInt_AS_LONG(obj);
      return (MsgTypeObject*)MsgType_FromLong(val);
    }
  }

  PyErr_SetString(PyExc_TypeError, "string or unsigned int is expected");
  return NULL;
}

// repr 関数
PyObject*
MsgType_repr(MsgTypeObject* self)
{
  switch ( self->mType ) {
  case kMsgError:   return Py_kMsgErrorString;
  case kMsgWarning: return Py_kMsgWarningString;
  case kMsgFailure: return Py_kMsgFailureString;
  case kMsgInfo:    return Py_kMsgInfoString;
  case kMsgDebug:   return Py_kMsgDebugString;
  default: break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// str 関数
PyObject*
MsgType_str(MsgTypeObject* self)
{
  ostringstream buf;
  buf << self->mType;
  return conv_to_pyobject(buf.str());
}

// bitmask 関数
PyObject*
MsgType_bitmask(MsgTypeObject* self,
		PyObject* args)
{
  ymuint32 val = (1U << static_cast<ymuint32>(self->mType));
  return conv_to_pyobject(val);
}


//////////////////////////////////////////////////////////////////////
// MsgTypeObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef MsgType_methods[] = {
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
  {"bitmask", (PyCFunction)MsgType_bitmask, METH_NOARGS,
   PyDoc_STR("convert to bitmask (NONE)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// MsgTypeObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject MsgTypeType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.MsgType",                // tp_name
  sizeof(MsgTypeObject),          // tp_basicsize
  (int)0,                         // tp_itemsize

  // Methods to implement standard operations

  (destructor)0,                  // tp_dealloc
  (printfunc)0,                   // tp_print
  (getattrfunc)0,                 // tp_getattr
  (setattrfunc)0,                 // tp_setattr
  (cmpfunc)0,                     // tp_compare
  (reprfunc)MsgType_repr,         // tp_repr

  // Method suites for standard classes
  0,                              // tp_as_number
  0,                              // tp_as_sequence
  0,                              // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                    // tp_hash
  (ternaryfunc)0,                 // tp_call
  (reprfunc)MsgType_str,          // tp_str
  (getattrofunc)0,                // tp_getattro
  (setattrofunc)0,                // tp_setattro

  // Functions to access object as input/output buffer
  0,                              // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,             // tp_flags

  // Documentation string
  "message type",                 // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                // tp_traverse

  // delete references to contained objects
  (inquiry)0,                     // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                 // tp_richcompare

  // weak reference enabler
  (long)0,                        // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                 // tp_iter
  (iternextfunc)0,                // tp_iternext

  // Attribute descriptor and subclassing stuff
  MsgType_methods,                // tp_methods
  0,                              // tp_members
  0,                              // tp_getset
  (struct _typeobject*)0,         // tp_base
  (PyObject*)0,                   // tp_dict
  (descrgetfunc)0,                // tp_descr_get
  (descrsetfunc)0,                // tp_descr_set
  (long)0,                        // tp_dictoffset
  (initproc)0,                    // tp_init
  (allocfunc)0,                   // tp_alloc
  (newfunc)MsgType_new,           // tp_new
  (freefunc)0,                    // tp_free
  (inquiry)0,                     // tp_is_gc

  (PyObject*)0,                   // tp_bases
  (PyObject*)0,                   // tp_mro (method resolution order)
  (PyObject*)0,                   // tp_cache
  (PyObject*)0,                   // tp_subclasses
  (PyObject*)0                    // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と MsgType の間の変換関数
//////////////////////////////////////////////////////////////////////

// tMsgType からの変換関数
PyObject*
MsgType_FromMsgType(tMsgType type)
{
  PyObject* result = NULL;
  switch ( type ) {
  case kMsgError:   result = Py_kMsgError; break;
  case kMsgWarning: result = Py_kMsgWarning; break;
  case kMsgFailure: result = Py_kMsgFailure; break;
  case kMsgInfo:    result = Py_kMsgInfo; break;
  case kMsgDebug:   result = Py_kMsgDebug; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  Py_INCREF(result);
  return result;
}

// 文字列からの変換関数
PyObject*
MsgType_FromString(const char* str)
{
  if ( str == NULL ) {
    PyErr_SetString(PyExc_ValueError,
		    "NULL pointer is not allowed here");
    return NULL;
  }

  // 小文字に変換する．
  // 効率が悪いけど元の文字列を変更したくないので
  // 新たな領域を確保する．
  ymuint n = strlen(str);
  char* buf = new char[n + 1];
  for (ymuint i = 0; i < n; ++ i) {
    buf[i] = tolower(str[i]);
  }
  buf[n] = '\0';

  PyObject* result = NULL;
  if ( strcmp(buf, "error") == 0 ) {
    result = Py_kMsgError;
  }
  else if ( strcmp(buf, "warning") == 0 ) {
    result = Py_kMsgWarning;
  }
  else if ( strcmp(buf, "failure") == 0 ) {
    result = Py_kMsgFailure;
  }
  else if ( strcmp(buf, "info") == 0 ) {
    result = Py_kMsgInfo;
  }
  else if ( strcmp(buf, "debug") == 0 ) {
    result = Py_kMsgDebug;
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "parameter must be error|warning|failure|info|debug");
    delete [] buf;
    return NULL;
  }

  delete [] buf;

  Py_INCREF(result);
  return result;
}

// long からの変換関数
PyObject*
MsgType_FromLong(long val)
{
  PyObject* result = NULL;
  switch ( val ) {
  case 1: result = Py_kMsgError; break;
  case 2: result = Py_kMsgWarning; break;
  case 3: result = Py_kMsgFailure; break;
  case 4: result = Py_kMsgInfo; break;
  case 5: result = Py_kMsgDebug; break;
  default:
    PyErr_SetString(PyExc_ValueError,
		    "parameter must be in range of [1 ... 5]");
    return NULL;
  }

  Py_INCREF(result);
  return result;
}

// @brief PyObject から MsgType を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj MsgType を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が MsgTypeObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   tMsgType& obj)
{
  // 型のチェック
  if ( !MsgTypeObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  MsgTypeObject* my_obj = (MsgTypeObject*)py_obj;

  obj = my_obj->mType;

  return true;
}

// MsgType の定数を設定する関数
inline
void
MsgType_set(MsgTypeObject& msg_obj,
	    PyObject*& py_obj,
	    PyObject* module,
	    const char* name)
{
  py_obj = (PyObject*)&msg_obj;
  Py_XINCREF(py_obj);
  PyModule_AddObject(module, name, py_obj);
}

// 文字列用オブジェクトの生成を行う．
inline
PyObject*
new_string(const char* str)
{
  PyObject* py_obj = PyString_InternFromString(str);
  Py_INCREF(py_obj);
  return py_obj;
}

// MsgTypeObject 関係の初期化を行う．
void
MsgTypeObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&MsgTypeType) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "MsgType", (PyObject*)&MsgTypeType);

  // 定数オブジェクトの生成と登録
  MsgType_set(Py_kMsgErrorStruct,   Py_kMsgError,   m, "kMsgError");
  MsgType_set(Py_kMsgWarningStruct, Py_kMsgWarning, m, "kMsgWarning");
  MsgType_set(Py_kMsgFailureStruct, Py_kMsgFailure, m, "kMsgFailure");
  MsgType_set(Py_kMsgInfoStruct,    Py_kMsgInfo,    m, "kMsgInfo");
  MsgType_set(Py_kMsgDebugStruct,   Py_kMsgDebug,   m, "kMsgDebug");

  // 定数オブジェクト用の文字列オブジェクトの生成
  Py_kMsgErrorString   = new_string("error");
  Py_kMsgWarningString = new_string("warning");
  Py_kMsgFailureString = new_string("failure");
  Py_kMsgInfoString    = new_string("info");
  Py_kMsgDebugString   = new_string("debug");
}

END_NAMESPACE_YM_PYTHON
