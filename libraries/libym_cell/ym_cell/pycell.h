#ifndef YM_CELL_PYCELL_H
#define YM_CELL_PYCELL_H

/// @file ym_cell/pycell.h
/// @brief libym_cell の Python 用拡張モジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "ym_cell/CellArea.h"


BEGIN_NAMESPACE_YM_PYTHON

//////////////////////////////////////////////////////////////////////
// 型を表すタイプオブジェクト
//////////////////////////////////////////////////////////////////////

/// @brief CellArea を表す型
extern
PyTypeObject CellAreaType;

/// @brief CellArea の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellAreaInf;


/// @brief CellCapacitance を表す型
extern
PyTypeObject CellCapacitanceType;

/// @brief CellCapacitance の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellCapacitanceInf;


/// @brief CellTime を表す型
extern
PyTypeObject CellTimeType;

/// @brief CellTime の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellTimeInf;


//////////////////////////////////////////////////////////////////////
// 型をチェックする関数
//////////////////////////////////////////////////////////////////////

/// @brief CellAreaType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellAreaType だった．
/// @retval false obj が他の型だった．
inline
bool
CellAreaObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &CellAreaType;
}

/// @brief CellCapacitanceType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellCapacitanceType だった．
/// @retval false obj が他の型だった．
inline
bool
CellCapacitanceObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &CellCapacitanceType;
}

/// @brief CellTimeType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellTimeType だった．
/// @retval false obj が他の型だった．
inline
bool
CellTimeObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &CellTimeType;
}


//////////////////////////////////////////////////////////////////////
// PyObject からの型変換
//////////////////////////////////////////////////////////////////////

/// @brief PyObject から CellArea を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj CellArea を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が CellAreaObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   CellArea& obj);

/// @brief PyObject から CellCapacitance を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj CellCapacitance を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が CellCapacitanceObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   CellCapacitance& obj);

/// @brief PyObject から CellTime を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj CellTime を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が CellTimeObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   CellTime& obj);


//////////////////////////////////////////////////////////////////////
// PyObject への型変換
//////////////////////////////////////////////////////////////////////

/// @brief CellArea から CellAreaObject を生成する．
/// @param[in] obj CellArea オブジェクト
extern
PyObject*
CellArea_FromCellArea(const CellArea& obj);

/// @brief double から CellAreaObject を生成する．
/// @param[in] val 値
extern
PyObject*
CellArea_FromDouble(double val);

/// @brief CellCapacitance から CellCapacitanceObject を生成する．
/// @param[in] obj CellCapacitance オブジェクト
extern
PyObject*
CellCapacitance_FromCellCapacitance(const CellCapacitance& obj);

/// @brief double から CellCapacitanceObject を生成する．
/// @param[in] val 値
extern
PyObject*
CellCapacitance_FromDouble(double val);

/// @brief CellTime から CellTimeObject を生成する．
/// @param[in] obj CellTime オブジェクト
extern
PyObject*
CellTime_FromCellTime(const CellTime& obj);

/// @brief double から CellTimeObject を生成する．
/// @param[in] val 値
extern
PyObject*
CellTime_FromDouble(double val);

END_NAMESPACE_YM_PYTHON


//////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////

BEGIN_EXTERN_C

void
cell_init();

END_EXTERN_C


#endif // YM_CELL_PYCELL_H
