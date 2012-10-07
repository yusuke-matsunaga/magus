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


//////////////////////////////////////////////////////////////////////
// 名前空間の定義
//////////////////////////////////////////////////////////////////////

/// @brief ympython 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_PYTHON		\
  BEGIN_NAMESPACE_YM \
  BEGIN_NAMESPACE(nsPython)

/// @brief ympython 用の名前空間の終了
#define END_NAMESPACE_YM_PYTHON			\
  END_NAMESPACE(nsPython)			\
  END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_PYTHON

//////////////////////////////////////////////////////////////////////
// 共通のオブジェクトの宣言
//////////////////////////////////////////////////////////////////////

/// @brief エラー用のオブジェクト
extern
PyObject* ErrorObject;


//////////////////////////////////////////////////////////////////////
// PyObject への型変換
//////////////////////////////////////////////////////////////////////

/// @brief bool から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(bool val);

/// @brief ymint8 から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(ymint8 val);

/// @brief ymint16 から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(ymint16 val);

/// @brief ymint32 から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(ymint32 val);

/// @brief ymint64 から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(ymint64 val);

/// @brief ymuint8 から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(ymuint8 val);

/// @brief ymuint16 から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(ymuint16 val);

/// @brief ymuint32 から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(ymuint32 val);

/// @brief ymuint64 から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(ymuint64 val);

/// @brief float から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(float val);

/// @brief double から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(double val);

/// @brief const char* から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(const char* val);

/// @brief string から PyObject を生成する．
extern
PyObject*
conv_to_pyobject(const string& val);

END_NAMESPACE_YM_PYTHON


//////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////

BEGIN_EXTERN_C

void
ympython_init();

END_EXTERN_C

#endif // YMPYTHON_H
