#ifndef YM_LOGIC_PYLOGIC_H
#define YM_LOGIC_PYLOGIC_H

/// @file ym_logic/logicpy.h
/// @brief libym_logic の Python 用の拡張モジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "ym_logic/Bool3.h"
#include "ym_logic/VarId.h"
#include "ym_logic/Pol.h"
#include "ym_logic/Literal.h"
#include "ym_logic/lexp_nsdef.h"
#include "ym_logic/bdd_nsdef.h"
#include "ym_logic/aig_nsdef.h"
#include "ym_logic/sat_nsdef.h"


BEGIN_NAMESPACE_YM_PYTHON

//////////////////////////////////////////////////////////////////////
// 型を表すタイプオブジェクト
//////////////////////////////////////////////////////////////////////

/// @brief Bool3 を表す型
extern
PyTypeObject Bool3Type;

/// @brief kB3True を表すオブジェクト
extern
PyObject* Py_kB3True;

/// @brief kB3False を表すオブジェクト
extern
PyObject* Py_kB3False;

/// @brief kB3X を表すオブジェクト
extern
PyObject* Py_kB3X;

/// @brief VarId を表す型
extern
PyTypeObject VarIdType;

/// @brief tPol を表す型
extern
PyTypeObject PolType;

/// @brief kPolPosi を表すオブジェクト
extern
PyObject* Py_kPolPosi;

/// @brief kPolNega を表すオブジェクト
extern
PyObject* Py_kPolNega;

/// @brief Literal を表す型
extern
PyTypeObject LiteralType;

/// @brief LogExpr を表す型
extern
PyTypeObject LogExprType;

/// @brief 定数0の式
extern
PyObject* Py_kLogExprConst0;

/// @brief 定数1の式
extern
PyObject* Py_kLogExprConst1;

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

/// @brief Bool3Type の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が Bool3Type だった．
/// @retval false obj が他の型だった．
inline
bool
Bool3Object_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &Bool3Type;
}

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

/// @brief PyObject から Bool3 を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj Bool3 を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が Bool3Object ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   Bool3& obj);

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
		   BddMgr*& obj_p);

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
		   AigMgr*& obj);

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
/// @param[out] p_obj SatSolver のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が SatSolverObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   SatSolver*& p_obj);


//////////////////////////////////////////////////////////////////////
// PyObject への型変換
//////////////////////////////////////////////////////////////////////

/// @brief Bool3 から PyObject を生成する．
/// @param[in] obj Bool3 オブジェクト
extern
PyObject*
Bool3_FromBool3(Bool3 obj);

/// @brief 文字列から Bool3Object への変換関数
/// @param[in] str 値を表す文字列("true"|"false"|"x")
/// @note 不正な文字列が与えられた場合には NULL を返す．
PyObject*
Bool3_FromString(const char* str);

/// @brief long から Bool3Object への変換関数
/// @param[in] val 値
/// @note 0 を kB3False, それ以外を kB3True に対応させる．
PyObject*
Bool3_FromLong(ymlong val);

/// @brief VarId から PyObject を生成する．
/// @param[in] obj VarId オブジェクト
extern
PyObject*
VarId_FromVarId(VarId obj);

/// @brief tPol から PyObject を生成する．
/// @param[in] obj tPol オブジェクト
extern
PyObject*
Pol_FromPol(tPol obj);

/// @brief 文字列から PolObject への変換関数
/// @param[in] str 極性を表す文字列("positive"|"negative")
/// @note 不正な文字列が与えられた場合には NULL を返す．
PyObject*
Pol_FromString(const char* str);

/// @brief Literal から PyObject を生成する．
/// @param[in] obj Literal オブジェクト
extern
PyObject*
Literal_FromLiteral(Literal obj);

/// @brief LogExpr から PyObject を生成する．
/// @param[in] obj LogExpr オブジェクト
extern
PyObject*
LogExpr_FromLogExpr(const LogExpr& obj);

/// @brief Bdd から PyObject を生成する．
/// @param[in] obj Bdd オブジェクト
extern
PyObject*
Bdd_FromBdd(const Bdd& obj);

/// @brief Aig から PyObject を生成する．
/// @param[in] obj Aig オブジェクト
extern
PyObject*
Aig_FromAig(const Aig& obj);

END_NAMESPACE_YM_PYTHON


//////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////

BEGIN_EXTERN_C

void
logic_init();

END_EXTERN_C

#endif // YM_LOGIC_PYLOGIC_H
