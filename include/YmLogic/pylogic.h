#ifndef YM_YMYMLOGIC_PYYMYMLOGIC_H
#define YM_YMYMLOGIC_PYYMYMLOGIC_H

/// @file ym_YmLogic/pylogic.h
/// @brief libym_logic の Python 用の拡張モジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "ym_YmLogic/Bool3.h"
#include "ym_YmLogic/VarId.h"
#include "ym_YmLogic/Pol.h"
#include "ym_YmLogic/Literal.h"
#include "ym_YmLogic/TvFunc.h"
#include "ym_YmLogic/lexp_nsdef.h"
#include "ym_YmLogic/bdd_nsdef.h"
#include "ym_YmLogic/aig_nsdef.h"
#include "ym_YmLogic/sat_nsdef.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// PyBool3: 3値の論理値を表す型
//////////////////////////////////////////////////////////////////////

/// @brief Bool3 を表すタイプオブジェクト
extern
PyTypeObject PyBool3_Type;

/// @brief PyBool3 の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyBool3_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyBool3_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyBool3_Type;
}

/// @brief Bool3 から PyObject を生成する．
/// @param[in] obj Bool3 オブジェクト
extern
PyObject*
PyBool3_FromBool3(Bool3 obj);

/// @brief 文字列から Bool3Object への変換関数
/// @param[in] str 値を表す文字列("true"|"false"|"x")
/// @note 不正な文字列が与えられた場合には NULL を返す．
PyObject*
PyBool3_FromString(const char* str);

/// @brief long から Bool3Object への変換関数
/// @param[in] val 値
/// @note 0 を kB3False, それ以外を kB3True に対応させる．
PyObject*
PyBool3_FromLong(ymlong val);

/// @brief PyObject から Bool3 を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return Bool3 を返す．
/// @note 変換が失敗したら TypeError を送出し，kB3X を返す．
extern
Bool3
PyBool3_AsBool3(PyObject* py_obj);

/// @brief kB3True を表す定数オブジェクト
extern
PyObject* Py_kB3True;

/// @brief kB3False を表す定数オブジェクト
extern
PyObject* Py_kB3False;

/// @brief kB3X を表す定数オブジェクト
extern
PyObject* Py_kB3X;


//////////////////////////////////////////////////////////////////////
// PyVarId: 変数番号を表す型
//////////////////////////////////////////////////////////////////////

/// @brief VarId を表すタイプオブジェクト
extern
PyTypeObject PyVarId_Type;

/// @brief PyVarId の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyVarId_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyVarId_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyVarId_Type;
}

/// @brief VarId から PyObject を生成する．
/// @param[in] obj VarId オブジェクト
extern
PyObject*
PyVarId_FromVarId(VarId obj);

/// @brief PyObject から VarId を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return VarId を返す．
/// @note 変換が失敗したら TypeError を送出し，VarId(0) を返す．
extern
VarId
PyVarId_AsVarId(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyPol: 極性を表す型
//////////////////////////////////////////////////////////////////////

/// @brief tPol を表すタイプオブジェクト
extern
PyTypeObject PyPol_Type;

/// @brief PyPol の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyPol_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyPol_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyPol_Type;
}

/// @brief tPol から PyObject を生成する．
/// @param[in] obj tPol オブジェクト
extern
PyObject*
PyPol_FromPol(tPol obj);

/// @brief 文字列から PolObject への変換関数
/// @param[in] str 極性を表す文字列("positive"|"negative")
/// @note 不正な文字列が与えられた場合には NULL を返す．
PyObject*
PyPol_FromString(const char* str);

/// @brief PyObject から tPol を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return tPol を返す．
/// @note 変換が失敗したら TypeError を送出し，kPolPosi を返す．
extern
tPol
PyPol_AsPol(PyObject* py_obj);

/// @brief kPolPosi を表すオブジェクト
extern
PyObject* Py_kPolPosi;

/// @brief kPolNega を表すオブジェクト
extern
PyObject* Py_kPolNega;


//////////////////////////////////////////////////////////////////////
// PyLiteral: リテラルを表す型
//////////////////////////////////////////////////////////////////////

/// @brief Literal を表すタイプオブジェクト
extern
PyTypeObject PyLiteral_Type;

/// @brief PyLiteral の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyLiteral_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyLiteral_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyLiteral_Type;
}

/// @brief Literal から PyObject を生成する．
/// @param[in] obj Literal オブジェクト
extern
PyObject*
PyLiteral_FromLiteral(Literal obj);

