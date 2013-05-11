#ifndef YM_CELL_PYCELL_H
#define YM_CELL_PYCELL_H

/// @file ym_cell/pycell.h
/// @brief libym_cell の Python 用拡張モジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// PyCellPinDir: ピンの方向を表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellDir を表すタイプオブジェクト
extern
PyTypeObject PyCellPinDir_Type;

/// @brief PyCellPinDir の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellPinDir_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellPinDir_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellPinDir_Type;
}

/// @brief tCellPinDirection から CellDirObject を生成する．
/// @param[in] obj CellDir オブジェクト
extern
PyObject*
PyCellPinDir_FromCellPinDirection(tCellPinDirection dir);

/// @brief 方向を表す文字列から CellDirObject を生成する．
/// @param[in] str 文字列
extern
PyObject*
PyCellPinDir_FromString(const char* str);

/// @brief PyObject から tCellPinDirection を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return tCellPinDirection を返す．
/// @note 変換が失敗したら TypeError を送出し，kCellPinInput を返す．
extern
tCellPinDirection
PyCellPinDir_AsCellPinDirection(PyObject* py_obj);

/// @brief kCellPinInput を表すオブジェクト
extern
PyObject* Py_kCellPinInput;

/// @brief kCellPinOutput を表すオブジェクト
extern
PyObject* Py_kCellPinOutput;

/// @brief kCellPinInout を表すオブジェクト
extern
PyObject* Py_kCellPinInout;

/// @brief kCellPinInternal を表すオブジェクト
extern
PyObject* Py_kCellPinInternal;


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
/// @note 変換が失敗したら TypeError を送出し，CellTime(0.0) を返す．
extern
CellTime
PyCellTime_AsCellTime(PyObject* py_obj);

/// @brief PyObject から double を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return double を返す．
/// @note 変換が失敗したら TypeError を送出し，0.0 を返す．
extern
double
PyCellTime_AsDouble(PyObject* py_obj);

/// @brief CellTime の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellTimeInf;


//////////////////////////////////////////////////////////////////////
// PyCellResistance: 抵抗値を表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellResistance を表す型
extern
PyTypeObject PyCellResistance_Type;

/// @brief PyCellResistance の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellResistance_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellResistance_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellResistance_Type;
}

/// @brief CellResistance から CellResistanceObject を生成する．
/// @param[in] obj CellResistance オブジェクト
extern
PyObject*
PyCellResistance_FromCellResistance(const CellResistance& obj);

/// @brief double から CellResistanceObject を生成する．
/// @param[in] val 値
extern
PyObject*
PyCellResistance_FromDouble(double val);

/// @brief PyObject から CellResistance を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellResistance を返す．
/// @note 変換が失敗したら TypeError を送出し，CellResistance(0.0) を返す．
extern
CellResistance
PyCellResistance_AsCellResistance(PyObject* py_obj);

/// @brief PyObject から double を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return double を返す．
/// @note 変換が失敗したら TypeError を送出し，0.0 を返す．
extern
double
PyCellResistance_AsDouble(PyObject* py_obj);

/// @brief CellResistance の無限大定数を表すオブジェクト
extern
PyObject* Py_kCellResistanceInf;


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

/// @brief PyObject から CellLibrary へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellLibrary へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellLibrary*
PyCellLibrary_AsCellLibraryPtr(PyObject* py_obj);


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

/// @brief PyObject から CellClass へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellClass へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellClass*
PyCellClass_AsCellClassPtr(PyObject* py_obj);


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

/// @brief PyObject から CellGroup へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellGroup へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellGroup*
PyCellGroup_AsCellGroupPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyCellCell: セルを表す型
//////////////////////////////////////////////////////////////////////

/// @brief Cell を表すタイプオブジェクト
extern
PyTypeObject PyCellCell_Type;

/// @brief PyCellCell の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellCell_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellCell_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellCell_Type;
}

/// @brief PyObject から Cell へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return Cell へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const Cell*
PyCellCell_AsCellPtr(PyObject* py_obj);


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

/// @brief PyObject から CellPin へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellPin へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellPin*
PyCellPin_AsCellPinPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyCellTechnology: tCellTechnology を表す型
//////////////////////////////////////////////////////////////////////

/// @brief tCellTechnology を表すタイプオブジェクト
extern
PyTypeObject PyCellTechnology_Type;

/// @brief PyCellTechnology の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellTechnology_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellTechnology_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellTechnology_Type;
}

/// @brief tCellTechnology から PyObject を作る．
/// @param[in] delay_model tCellTechnology
/// @return delay_model を表す PyObject
extern
PyObject*
PyCellTechnology_FromCellTechnology(tCellTechnology delay_model);

/// @brief 文字列から PyObject を作る．
/// @param[in] str 文字列
/// @return delay_model を表す PyObject
extern
PyObject*
PyCellTechnology_FromString(const char* str);

