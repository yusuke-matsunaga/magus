#ifndef YM_CELL_PYCELL_H
#define YM_CELL_PYCELL_H

/// @file ym_cell/pycell.h
/// @brief libym_cell の Python 用拡張モジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
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
// PyCellArea: セル面積を表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellArea を表すタイプオブジェクト
extern
PyTypeObject PyCellArea_Type;

/// @brief PyCellArea の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellArea_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellArea_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellArea_Type;
}

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

/// @brief PyObject から CellArea を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellArea を返す．
/// @note 変換が失敗したら TypeError を送出し，CellArea(0.0) を返す．
extern
CellArea
PyCellArea_AsCellArea(PyObject* py_obj);

/// @brief PyObject から double を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return double を返す．
/// @note 変換が失敗したら TypeError を送出し，0.0 を返す．
extern
double
PyCellArea_AsDouble(PyObject* py_obj);

/// @brief CellArea の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellAreaInf;


//////////////////////////////////////////////////////////////////////
// PyCellCapacitance: 容量を表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellCapacitance を表すタイプオブジェクト
extern
PyTypeObject PyCellCapacitance_Type;

/// @brief PyCellCapacitance の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellCapacitance_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellCapacitance_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellCapacitance_Type;
}

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

/// @brief PyObject から CellCapacitance を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellCapacitance を返す．
/// @note 変換が失敗したら TypeError を送出し，kCellCapacitanceInf を返す．
extern
CellCapacitance
PyCellCapacitance_AsCellCapacitance(PyObject* py_obj);

/// @brief PyObject から double を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return doubleを返す．
/// @note 変換が失敗したら TypeError を送出し，0.0 を返す．
extern
double
PyCellCapacitance_AsDouble(PyObject* py_obj);

/// @brief CellCapacitance の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellCapacitanceInf;


//////////////////////////////////////////////////////////////////////
// PyCellTime: 時間を表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellTime を表す型
extern
PyTypeObject PyCellTime_Type;

/// @brief PyCellTime の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellTime_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellTime_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellTime_Type;
}

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

/// @brief PyObject から CellTime を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellTime を返す．
/// @note 変換が失敗したら TypeError を送出し，kCellTimeInf を返す．
extern
CellTime
PyCellTime_AsCellTime(PyObject* py_obj);

/// @brief CellTime の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellTimeInf;


//////////////////////////////////////////////////////////////////////
// PyCellLutTemplate: LUT のテンプレートを表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellLutTemplate を表すタイプオブジェクト
extern
PyTypeObject PyCellLutTemplate_Type;

/// @brief PyCellLutTemplate の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellLutTemplate_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellLutTempalte_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellLutTemplate_Type;
}

/// @brief CellLutTemplate から CellLutTemplateObject を生成する．
/// @param[in] lut_template LUTテンプレート
extern
PyObject*
PyCellLutTemplate_FromCellLutTemplate(const CellLutTemplate* lut_template);

/// @brief PyObject から CellLutTemplate へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellLutTemplate へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellLutTemplate*
PyCellLutTemplate_AsCellLutTemplatePtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyCellLut: LUT を表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellLut を表すタイプオブジェクト
extern
PyTypeObject PyCellLut_Type;

/// @brief PyCellLut の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellLut_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellLut_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellLut_Type;
}

/// @brief CellLut から CellLutObject を生成する．
/// @param[in] lut LUT
extern
PyObject*
PyCellLut_FromCellLut(const CellLut* lut);

/// @brief PyObject から CellLut へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellLut へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellLut*
PyCellLut_AsCellLutPtr(PyObject* py_obj);


#if 0
//////////////////////////////////////////////////////////////////////
// PyCell: セルを表す型
//////////////////////////////////////////////////////////////////////

/// @brief Cell を表すタイプオブジェクト
extern
PyTypeObject PyCell_Type;

/// @brief PyCell の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCell_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCell_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCell_Type;
}

/// @brief PyObject から Cell へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return Cell へんおポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const Cell*
PyCell_AsCellPtr(PyObject* py_obj);
#endif

//////////////////////////////////////////////////////////////////////
// PyCellPin: セルのピンを表す型
//////////////////////////////////////////////////////////////////////

/// @breif CellPin を表すタイプオブジェクト
extern
PyTypeObject PyCellPin_Type;

/// @brief PyCellPin の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellPin_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellPin_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellPin_Type;
}


//////////////////////////////////////////////////////////////////////
// PyCellGroup: セルグループを表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellGroup を表すタイプオブジェクト
extern
PyTypeObject PyCellGroup_Type;

/// @brief PyCellGroup の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellGroup_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellGroup_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellGroup_Type;
}

/// @brief CellGroup から CellGroupObject を生成する．
/// @param[in] group グループ
extern
PyObject*
PyCellGroup_FromCellGroup(const CellGroup* group);

/// @brief PyObject から CellGroup へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellGroup へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellGroup*
PyCellGroup_AsCellGroupPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyCellClass: セルクラスを表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellClass を表すタイプオブジェクト
extern
PyTypeObject PyCellClass_Type;

/// @brief PyCellClass の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellClass_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellClass_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellClass_Type;
}

/// @brief CellClass から CellClassObject を生成する．
/// @param[in] cell_class セルクラス
extern
PyObject*
PyCellClass_FromCellClass(const CellClass* cell_class);

/// @brief PyObject から CellClass へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellClass へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellClass*
PyCellClass_AsCellClassPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyCellLibrary: セルライブラリを表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellLibrary を表すタイプオブジェクト
extern
PyTypeObject PyCellLibrary_Type;

/// @brief PyCellLibrary の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellLibrary_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellLibrary_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellLibrary_Type;
}

/// @brief CellLibrary から CellLibraryObject を生成する．
/// @param[in] cell_class セルクラス
extern
PyObject*
PyCellLibrary_FromCellLibrary(const CellLibrary* cell_class);

/// @brief PyObject から CellLibrary へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellLibrary へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellLibrary*
PyCellLibrary_AsCellLibraryPtr(PyObject* py_obj);


END_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////

BEGIN_EXTERN_C

void
cell_init();

END_EXTERN_C


#endif // YM_CELL_PYCELL_H
