#ifndef LIBYM_CELL_MISLIB_MISLIBCELLLIBRARY_H
#define LIBYM_CELL_MISLIB_MISLIBCELLLIBRARY_H

/// @file　libym_cell/mislib/MislibCellLibrary.h
/// @brief MislibCellLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellLibrary.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"


BEGIN_NAMESPACE_YM_CELL

class MislibCell;

//////////////////////////////////////////////////////////////////////
/// @class MislibCellLibrary MislibCellLibrary.h "MislibCellLibrary.h"
/// @brief mislib(genlib)用のセルライブラリを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibCellLibrary :
  public CellLibrary
{
  friend class MislibParserImpl;

public:

  /// @brief コンストラクタ
  MislibCellLibrary();

  /// @brief デストラクタ
  virtual
  ~MislibCellLibrary();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const;

  /// @brief テクノロジの取得
  virtual
  tCellTechnology
  technology() const;

  /// @brief 遅延モデルの取得
  virtual
  tCellDelayModel
  delay_model() const;

  /// @brief バス命名規則の取得
  virtual
  string
  bus_naming_style() const;

  /// @brief 日付情報の取得
  virtual
  string
  date() const;

  /// @brief リビジョン情報の取得
  virtual
  string
  revision() const;

  /// @brief コメント情報の取得
  virtual
  string
  comment() const;

  /// @brief 時間単位の取得
  virtual
  tCellTimeUnit
  time_unit() const;

  /// @brief 電圧単位の取得
  virtual
  tCellVoltageUnit
  voltage_unit() const;

  /// @brief 電流単位の取得
  virtual
  tCellCurrentUnit
  current_unit() const;

  /// @brief 抵抗単位の取得
  virtual
  tCellResistanceUnit
  pulling_resistance_unit() const;

  /// @brief 容量単位の取得
  virtual
  tCellCapacitanceUnit
  capacitive_load_unit() const;

  /// @brief 電力単位の取得
  virtual
  tCellPowerUnit
  leakage_power_unit() const;

  /// @brief ルックアップテーブルのテンプレートの取得
  /// @param[in] name テンプレート名
  /// @note なければ NULL を返す．
  virtual
  const CellLutTemplate*
  lu_table_template(const char* name) const;

  /// @brief バスタイプの取得
  /// @param[in] name バスタイプ名
  /// @note なければ NULL を返す．
  virtual
  const CellBusType*
  bus_type(const char* name) const;


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief このライブラリの持つセル数の取得
  virtual
  ymuint
  n_cells() const;

  /// @brief セルの取得
  /// @param[in] pos 位置番号( 0 <= pos < n_cells() )
  virtual
  const Cell*
  cell(ymuint pos) const;

  /// @brief 名前からのセルの取得
  virtual
  const Cell*
  cell(const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルのリスト


};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_MISLIB_MISLIBCELLLIBRARY_H