/// @brief PyObject から tCellTechnology の値を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return tCellTechnology を返す．
/// @note 変換が失敗したら TypeError を送出し，kCellTechnologyGenericCmos を返す．
extern
tCellTechnology
PyCellTechnology_AsCellTechnology(PyObject* py_obj);

/// @brief kTechCmos を表す Python オブジェクト
extern
PyObject* Py_kCellTechCmos;

/// @brief kTechFpga を表す Python オブジェクト
extern
PyObject* Py_kCellTechFpga;


//////////////////////////////////////////////////////////////////////
// PyCellDelayModel: tCellDelayModel を表す型
//////////////////////////////////////////////////////////////////////

/// @brief tCellDelayModel を表すタイプオブジェクト
extern
PyTypeObject PyCellDelayModel_Type;

/// @brief PyCellDelayModel の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellDelayModel_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellDelayModel_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellDelayModel_Type;
}

/// @brief tCellDelayModel から PyObject を作る．
/// @param[in] delay_model tCellDelayModel
/// @return delay_model を表す PyObject
extern
PyObject*
PyCellDelayModel_FromCellDelayModel(tCellDelayModel delay_model);

/// @brief 文字列から PyObject を作る．
/// @param[in] str 文字列
/// @return delay_model を表す PyObject
extern
PyObject*
PyCellDelayModel_FromString(const char* str);

/// @brief PyObject から tCellDelayModel の値を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return tCellDelayModel を返す．
/// @note 変換が失敗したら TypeError を送出し，kCellDelayModelGenericCmos を返す．
extern
tCellDelayModel
PyCellDelayModel_AsCellDelayModel(PyObject* py_obj);

/// @brief kCellDelayGenericCmos を表す Python オブジェクト
extern
PyObject* Py_kCellDelayGenericCmos;

/// @brief kCellDelayTableLookup を表す Python オブジェクト
extern
PyObject* Py_kCellDelayTableLookup;

/// @brief kCellDelayPiecewiseCmos を表す Python オブジェクト
extern
PyObject* Py_kCellDelayPiecewiseCmos;

/// @brief kCellDelayCmos2 を表す Python オブジェクト
extern
PyObject* Py_kCellDelayCmos2;

/// @brief kCellDelayDcm を表す Python オブジェクト
extern
PyObject* Py_kCellDelayDcm;


//////////////////////////////////////////////////////////////////////
// PyCellPatType: CellPatType を表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellPatType を表すタイプオブジェクト
extern
PyTypeObject PyCellPatType_Type;

/// @brief PyCellPatType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellPatType_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellPatType_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellPatType_Type;
}

/// @brief tCellPatType から CellPatTypeObject を生成する．
/// @param[in] technology tCellPatType の値
extern
PyObject*
PyCellPatType_FromCellPatType(tCellPatType technology);

/// @brief 文字列から CellPatTypeObject を生成する．
/// @param[in] str 文字列
extern
PyObject*
PyCellPatType_FromString(const char* str);

/// @brief PyObject から tCellPatType を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return tCellPatType の値を返す．
/// @note 変換が失敗したら TypeError を送出し，kCellPatInput を返す．
extern
tCellPatType
PyCellPatType_AsCellPatType(PyObject* py_obj);

/// @brief kCellPatInput を表す Python オブジェクト
extern
PyObject* Py_kCellPatInput;

/// @brief kCellPatAnd を表す Python オブジェクト
extern
PyObject* Py_kCellPatAnd;

/// @brief kCellPatXor を表す Python オブジェクト
extern
PyObject* Py_kCellPatXor;


//////////////////////////////////////////////////////////////////////
// PyCellPatGraph: CellPatGraph を表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellPatGraph を表すタイプオブジェクト
extern
PyTypeObject PyCellPatGraph_Type;

/// @brief PyCellPatGraph の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellPatGraph_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellPatGraph_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellPatGraph_Type;
}

/// @brief PyObject から CellPatGraph へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellPatGraph へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellPatGraph*
PyCellPatGraph_AsCellPatGraphPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyCellTiming: CellTiming を表す型
//////////////////////////////////////////////////////////////////////

/// @brief CellTiming を表すタイプオブジェクト
extern
PyTypeObject PyCellTiming_Type;

/// @brief PyCellTiming の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellTiming_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellTiming_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellTiming_Type;
}

/// @brief CellTiming から PyObject を作る．
/// @param[in] timing CellTiming へのポインタ
/// @return timing を表す PyObject
extern
PyObject*
PyCellTiming_FromCellTiming(const CellTiming* timing);

/// @brief PyObject から CellTiming へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CellTiming へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
const CellTiming*
PyCellTiming_AsCellTimingPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyCellTimingType: tCellTimingType を表す型
//////////////////////////////////////////////////////////////////////

/// @brief tCellTimingType を表すタイプオブジェクト
extern
PyTypeObject PyCellTimingType_Type;

/// @brief PyCellTimingType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCellTimingType_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCellTimingType_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellTimingType_Type;
}

