#ifndef LOGICPY_H
#define LOGICPY_H

/// @file logicpy.h
/// @brief libym_logic の Python 用の拡張モジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.

/// 基本的にこのファイルはローカルなのでユーザーが参照することはない．
/// 内部でよく用いる定義がまとめてあるだけ．


#include "Python.h"
#include "ym_logic/VarId.h"
#include "ym_logic/Pol.h"
#include "ym_logic/Literal.h"
#include "ym_logic/lexp_nsdef.h"
#include "ym_logic/bdd_nsdef.h"
#include "ym_logic/aig_nsdef.h"
#include "ym_logic/sat_nsdef.h"


//////////////////////////////////////////////////////////////////////
// 名前空間の定義
//////////////////////////////////////////////////////////////////////

/// @brief logicpy 用の名前空間の開始
#define BEGIN_NAMESPACE_LOGICPY			\
  BEGIN_NAMESPACE_YM \
  BEGIN_NAMESPACE(nsLogicpy)

/// @brief logicpy 用の名前空間の終了
#define END_NAMESPACE_LOGICPY			\
  END_NAMESPACE(nsLogicpy)			\
  END_NAMESPACE_YM


BEGIN_NAMESPACE_LOGICPY


//////////////////////////////////////////////////////////////////////
// 共通のオブジェクトの宣言
//////////////////////////////////////////////////////////////////////

/// @brief エラー用のオブジェクト
extern
PyObject* ErrorObject;


//////////////////////////////////////////////////////////////////////
// 型を表すタイプオブジェクト
//////////////////////////////////////////////////////////////////////

/// @brief VarId を表す型
extern
PyTypeObject VarIdType;

/// @brief tPol を表す型
extern
PyTypeObject PolType;

/// @brief Literal を表す型
extern
PyTypeObject LiteralType;

/// @brief LogExpr を表す型
extern
PyTypeObject LogExprType;

/// @brief BddMgr を表す型
extern
PyTypeObject BddMgrType;

/// @brief Bdd を表す型
extern
PyTypeObject BddType;

/// @brief AigMgr を表す型
extern
PyTypeObject AigMgrType;

/// @brief Aig を表す型
extern
PyTypeObject AigType;

/// @brief SatSolver を表す型
extern
PyTypeObject SatSolverType;


//////////////////////////////////////////////////////////////////////
// 型をチェックする関数
//////////////////////////////////////////////////////////////////////

/// @brief VarIdType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が VarIdType だった．
/// @retval false obj が他の型だった．
inline
bool
VarIdObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &VarIdType;
}

/// @brief PolType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PolType だった．
/// @retval false obj が他の型だった．
inline
bool
PolObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PolType;
}

/// @brief LiteralType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が LiteralType だった．
/// @retval false obj が他の型だった．
inline
bool
LiteralObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &LiteralType;
}

/// @brief LogExprType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が LogExprType だった．
/// @retval false obj が他の型だった．
inline
bool
LogExprObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &LogExprType;
}

/// @brief BddMgrType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が BddMgrType だった．
/// @retval false obj が他の型だった．
inline
bool
BddMgrObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &BddMgrType;
}

/// @brief BddType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が BddType だった．
/// @retval false obj が他の型だった．
inline
bool
BddObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &BddType;
}

/// @brief AigMgrType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が AigMgrType だった．
/// @retval false obj が他の型だった．
inline
bool
AigMgrObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &AigMgrType;
}

/// @brief AigType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が AigType だった．
/// @retval false obj が他の型だった．
inline
bool
AigObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &AigType;
}

/// @brief SatSolverType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が SatSolverType だった．
/// @retval false obj が他の型だった．
inline
bool
SatSolverObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &SatSolverType;
}


//////////////////////////////////////////////////////////////////////
// PyObject からの型変換
//////////////////////////////////////////////////////////////////////

/// @brief PyObject から VarId を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj VarId を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が VarIdObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   VarId& obj);

/// @brief PyObject から tPol を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj tPol を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が PolObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   tPol& obj);

/// @brief PyObject から Literal を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj Literal を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が LiteralObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   Literal& obj);

/// @brief PyObject から LogExpr を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj LogExpr を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が LogExprObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   LogExpr& obj);

/// @brief PyObject から BddMgr を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj BddMgr を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が BddMgrObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   BddMgr* obj_p);

/// @brief PyObject から Bdd を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj Bdd を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が BddObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   Bdd& obj);

/// @brief PyObject から AigMgr を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj AigMgr を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が AigMgrObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   AigMgr& obj);

/// @brief PyObject から Aig を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj Aig を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が AigObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   Aig& obj);

/// @brief PyObject から SatSolver を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj SatSolver を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が SatSolverObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   SatSolver& obj);


//////////////////////////////////////////////////////////////////////
// PyObject への型変換
//////////////////////////////////////////////////////////////////////

/// @brief VarId から PyObject を生成する．
/// @param[in] obj VarId オブジェクト
extern
PyObject*
conv_to_pyobject(VarId obj);

/// @brief tPol から PyObject を生成する．
/// @param[in] obj tPol オブジェクト
extern
PyObject*
conv_to_pyobject(tPol obj);

/// @brief Literal から PyObject を生成する．
/// @param[in] obj Literal オブジェクト
extern
PyObject*
conv_to_pyobject(Literal obj);

/// @brief LogExpr から PyObject を生成する．
/// @param[in] obj LogExpr オブジェクト
extern
PyObject*
conv_to_pyobject(const LogExpr& obj);

/// @brief Bdd から PyObject を生成する．
/// @param[in] obj Bdd オブジェクト
extern
PyObject*
conv_to_pyobject(const Bdd& obj);

/// @brief Aig から PyObject を生成する．
/// @param[in] obj Aig オブジェクト
extern
PyObject*
conv_to_pyobject(const Aig& obj);

/// @brief SatSolver から PyObject を生成する．
/// @param[in] obj SatSolver オブジェクト
extern
PyObject*
conv_to_pyobject(SatSolver obj);

END_NAMESPACE_LOGICPY

#endif // LOGICPY_H
