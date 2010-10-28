
/// @file libym_cell/mislib/MislibLibrary.cc
/// @brief MislibLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MislibLibrary.h"
#include "MislibCell.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス MislibLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibLibrary::MislibLibrary() :
  mCellNum(0U),
  mCellArray(NULL)
{
}

// @brief デストラクタ
MislibLibrary::~MislibLibrary()
{
}

// @brief 名前の取得
string
MislibLibrary::name() const
{
}

// @brief テクノロジの取得
tCellTechnology
MislibLibrary::technology() const
{
}

// @brief 遅延モデルの取得
tCellDelayModel
MislibLibrary::delay_model() const
{
}

// @brief バス命名規則の取得
string
MislibLibrary::bus_naming_style() const
{
}

// @brief 日付情報の取得
string
MislibLibrary::date() const
{
}

// @brief リビジョン情報の取得
string
MislibLibrary::revision() const
{
}

// @brief コメント情報の取得
string
MislibLibrary::comment() const
{
}

// @brief 時間単位の取得
tCellTimeUnit
MislibLibrary::time_unit() const
{
}

// @brief 電圧単位の取得
tCellVoltageUnit
MislibLibrary::voltage_unit() const
{
}

// @brief 電流単位の取得
tCellCurrentUnit
MislibLibrary::current_unit() const
{
}

// @brief 抵抗単位の取得
tCellResistanceUnit
MislibLibrary::pulling_resistance_unit() const
{
}

// @brief 容量単位の取得
tCellCapacitanceUnit
MislibLibrary::capacitive_load_unit() const
{
}

// @brief 電力単位の取得
tCellPowerUnit
MislibLibrary::leakage_power_unit() const
{
}

// @brief ルックアップテーブルのテンプレートの取得
// @param[in] name テンプレート名
// @note なければ NULL を返す．
const CellLutTemplate*
MislibLibrary::lu_table_template(const char* name) const
{
  return NULL;
}

// @brief バスタイプの取得
// @param[in] name バスタイプ名
// @note なければ NULL を返す．
const CellBusType*
MislibLibrary::bus_type(const char* name) const
{
  return NULL;
}

// @brief このライブラリの持つセル数の取得
ymuint
MislibLibrary::cell_numm() const
{
  return mCellNum;
}

// @brief セルの取得
// @param[in] pos 位置番号( 0 <= pos < cell_num() )
const Cell*
MislibLibrary::cell(ymuint pos) const
{
  return &mCellArray[pos];
}

// @brief 名前からのセルの取得
const Cell*
MislibLibrary::cell(const char* name) const
{
}

END_NAMESPACE_YM_CELL

