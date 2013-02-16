#ifndef PYSATPG_H
#define PYSATPG_H

/// @file pysatpg.h
/// @brief satpg の Python 用拡張モジュール
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "satpg_nsdef.h"
#include "Val3.h"
#include "FaultStatus.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 型を表すタイプオブジェクト
//////////////////////////////////////////////////////////////////////

/// @brief Val3 を表す型
extern
PyTypeObject PyVal3_Type;

/// @brief FaultStatus を表す型
extern
PyTypeObject PyFaultStatus_Type;

/// @brief SaFault を表す型
extern
PyTypeObject PySaFault_Type;

/// @brief FaultMgr を表す型
extern
PyTypeObject PyFaultMgr_Type;

/// @brief TestVector を表す型
extern
PyTypeObject PyTestVector_Type;

/// @brief TvMgr を表す型
extern
PyTypeObject PyTvMgr_Type;


//////////////////////////////////////////////////////////////////////
// 型をチェックする関数
//////////////////////////////////////////////////////////////////////

/// @brief Val3 の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が Val3Type だった．
/// @retval false obj が他の型だった．
inline
bool
Val3Object_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyVal3_Type;
}

/// @brief FaultStatusType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が FaultStatusType だった．
/// @retval false obj が他の型だった．
inline
bool
FaultStatusObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyFaultStatus_Type;
}

/// @brief SaFaultType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が SaFaultType だった．
/// @retval false obj が他の型だった．
inline
bool
SaFaultObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PySaFault_Type;
}

/// @brief FaultMgrType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が FaultMgrType だった．
/// @retval false obj が他の型だった．
inline
bool
FaultMgrObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyFaultMgr_Type;
}

/// @brief TestVectorType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が TestVectorType だった．
/// @retval false obj が他の型だった．
inline
bool
TestVectorObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyTestVector_Type;
}

/// @brief TvMgrType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が TvMgrType だった．
/// @retval false obj が他の型だった．
inline
bool
TvMgrObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyTvMgr_Type;
}


//////////////////////////////////////////////////////////////////////
// PyObject からの型変換
//////////////////////////////////////////////////////////////////////

/// @brief PyObject から Val3 を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj Val3 を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が Val3Object ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   Val3& obj);

/// @brief PyObject から FaultStatus を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj FaultStatus を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が FaultStatusObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   FaultStatus& obj);

/// @brief PyObject から SaFault を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] pobj SaFault を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が SaFaultObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   SaFault*& pobj);


//////////////////////////////////////////////////////////////////////
// PyObject への型変換
//////////////////////////////////////////////////////////////////////

/// @brief Val3 から PyObject を生成する．
/// @param[in] obj Val3 オブジェクト
extern
PyObject*
PyVal3_FromVal3(Val3 obj);

/// @brief Val3 を表す文字列から PyObject を生成する．
/// @param[in] str 文字列
extern
PyObject*
PyVal3_FromString(const char* str);

/// @brief 整数から PyObject を生成する．
/// @param[in] val 整数値
extern
PyObject*
PyVal3_FromLong(long val);

/// @brief FaultStatus から PyObject を生成する．
/// @param[in] obj FaultStatus オブジェクト
extern
PyObject*
PyFaultStatus_FromFaultStatus(FaultStatus obj);

/// @brief 文字列から PyObject を生成する．
/// @param[in] str 文字列
extern
PyObject*
PyFaultStatus_FromString(const char* str);

/// @brief SaFault から PyObject を生成する．
/// @param[in] obj SaFault オブジェクト
extern
PyObject*
PySaFault_FromSaFault(SaFault* obj);


//////////////////////////////////////////////////////////////////////
// 引数のパース用便利関数
//////////////////////////////////////////////////////////////////////

#if 0
/// @brief 引数をパースして FileBinO を取り出す．
/// @param[in] args 引数オブジェクト
/// @return FileBinO のポインタを返す．
/// @note エラーが起きたら NULL を返す．
extern
FileBinO*
parse_FileBinO(PyObject* args);

/// @brief 引数をパースして FileBinI を取り出す．
/// @param[in] args 引数オブジェクト
/// @return FileBinI のポインタを返す．
/// @note エラーが起きたら NULL を返す．
extern
FileBinI*
parse_FileBinI(PyObject* args);
#endif

END_NAMESPACE_YM_SATPG


//////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////

BEGIN_EXTERN_C

void
satpg_init();

END_EXTERN_C

#endif // PYSATPG_H