/// @brief PyObject から Literal を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return Literal を返す．
/// @note 変換が失敗したら TypeError を送出し，Literal(VarId(0), kPolPosi) を返す．
extern
Literal
PyLiteral_AsLiteral(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyLogExpr: 論理式を表す型
//////////////////////////////////////////////////////////////////////

/// @brief LogExpr を表すタイプオブジェクト
extern
PyTypeObject PyLogExpr_Type;

/// @brief PyLogExpr の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyLogExpr_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyLogExpr_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyLogExpr_Type;
}

/// @brief LogExpr から PyObject を生成する．
/// @param[in] obj LogExpr オブジェクト
extern
PyObject*
PyLogExpr_FromLogExpr(const LogExpr& obj);

/// @brief PyObject から LogExpr へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return LogExpr へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
LogExpr*
PyLogExpr_AsLogExprPtr(PyObject* py_obj);

/// @brief 定数0の式
extern
PyObject* Py_kLogExprConst0;

/// @brief 定数1の式
extern
PyObject* Py_kLogExprConst1;


//////////////////////////////////////////////////////////////////////
// PyTvFunc: 真理値表形式の関数を表す型
//////////////////////////////////////////////////////////////////////

/// @brief TvFunc を表すタイプオブジェクト
extern
PyTypeObject PyTvFunc_Type;

/// @brief PyTvFunc の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyTvFunc_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyTvFunc_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyTvFunc_Type;
}

/// @brief TvFunc から PyObject を生成する．
/// @param[in] obj TvFunc オブジェクト
extern
PyObject*
PyTvFunc_FromTvFunc(const TvFunc& obj);

/// @brief PyObject から TvFunc へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return TvFunc へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
TvFunc*
PyTvFunc_AsTvFuncPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyBdd: BDD を表す型
//////////////////////////////////////////////////////////////////////

/// @brief Bdd を表すタイプオブジェクト
extern
PyTypeObject PyBdd_Type;

/// @brief PyBdd の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyBdd_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyBdd_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyBdd_Type;
}

/// @brief Bdd から PyObject を生成する．
/// @param[in] obj Bdd オブジェクト
extern
PyObject*
PyBdd_FromBdd(const Bdd& obj);

/// @brief PyObject から Bdd を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return Bdd を返す．
/// @note 変換が失敗したら TypeError を送出し，0のBDD を返す．
extern
Bdd
PyBdd_AsBdd(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyBddMgr: BDD マネージャを表す型
//////////////////////////////////////////////////////////////////////

/// @brief BddMgr を表すタイプオブジェクト
extern
PyTypeObject PyBddMgr_Type;

/// @brief PyBddMgr の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyBddMgr_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyBddMgr_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyBddMgr_Type;
}

/// @brief PyObject から BddMgr へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return BddMgr へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
BddMgr*
PyBddMgr_AsBddMgrPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyAig: AIG を表す型
//////////////////////////////////////////////////////////////////////

/// @brief Aig を表すタイプオブジェクト
extern
PyTypeObject PyAig_Type;

/// @brief PyAig の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyAig_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyAig_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyAig_Type;
}

/// @brief Aig から PyObject を生成する．
/// @param[in] obj Aig オブジェクト
extern
PyObject*
PyAig_FromAig(const Aig& obj);

/// @brief PyObject から Aig を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return Aig を返す．
/// @note 変換が失敗したら TypeError を送出し，適当な値を返す．
extern
Aig
PyAig_AsAig(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyAigMgr: AIG マネージャを表す型
//////////////////////////////////////////////////////////////////////

/// @brief AigMgr を表すタイプオブジェクト
extern
PyTypeObject PyAigMgr_Type;

/// @brief PyAigMgr の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyAigMgr_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyAigMgr_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyAigMgr_Type;
}

/// @brief PyObject から AigMgr へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return AigMgr へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
AigMgr*
PyAigMgr_AsAigMgrPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PySatSolver: SAT ソルバを表す型
//////////////////////////////////////////////////////////////////////

/// @brief SatSolver を表すタイプオブジェクト
extern
PyTypeObject PySatSolver_Type;

/// @brief PySatSolver の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PySatSolver_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PySatSolver_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PySatSolver_Type;
}

/// @brief PyObject から SatSolver へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return SatSolver へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
SatSolver*
PySatSolver_AsSatSolverPtr(PyObject* py_obj);

END_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////

BEGIN_EXTERN_C

void
logic_init();

END_EXTERN_C

#endif // YM_YMYMLOGIC_PYYMYMLOGIC_H
