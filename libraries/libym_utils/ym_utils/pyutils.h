#ifndef YM_UTILS_PYUTILS_H
#define YM_UTILS_PYUTILS_H

/// @file ym_utils/pyutils.h
/// @brief libym_utils の Python 用拡張モジュール
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "ym_utils/USTime.h"
#include "ym_utils/StopWatch.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_PYTHON

//////////////////////////////////////////////////////////////////////
// 型を表すタイプオブジェクト
//////////////////////////////////////////////////////////////////////

/// @brief USTime を表す型
extern
PyTypeObject USTimeType;

/// @brief StopWatch を表す型
extern
PyTypeObject StopWatchType;

/// @brief MStopWatch を表す型
extern
PyTypeObject MStopWatchType;

/// @brief RandGen を表す型
extern
PyTypeObject RandGenType;

/// @brief RandPermGen を表す型
extern
PyTypeObject RandPermGenType;

/// @brief RandCombiGen を表す型
extern
PyTypeObject RandCombiGenType;


//////////////////////////////////////////////////////////////////////
// 型をチェックする関数
//////////////////////////////////////////////////////////////////////

/// @brief USTimeType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が USTimeType だった．
/// @retval false obj が他の型だった．
inline
bool
USTimeObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &USTimeType;
}

/// @brief StopWatchType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が StopWatchType だった．
/// @retval false obj が他の型だった．
inline
bool
StopWatchObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &StopWatchType;
}

/// @brief MStopWatchType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が MStopWatchType だった．
/// @retval false obj が他の型だった．
inline
bool
MStopWatchObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &MStopWatchType;
}

/// @brief RandGenType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が RandGenType だった．
/// @retval false obj が他の型だった．
inline
bool
RandGenObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &RandGenType;
}

/// @brief RandPermGenType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が RandPermGenType だった．
/// @retval false obj が他の型だった．
inline
bool
RandPermGenObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &RandPermGenType;
}

/// @brief RandCombiGenType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が RandCombiGenType だった．
/// @retval false obj が他の型だった．
inline
bool
RandCombiGenObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &RandCombiGenType;
}


//////////////////////////////////////////////////////////////////////
// PyObject からの型変換
//////////////////////////////////////////////////////////////////////

/// @brief PyObject から USTime を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj USTime を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が USTimeObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   USTime& obj);

/// @brief PyObject から StopWatch を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj StopWatch を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が StopWatchObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   StopWatch& obj);


//////////////////////////////////////////////////////////////////////
// PyObject への型変換
//////////////////////////////////////////////////////////////////////

/// @brief USTime から PyObject を生成する．
/// @param[in] obj USTime オブジェクト
extern
PyObject*
conv_to_pyobject(USTime obj);

/// @brief StopWatch から PyObject を生成する．
/// @param[in] obj StopWatch オブジェクト
extern
PyObject*
conv_to_pyobject(StopWatch obj);

END_NAMESPACE_YM_PYTHON

#endif // YM_UTILS_PYUTILS_H
