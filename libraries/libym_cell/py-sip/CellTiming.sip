
/// @file CellTiming.sip
/// @brief CellTiming の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellTiming /Abstract/
{
%TypeHeaderCode
#include "cell/CellTiming.h"
using nsYm::CellTiming;
%End

public:

  /// @brief ID番号の取得
  /// @note timing = cell->timing(id); の時，timing->id() = id となる．
  unsigned int
  id() const;

  /// @brief 型の取得
  tCellTimingType
  type() const;

  /// @brief タイミング条件式の取得
  /// @note ない場合には定数1の式が返される．
  Expr
  timing_cond() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック/CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり固有遅延の取得
  CellTime
  intrinsic_rise() const;

  /// @brief 立ち下がり固有遅延の取得
  CellTime
  intrinsic_fall() const;

  /// @brief 立ち上がりスロープ遅延の取得
  CellTime
  slope_rise() const;

  /// @brief 立ち下がりスロープ遅延の取得
  CellTime
  slope_fall() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延の取得
  CellResistance
  rise_resistance() const;

  /// @brief 立ち下がり遷移遅延の取得
  CellResistance
  fall_resistance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延の取得
  CellResistance
  rise_pin_resistance() const;

  /// @brief 立ち下がり遷移遅延の取得
  CellResistance
  fall_pin_resistance() const;

  /// @brief 立ち上がり？？？
  CellTime
  rise_delay_intercept() const;

  /// @brief 立ち下がり？？？
  CellTime
  fall_delay_intercept() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延テーブルの取得
  const CellLut*
  rise_transition() const;

  /// @brief 立ち下がり遷移遅延テーブルの取得
  const CellLut*
  fall_transition() const;

  /// @brief 立ち上がり伝搬遅延テーブルの取得
  const CellLut*
  rise_propagation() const;

  /// @brief 立ち下がり伝搬遅延テーブルの取得
  const CellLut*
  fall_propagation() const;

  /// @brief 立ち上がりセル遅延テーブルの取得
  const CellLut*
  cell_rise() const;

  /// @brief 立ち下がりセル遅延テーブルの取得
  const CellLut*
  cell_fall() const;


private:

  ~CellTiming();

};