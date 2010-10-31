#ifndef LIBYM_CELL_MISLIB_MISLIBLIBRARY_H
#define LIBYM_CELL_MISLIB_MISLIBLIBRARY_H

/// @file　libym_cell/mislib/MislibLibrary.h
/// @brief MislibLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellLibrary.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"
#include "ym_lexp/lexp_nsdef.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

class MislibCell;

//////////////////////////////////////////////////////////////////////
/// @class MislibLibrary MislibLibrary.h "MislibLibrary.h"
/// @brief mislib(genlib)用のセルライブラリを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibLibrary :
  public CellLibrary
{
  friend class MislibParserImpl;

public:

  /// @brief コンストラクタ
  MislibLibrary();

  /// @brief デストラクタ
  virtual
  ~MislibLibrary();


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
  string
  time_unit() const;

  /// @brief 電圧単位の取得
  virtual
  string
  voltage_unit() const;

  /// @brief 電流単位の取得
  virtual
  string
  current_unit() const;

  /// @brief 抵抗単位の取得
  virtual
  string
  pulling_resistance_unit() const;

  /// @brief 容量単位の取得
  virtual
  string
  capacitive_load_unit() const;

  /// @brief 電力単位の取得
  virtual
  string
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
  cell_num() const;

  /// @brief セルの取得
  /// @param[in] pos 位置番号( 0 <= pos < cell_num() )
  virtual
  const Cell*
  cell(ymuint pos) const;

  /// @brief 名前からのセルの取得
  virtual
  const Cell*
  cell(const char* name) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セル数を設定する．
  /// @param[in] num 設定する値
  void
  set_cell_num(ymuint num);

  /// @brief セルの内容を設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] opin_name 出力ピンの名前
  /// @param[in] opin_expr 出力ピンの論理式
  /// @param[in] ni 入力数
  void
  set_cell(ymuint pos,
	   ShString name,
	   CellArea area,
	   ShString opin_name,
	   const LogExpr& opin_expr,
	   ymuint ni);

  /// @brief セルの入力ピンの内容を設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  /// @param[in] ipin_pos 入力ピン番号
  /// @param[in] ipin_name 入力ピン名
  /// @param[in] ipin_load 入力ピンの負荷容量
  void
  set_cell_ipin(ymuint pos,
		ymuint ipin_pos,
		ShString ipin_name,
		CellCapacitance ipin_load);

  /// @brief セルのタイミング情報を設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  /// @param[in] ipin_pos 入力ピン番号
  /// @param[in] sense タイミング条件
  /// @param[in] rise_intrinsic 立ち上がり固有遅延
  /// @param[in] rise_resistance 立ち上がり負荷依存係数
  /// @param[in] fall_intrinsic 立ち下がり固有遅延
  /// @param[in] fall_resistance 立ち下がり負荷依存係数
  void
  set_cell_timing(ymuint pos,
		  ymuint ipin_pos,
		  tCellTimingSense sense,
		  CellTime rise_intrinsic,
		  CellResistance rise_resistance,
		  CellTime fall_intrinsic,
		  CellResistance fall_resistance);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のオブジェクト
  SimpleAlloc mAlloc;

  // ライブラリの名前
  string mName;

  // セル数
  ymuint32 mCellNum;

  // セルのリスト
  MislibCell* mCellArray;

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_MISLIB_MISLIBLIBRARY_H
