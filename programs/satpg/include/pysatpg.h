#ifndef PYSATPG_H
#define PYSATPG_H

/// @file pysatpg.h
/// @brief satpg の Python 用拡張モジュール
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "satpg_nsdef.h"
#include "Val3.h"
#include "FaultStatus.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// PyVal3: 3値の論理型を表す型
//////////////////////////////////////////////////////////////////////

/// @brief Val3 を表すタイプオブジェクト
extern
PyTypeObject PyVal3_Type;

/// @brief PyVal3 の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyVal3_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyVal3_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyVal3_Type;
}

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

/// @brief PyObject から Val3 を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return Val3 を返す．
/// @note 変換が失敗したら TypeError を送出し，kVal3X を返す．
extern
Val3
PyVal3_AsVal3(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyFaultStatus: 故障の状態を表す型
//////////////////////////////////////////////////////////////////////

/// @brief FaultStatus を表すタイプオブジェクト
extern
PyTypeObject PyFaultStatus_Type;

/// @brief PyFaultStatus の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyFaultStatus_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyFaultStatus_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyFaultStatus_Type;
}

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

/// @brief PyObject から FaultStatus を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return FaultStatus を返す．
/// @note 変換が失敗したら TypeError を送出し，kFsUndetected を返す．
extern
FaultStatus
PyFaultStatus_AsFaultStatus(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyTpgFault: 縮退故障を表す型
//////////////////////////////////////////////////////////////////////

/// @brief TpgFault を表すタイプオブジェクト
extern
PyTypeObject PyTpgFault_Type;

/// @brief PyTpgFault の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyTpgFault_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyTpgFault_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyTpgFault_Type;
}

/// @brief TpgFault から PyObject を生成する．
/// @param[in] obj TpgFault オブジェクト
extern
PyObject*
PyTpgFault_FromTpgFault(TpgFault* obj);

/// @brief PyObject から TpgFault へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return TpgFault へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
TpgFault*
PyTpgFault_AsTpgFaultPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyFaultMgr: 故障を管理するクラス
//////////////////////////////////////////////////////////////////////

/// @brief FaultMgr を表すタイプオブジェクト
extern
PyTypeObject PyFaultMgr_Type;

/// @brief PyFaultMgr の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyFaultMgr_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyFaultMgr_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyFaultMgr_Type;
}

/// @brief PyObject から FaultMgr へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return FaultMgr へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
FaultMgr*
PyFaultMgr_AsFaultMgrPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyTestVector: テストベクタを表す型
//////////////////////////////////////////////////////////////////////

/// @brief TestVector を表すタイプオブジェクト
extern
PyTypeObject PyTestVector_Type;

/// @brief PyTestVector の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyTestVector_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyTestVector_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyTestVector_Type;
}

/// @brief TestVector から PyObject を生成する．
/// @param[in] obj TestVector オブジェクト
extern
PyObject*
PyTestVector_FromTestVector(TestVector* obj);

/// @brief PyObject から TestVector へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return TestVector へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
TestVector*
PyTestVector_AsTestVectorPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyTvMgr: テストベクタを管理するクラス
//////////////////////////////////////////////////////////////////////

/// @brief TvMgr を表すタイプオブジェクト
extern
PyTypeObject PyTvMgr_Type;

/// @brief PyTvMgr の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyTvMgr_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyTvMgr_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyTvMgr_Type;
}

/// @brief PyObject から TvMgr へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return TvMgr へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
TvMgr*
PyTvMgr_AsTvMgrPtr(PyObject* py_obj);


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
