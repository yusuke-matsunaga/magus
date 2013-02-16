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
#include "ym_cell/CellCapacitance.h"
#include "ym_cell/CellTime.h"
#include "ym_cell/CellLut.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellGroup.h"
#include "ym_cell/CellClass.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// 型を表すタイプオブジェクト
//////////////////////////////////////////////////////////////////////

/// @brief CellArea を表す型
extern
PyTypeObject PyCellArea_Type;

/// @brief CellArea の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellAreaInf;

/// @brief CellCapacitance を表す型
extern
PyTypeObject PyCellCapacitance_Type;

/// @brief CellCapacitance の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellCapacitanceInf;

/// @brief CellTime を表す型
extern
PyTypeObject PyCellTime_Type;

/// @brief CellTime の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellTimeInf;

/// @brief CellLutTemplate を表す型
extern
PyTypeObject PyCellLutTemplate_Type;

/// @brief CellLut を表す型
extern
PyTypeObject PyCellLut_Type;

/// @brief Cell を表す型
extern
PyTypeObject PyCell_Type;

/// @breif CellPin を表す型
extern
PyTypeObject PyCellPin_Type;

/// @brief CellGroup を表す型
extern
PyTypeObject PyCellGroup_Type;

/// @brief CellClass を表す型
extern
PyTypeObject PyCellClass_Type;

/// @brief CellLibrary を表す型
extern
PyTypeObject PyCellLibrary_Type;


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
  return Py_TYPE(obj) == &PyCellArea_Type;
}

/// @brief CellCapacitanceType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellCapacitanceType だった．
/// @retval false obj が他の型だった．
inline
bool
CellCapacitanceObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellCapacitance_Type;
}

/// @brief CellTimeType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellTimeType だった．
/// @retval false obj が他の型だった．
inline
bool
CellTimeObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellTime_Type;
}

/// @brief CellLutTemplateType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellLutTemplateType だった．
/// @retval false obj が他の型だった．
inline
bool
CellLutTempalteObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellLutTemplate_Type;
}

/// @brief CellLutType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellLutType だった．
/// @retval false obj が他の型だった．
inline
bool
CellLutObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellLut_Type;
}

/// @brief CellType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellType だった．
/// @retval false obj が他の型だった．
inline
bool
CellObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCell_Type;
}

/// @brief CellPinType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellPinType だった．
/// @retval false obj が他の型だった．
inline
bool
CellPinObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellPin_Type;
}

/// @brief CellGroupType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellGroupType だった．
/// @retval false obj が他の型だった．
inline
bool
CellGroupObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellGroup_Type;
}

/// @brief CellClassType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellClassType だった．
/// @retval false obj が他の型だった．
inline
bool
CellClassObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellClass_Type;
}

/// @brief CellLibraryType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が CellLibraryType だった．
/// @retval false obj が他の型だった．
inline
bool
CellLibraryObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellLibrary_Type;
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

/// @brief PyObject から CellLutTemplate を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj CellLutTemplate のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が CellLutTemplateObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   const CellLutTemplate*& p_obj);

/// @brief PyObject から CellLut を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj CellLut のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が CellLutObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   const CellLut*& p_obj);

/// @brief PyObject から Cell を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj Cell のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が CellObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   const Cell*& p_obj);

/// @brief PyObject から CellGroup を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj CellGroup のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が CellGroupObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   const CellGroup*& p_obj);

/// @brief PyObject から CellClass を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj CellClass のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が CellClassObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   const CellClass*& p_obj);

/// @brief PyObject から CellLibrary を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj CellLibrary のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が CellLibraryObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   const CellLibrary*& p_obj);


//////////////////////////////////////////////////////////////////////
// PyObject への型変換
//////////////////////////////////////////////////////////////////////

/// @brief CellArea から CellAreaObject を生成する．
/// @param[in] obj CellArea オブジェクト
extern
PyObject*
PyCellArea_FromCellArea(const CellArea& obj);

/// @brief double から CellAreaObject を生成する．
/// @param[in] val 値
extern
PyObject*
PyCellArea_FromDouble(double val);

/// @brief CellCapacitance から CellCapacitanceObject を生成する．
/// @param[in] obj CellCapacitance オブジェクト
extern
PyObject*
PyCellCapacitance_FromCellCapacitance(const CellCapacitance& obj);

/// @brief double から CellCapacitanceObject を生成する．
/// @param[in] val 値
extern
PyObject*
PyCellCapacitance_FromDouble(double val);

/// @brief CellTime から CellTimeObject を生成する．
/// @param[in] obj CellTime オブジェクト
extern
PyObject*
PyCellTime_FromCellTime(const CellTime& obj);

/// @brief double から CellTimeObject を生成する．
/// @param[in] val 値
extern
PyObject*
PyCellTime_FromDouble(double val);

/// @brief CellLutTemplate から CellLutTemplateObject を生成する．
/// @param[in] lut_template LUTテンプレート
extern
PyObject*
PyCellLutTemplate_FromCellLutTemplate(const CellLutTemplate* lut_template);

/// @brief CellLut から CellLutObject を生成する．
/// @param[in] lut LUT
extern
PyObject*
PyCellLut_FromCellLut(const CellLut* lut);

/// @brief Cell から CellObject を生成する．
/// @param[in] cell セル
extern
PyObject*
PyCell_FromCell(const Cell* cell);

/// @brief CellGroup から CellGroupObject を生成する．
/// @param[in] group グループ
extern
PyObject*
PyCellGroup_FromCellGroup(const CellGroup* group);

/// @brief CellClass から CellClassObject を生成する．
/// @param[in] cell_class セルクラス
extern
PyObject*
PyCellClass_FromCellClass(const CellClass* cell_class);

/// @brief CellLibrary から CellLibraryObject を生成する．
/// @param[in] cell_class セルクラス
extern
PyObject*
PyCellLibrary_FromCellLibrary(const CellLibrary* cell_class);


END_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////

BEGIN_EXTERN_C

void
cell_init();

END_EXTERN_C


#endif // YM_CELL_PYCELL_H
