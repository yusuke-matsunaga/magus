#ifndef YM_CELL_CELLTIMING_H
#define YM_CELL_CELLTIMING_H

/// @file　ym_cell/CellTiming.h
/// @brief CellTiming のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellTiming.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellTiming CellTiming.h <ym_cell/CellTiming.h>
/// @brief タイミング情報を表すクラス
//////////////////////////////////////////////////////////////////////
class CellTiming
{
protected:

  /// @brief コンストラクタ
  CellTiming() { }

  /// @brief デストラクタ
  virtual
  ~CellTiming() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 関連する開始ピン数の取得
  virtual
  ymuint
  ipin_num() const = 0;

  /// @brief 関連する開始ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < ipin_num() )
  virtual
  const CellPin*
  ipin(ymuint pos) const = 0;

  /// @brief 関連する終点ピンの取得
  virtual
  const CellPin*
  opin() const = 0;

  /// @brief 条件の取得
  virtual
  tCellTimingSense
  timing_sense() const = 0;

  /// @brief 型の取得
  virtual
  tCellTimingType
  timing_type() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック/CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり固有遅延の取得
  virtual
  CellTime
  intrinsic_rise() const = 0;

  /// @brief 立ち下がり固有遅延の取得
  virtual
  CellTime
  intrinsic_fall() const = 0;

  /// @brief 立ち上がりスロープ遅延の取得
  virtual
  CellTime
  slope_rise() const = 0;

  /// @brief 立ち下がりスロープ遅延の取得
  virtual
  CellTime
  slope_fall() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延の取得
  virtual
  CellResistance
  rise_resistance() const = 0;

  /// @brief 立ち下がり遷移遅延の取得
  virtual
  CellResistance
  fall_resistance() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延の取得
  virtual
  CellResistance
  rise_pin_resistance() const = 0;

  /// @brief 立ち下がり遷移遅延の取得
  virtual
  CellResistance
  fall_pin_resistance() const = 0;

  /// @brief 立ち上がり？？？
  virtual
  CellTime
  rise_delay_intercept() const = 0;

  /// @brief 立ち下がり？？？
  virtual
  CellTime
  fall_delay_intercept() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延テーブルの取得
  virtual
  const CellLut*
  rise_transition() const = 0;

  /// @brief 立ち下がり遷移遅延テーブルの取得
  virtual
  const CellLut*
  fall_transition() const = 0;

  /// @brief 立ち上がり伝搬遅延テーブルの取得
  virtual
  const CellLut*
  rise_propagation() const = 0;

  /// @brief 立ち下がり伝搬遅延テーブルの取得
  virtual
  const CellLut*
  fall_propagation() const = 0;

  /// @brief 立ち上がりセル遅延テーブルの取得
  virtual
  const CellLut*
  cell_rise() const = 0;

  /// @brief 立ち下がりセル遅延テーブルの取得
  virtual
  const CellLut*
  cell_fall() const = 0;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLTIMING_H
