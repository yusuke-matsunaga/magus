#ifndef PYLIBRARY_H
#define PYLIBRARY_H

/// @file PyLibrary.h
/// @brief PyLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyLibrary PyLibrary.h "PyLibrary.h"
/// @brief CellLibraryObject のための補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyLibrary
{
public:

  /// @brief コンストラクタ
  PyLibrary(const CellLibrary* library);

  /// @brief デストラクタ
  ~PyLibrary();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief CellLibrary を返す．
  const CellLibrary*
  library();

  /// @brief 名前を返す．
  PyObject*
  name();

  /// @brief テクノロジを表す文字列を返す．
  PyObject*
  technology();

  /// @brief 遅延モデルを表す文字列を返す．
  PyObject*
  delay_model();

  /// @brief バス命名規則を表す文字列を返す．
  PyObject*
  bus_naming_style();

  /// @brief 日付を返す．
  PyObject*
  date();

  /// @brief リビジョンを返す．
  PyObject*
  revision();

  /// @brief コメントを返す．
  PyObject*
  comment();

  /// @brief 時間単位を返す．
  PyObject*
  time_unit();

  /// @brief 電圧単位を返す．
  PyObject*
  voltage_unit();

  /// @brief 電流単位を返す．
  PyObject*
  current_unit();

  /// @brief 抵抗単位を返す．
  PyObject*
  pulling_resistance_unit();

  /// @brief 容量単位を返す．
  PyObject*
  capacitive_load_unit();

  /// @brief 電力単位の取得
  PyObject*
  leakage_power_unit();

  /// @brief 遅延テーブルのテンプレートを返す．
  PyObject*
  lu_table_template(ymuint pos);

  /// @brief セルを返す．
  PyObject*
  cell(ymuint pos);

  /// @brief セルグループを返す．
  PyObject*
  cell_group(ymuint pos);

  /// @brief NPN同値クラスを返す．
  PyObject*
  npn_class(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CellLibrary の本体
  const CellLibrary* mLibrary;

  // 名前
  PyObject* mName;

  // テクノロジ
  PyObject* mTechnology;

  // 遅延モデル
  PyObject* mDelayModel;

  // bus_name_style オブジェクト
  PyObject* mBusNamingStyle;

  // date オブジェクト
  PyObject* mDate;

  // revision オブジェクト
  PyObject* mRevision;

  // comment オブジェクト
  PyObject* mComment;

  // time_unit オブジェクト
  PyObject* mTimeUnit;

  // voltage_unit オブジェクト
  PyObject* mVoltageUnit;

  // current_unit オブジェクト
  PyObject* mCurrentUnit;

  // pulling_resistance_unit オブジェクト
  PyObject* mPullingResistanceUnit;

  // capacitive_load_unit オブジェクト
  PyObject* mCapacitiveLoadUnit;

  // leakage_power_unit オブジェクト
  PyObject* mLeakagePowerUnit;

  // セルオブジェクトの配列
  PyObject** mCellList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief CellLibrary を返す．
inline
const CellLibrary*
PyLibrary::library()
{
  return mLibrary;
}

// @brief 名前を返す．
inline
PyObject*
PyLibrary::name()
{
  return mName;
}

// @brief テクノロジを表す文字列を返す．
inline
PyObject*
PyLibrary::technology()
{
  return mTechnology;
}

// @brief 遅延モデルを表す文字列を返す．
inline
PyObject*
PyLibrary::delay_model()
{
  return mDelayModel;
}

// @brief バス命名規則を表す文字列を返す．
inline
PyObject*
PyLibrary::bus_naming_style()
{
  return mBusNamingStyle;
}

// @brief 日付を返す．
inline
PyObject*
PyLibrary::date()
{
  return mDate;
}

// @brief リビジョンを返す．
inline
PyObject*
PyLibrary::revision()
{
  return mRevision;
}

// @brief コメントを返す．
inline
PyObject*
PyLibrary::comment()
{
  return mComment;
}

// @brief 時間単位を返す．
inline
PyObject*
PyLibrary::time_unit()
{
  return mTimeUnit;
}

// @brief 電圧単位を返す．
inline
PyObject*
PyLibrary::voltage_unit()
{
  return mVoltageUnit;
}

// @brief 電流単位を返す．
inline
PyObject*
PyLibrary::current_unit()
{
  return mCurrentUnit;
}

// @brief 抵抗単位を返す．
inline
PyObject*
PyLibrary::pulling_resistance_unit()
{
  return mPullingResistanceUnit;
}

// @brief 容量単位を返す．
inline
PyObject*
PyLibrary::capacitive_load_unit()
{
  return mCapacitiveLoadUnit;
}

// @brief 電力単位の取得
inline
PyObject*
PyLibrary::leakage_power_unit()
{
  return mLeakagePowerUnit;
}

// @brief セルを返す．
inline
PyObject*
PyLibrary::cell(ymuint pos)
{
  return mCellList[pos];
}

END_NAMESPACE_YM

#endif // PYLIBRARY_H
