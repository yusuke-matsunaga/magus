
/// @file PyMt19937.cc
/// @brief PyMt19937 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "PyMt19937.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// 通常この構造体は同じサイズのヒープから作られるので
// コンストラクタは起動されない．
// そのためメンバはポインタの形のほうが安全．
struct Mt19937Object
{
  PyObject_HEAD
  std::mt19937* mMt19937;
};

// 生成関数
PyObject*
Mt19937_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  auto self = type->tp_alloc(type, 0);
  auto mt_obj = reinterpret_cast<Mt19937Object*>(self);
  mt_obj->mMt19937 = new std::mt19937;
  return self;
}

// 終了関数
void
Mt19937_dealloc(
  PyObject* self
)
{
  auto mt_obj = reinterpret_cast<Mt19937Object*>(self);
  delete mt_obj->mMt19937;
  Py_TYPE(self)->tp_free(self);
}

// 初期化関数(__init__()相当)
int
Mt19937_init(
  PyObject* self,
  PyObject* args,
  PyObject* Py_UNUSED(kwds)
)
{
  int val = 0;
  if ( !PyArg_ParseTuple(args, "|i", &val) ) {
    return -1;
  }
  auto randgen = PyMt19937::_get(self);
  randgen->seed(val);
  return 0;
}

// eval() 関数
PyObject*
Mt19937_eval(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto randgen = PyMt19937::_get(self);
  auto val = randgen->operator()();
  return PyLong_FromLong(val);
}

// メソッド定義
PyMethodDef Mt19937_methods[] = {
  {"eval", Mt19937_eval, METH_NOARGS,
   PyDoc_STR("generate a random number")},
  {nullptr, nullptr, 0, nullptr}
};

// Python 用のタイプ定義
PyTypeObject Mt19937Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyInit_Mt19937 中で初期化する．
};

END_NONAMESPACE


// @brief 'Mt19937' オブジェクトを使用可能にする．
bool
PyMt19937::init(
  PyObject* m
)
{
  Mt19937Type.tp_name = "matoth.Mt19937";
  Mt19937Type.tp_basicsize = sizeof(Mt19937Object);
  Mt19937Type.tp_itemsize = 0;
  Mt19937Type.tp_dealloc = Mt19937_dealloc;
  Mt19937Type.tp_flags = Py_TPFLAGS_DEFAULT;
  Mt19937Type.tp_doc = PyDoc_STR("Mt19937 objects");
  Mt19937Type.tp_methods = Mt19937_methods;
  Mt19937Type.tp_init = reinterpret_cast<initproc>(Mt19937_init);
  Mt19937Type.tp_new = Mt19937_new;
  if ( PyType_Ready(&Mt19937Type) < 0 ) {
    return false;
  }

  auto type_obj = reinterpret_cast<PyObject*>(&Mt19937Type);
  if ( !reg_type_obj(m, "Mt19937", type_obj) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// PyObject から Mt19937 を取り出す．
bool
PyMt19937::FromPyObject(
  PyObject* obj,
  std::mt19937*& randgen
)
{
  if ( !_check(obj) ) {
    PyErr_SetString(PyExc_TypeError, "object is not a Mt19937 type");
    return false;
  }
  randgen = _get(obj);
  return true;
}

// @brief PyObject が mt19937 タイプか調べる．
bool
PyMt19937::_check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &Mt19937Type);
}

// @brief mt19937 を表す PyObject から mt19937 を取り出す．
std::mt19937*
PyMt19937::_get(
  PyObject* obj
)
{
  auto mt_obj = reinterpret_cast<Mt19937Object*>(obj);
  return mt_obj->mMt19937;
}

// @brief mt19937 を表す PyObject に値を設定する．
void
PyMt19937::_put(
  PyObject* obj,
  const std::mt19937& randgen
)
{
  auto mt_obj = reinterpret_cast<Mt19937Object*>(obj);
  *mt_obj->mMt19937 = randgen;
}

// @brief mt19937 を表すオブジェクトの型定義を返す．
PyTypeObject*
PyMt19937::_typeobject()
{
  return &Mt19937Type;
}

END_NAMESPACE_MAGUS
