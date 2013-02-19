
/// @file ympython.cc
/// @brief ymtools の Python 用の拡張モジュールの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"


BEGIN_NAMESPACE_YM

/// @brief bool から PyObject を生成する．
PyObject*
PyObject_FromBool(bool val)
{
  if ( val ) {
    Py_INCREF(Py_True);
    return Py_True;
  }
  else {
    Py_INCREF(Py_False);
    return Py_False;
  }
}

// @brief ymint8 から PyObject を生成する．
PyObject*
PyObject_FromYmin8(ymint8 val)
{
  return Py_BuildValue("b", val);
}

// @brief ymint16 から PyObject を生成する．
PyObject*
PyObject_FromYmint16(ymint16 val)
{
  return Py_BuildValue("h", val);
}

// @brief ymint32 から PyObject を生成する．
PyObject*
PyObject_FromYmint32(ymint32 val)
{
  return Py_BuildValue("l", val);
}

// @brief ymint64 から PyObject を生成する．
PyObject*
PyObject_FromYmint64(ymint64 val)
{
  return Py_BuildValue("L", val);
}

// @brief ymuint8 から PyObject を生成する．
PyObject*
PyObject_FromYmuint8(ymuint8 val)
{
  return Py_BuildValue("B", val);
}

// @brief ymuint16 から PyObject を生成する．
PyObject*
PyObject_FromYmuint16(ymuint16 val)
{
  return Py_BuildValue("H", val);
}

// @brief ymuint32 から PyObject を生成する．
PyObject*
PyObject_FromYmuint32(ymuint32 val)
{
  return Py_BuildValue("k", val);
}

// @brief ymuint64 から PyObject を生成する．
PyObject*
PyObject_FromYmuint64(ymuint64 val)
{
  return Py_BuildValue("K", val);
}

// @brief float から PyObject を生成する．
PyObject*
PyObject_FromFloat(float val)
{
  return Py_BuildValue("f", val);
}

// @brief double から PyObject を生成する．
PyObject*
PyObject_FromDouble(double val)
{
  return Py_BuildValue("d", val);
}

// @brief const char* から PyObject を生成する．
PyObject*
PyObject_FromString(const char* val)
{
  return Py_BuildValue("s", val);
}

// @brief string から PyObject を生成する．
PyObject*
PyObject_FromString(const string& val)
{
  return Py_BuildValue("s", val.c_str());
}

END_NAMESPACE_YM
