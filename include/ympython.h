#ifndef YMPYTHON_H
#define YMPYTHON_H

/// @file ympython.h
/// @brief ymtools の Python 用拡張モジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "Python.h"
#include "ymtools.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// PyObject への型変換
//////////////////////////////////////////////////////////////////////

/// @brief bool から PyObject を生成する．
extern
PyObject*
PyObject_FromBool(bool val);

/// @brief ymint8 から PyObject を生成する．
extern
PyObject*
PyObject_FromYmint8(ymint8 val);

/// @brief ymint16 から PyObject を生成する．
extern
PyObject*
PyObject_FromYmint16(ymint16 val);

/// @brief ymint32 から PyObject を生成する．
extern
PyObject*
PyObject_FromYmint32(ymint32 val);

/// @brief ymint64 から PyObject を生成する．
extern
PyObject*
PyObject_FromYmint64(ymint64 val);

/// @brief ymuint8 から PyObject を生成する．
extern
PyObject*
PyObject_FromYmuint8(ymuint8 val);

/// @brief ymuint16 から PyObject を生成する．
extern
PyObject*
PyObject_FromYmuint16(ymuint16 val);

/// @brief ymuint32 から PyObject を生成する．
extern
PyObject*
PyObject_FromYmuint32(ymuint32 val);

/// @brief ymuint64 から PyObject を生成する．
extern
PyObject*
PyObject_FromYmuint64(ymuint64 val);

/// @brief float から PyObject を生成する．
extern
PyObject*
PyObject_FromFloat(float val);

/// @brief double から PyObject を生成する．
extern
PyObject*
PyObject_FromDouble(double val);

/// @brief const char* から PyObject を生成する．
extern
PyObject*
PyObject_FromString(const char* val);

/// @brief string から PyObject を生成する．
extern
PyObject*
PyObject_FromString(const string& val);

END_NAMESPACE_YM

#endif // YMPYTHON_H
