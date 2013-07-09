#ifndef PYIGF_H
#define PYIGF_H

/// @file pyigf.h
/// @brief libphf の Python 用拡張モジュール
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// PyFuncVect: FuncVect の Python 用インターフェイス
//////////////////////////////////////////////////////////////////////

/// @brief FuncVect を表すタイプオブジェクト
extern
PyTypeObject PyFuncVect_Type;

/// @brief PyFuncVect の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyFuncVect 型だった．
/// @retval false obj が他の型だった．
inline
bool
PyFuncVect_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyFuncVect_Type;
}

/// @brief FuncVect から Python オブジェクトを生成する．
/// @param[in] obj FuncVect オブジェクト
/// @return 変換した Python オブジェクトを返す．
extern
PyObject*
PyFuncVect_FromFuncVect(const FuncVect& obj);

/// @brief Python オブジェクトから FuncVect を取り出す．
/// @param[in] py_obj Ptyhon オブジェクト
/// @return FuncVect へのポインタを返す．
extern
FuncVect*
PyFuncVect_AsFuncVect(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyRegVect: RegVect の Python 用インターフェイス
//////////////////////////////////////////////////////////////////////

/// @brief RegVect を表すタイプオブジェクト
extern
PyTypeObject PyRegVect_Type;

/// @brief PyRegVect の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyRegVect 型だった．
/// @retval false obj が他の型だった．
inline
bool
PyRegVect_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyRegVect_Type;
}

/// @brief RegVect から Python オブジェクトを生成する．
/// @param[in] obj RegVect オブジェクト
/// @return 変換した Python オブジェクトを返す．
extern
PyObject*
PyRegVect_FromRegVect(const RegVect* obj);

/// @brief Python オブジェクトから RegVect を取り出す．
/// @param[in] py_obj Ptyhon オブジェクト
/// @return RegVect へのポインタを返す．
extern
const RegVect*
PyRegVect_AsRegVect(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyRvMgr: RvMgr の Python 用インターフェイス
//////////////////////////////////////////////////////////////////////

/// @brief RvMgr を表すタイプオブジェクト
extern
PyTypeObject PyRvMgr_Type;

/// @brief PyRvMgr の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyRvMgr 型だった．
/// @retval false obj が他の型だった．
inline
bool
PyRvMgr_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyRvMgr_Type;
}

/// @brief RvMgr から Python オブジェクトを生成する．
/// @param[in] obj RvMgr オブジェクト
/// @return 変換した Python オブジェクトを返す．
extern
PyObject*
PyRvMgr_FromRvMgr(const RvMgr* obj);

/// @brief Python オブジェクトから RvMgr を取り出す．
/// @param[in] py_obj Ptyhon オブジェクト
/// @return RvMgr へのポインタを返す．
extern
const RvMgr*
PyRvMgr_AsRvMgr(PyObject* py_obj);



#if 0
//////////////////////////////////////////////////////////////////////
// PyPhfGraph: PhfGraph の Python 用インターフェイス
//////////////////////////////////////////////////////////////////////

/// @brief PhfGraph を表すタイプオブジェクト
extern
PyTypeObject PyPhfGraph_Type;

/// @brief PyPhfGraph の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyPhfGraph 型だった．
/// @retval false obj が他の型だった．
inline
bool
PyPhfGraph_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyPhfGraph_Type;
}

/// @brief PyObject から PhfGraph を取り出す．
extern
PhfGraph*
PyPhfGraph_AsPhfGraph(PyObject* py_obj);
#endif

END_NAMESPACE_YM_IGF

#endif // PYIGF_H