/// @brief tCellTimingType から PyObject を作る．
/// @param[in] timing_type tCellTimingType
/// @return timing_type を表す PyObject
extern
PyObject*
PyCellTimingType_FromCellTimingType(tCellTimingType timing_type);

/// @brief 文字列から PyObject を作る．
/// @param[in] str 文字列
/// @return timing_type を表す PyObject
extern
PyObject*
PyCellTimingType_FromString(const char* str);

/// @brief PyObject から tCellTimingType の値を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return tCellTimingType を返す．
/// @note 変換が失敗したら TypeError を送出し，kCellTimingCombinational を返す．
extern
tCellTimingType
PyCellTimingType_AsCellTimingType(PyObject* py_obj);

/// @brief kCellTimingCombinational を表す PyObject
extern
PyObject*
Py_kCellTimingCombinational;

/// @brief kCellTimingCombinationalRiseを表す PyObject
extern
PyObject*
Py_kCellTimingCombinationalRise;

/// @brief kCellTimingCombinationalFallを表す PyObject
extern
PyObject*
Py_kCellTimingCombinationalFall;

/// @brief kCellTimingThreeStateEnableを表す PyObject
extern
PyObject*
Py_kCellTimingThreeStateEnable;

/// @brief kCellTimingThreeStateDisableを表す PyObject
extern
PyObject*
Py_kCellTimingThreeStateDisable;

/// @brief kCellTimingThreeStateEnableRiseを表す PyObject
extern
PyObject*
Py_kCellTimingThreeStateEnableRise;

/// @brief kCellTimingThreeStateEnableFallを表す PyObject
extern
PyObject*
Py_kCellTimingThreeStateEnableFall;

/// @brief kCellTimingThreeStateDisableRiseを表す PyObject
extern
PyObject*
Py_kCellTimingThreeStateDisableRise;

/// @brief kCellTimingThreeStateDisableFallを表す PyObject
extern
PyObject*
Py_kCellTimingThreeStateDisableFall;

/// @brief kCellTimingRisingEdgeを表す PyObject
extern
PyObject*
Py_kCellTimingRisingEdge;

/// @brief kCellTimingFallingEdgeを表す PyObject
extern
PyObject*
Py_kCellTimingFallingEdge;

/// @brief kCellTimingPresetを表す PyObject
extern
PyObject*
Py_kCellTimingPreset;

/// @brief kCellTimingClearを表す PyObject
extern
PyObject*
Py_kCellTimingClear;

/// @brief kCellTimingHoldRisingを表す PyObject
extern
PyObject*
Py_kCellTimingHoldRising;

/// @brief kCellTimingHoldFallingを表す PyObject
extern
PyObject*
Py_kCellTimingHoldFalling;

/// @brief kCellTimingSetupRisingを表す PyObject
extern
PyObject*
Py_kCellTimingSetupRising;

/// @brief kCellTimingSetupFallingを表す PyObject
extern
PyObject*
Py_kCellTimingSetupFalling;

/// @brief kCellTimingRecoveryRisingを表す PyObject
extern
PyObject*
Py_kCellTimingRecoveryRising;

/// @brief kCellTimingRecoveryFallingを表す PyObject
extern
PyObject*
Py_kCellTimingRecoveryFalling;

/// @brief kCellTimingSkewRisingを表す PyObject
extern
PyObject*
Py_kCellTimingSkewRising;

/// @brief kCellTimingSkewFallingを表す PyObject
extern
PyObject*
Py_kCellTimingSkewFalling;

/// @brief kCellTimingRemovalRisingを表す PyObject
extern
PyObject*
Py_kCellTimingRemovalRising;

/// @brief kCellTimingRemovalFallingを表す PyObject
extern
PyObject*
Py_kCellTimingRemovalFalling;

/// @brief kCellTimingNonSeqSetupRisingを表す PyObject
extern
PyObject*
Py_kCellTimingNonSeqSetupRising;

/// @brief kCellTimingNonSeqSetupFallingを表す PyObject
extern
PyObject*
Py_kCellTimingNonSeqSetupFalling;

/// @brief kCellTimingNonSeqHoldRisingを表す PyObject
extern
PyObject*
Py_kCellTimingNonSeqHoldRising;

/// @brief kCellTimingNonSeqHoldFallingを表す PyObject
extern
PyObject*
Py_kCellTimingNonSeqHoldFalling;

/// @brief kCellTimingNochangeHighHighを表す PyObject
extern
PyObject*
Py_kCellTimingNochangeHighHigh;

/// @brief kCellTimingNochangeHighLowを表す PyObject
extern
PyObject*
Py_kCellTimingNochangeHighLow;

/// @brief kCellTimingNochangeLowHighを表す PyObject
extern
PyObject*
Py_kCellTimingNochangeLowHigh;

/// @brief kCellTimingNochangeLowLowを表す PyObject
extern
PyObject*
Py_kCellTimingNochangeLowLow;


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
PyCellLutTemplate_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCellLutTemplate_Type;
}

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


END_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////

BEGIN_EXTERN_C

void
cell_init();

END_EXTERN_C


#endif // YM_CELL_PYCELL_H
