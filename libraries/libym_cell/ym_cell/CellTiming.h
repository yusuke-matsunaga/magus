#ifndef YM_CELL_CELLTIMING_H
#define YM_CELL_CELLTIMING_H

/// @file　ym_cell/CellTiming.h
/// @brief CellTiming のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_cell/CellTime.h"
#include "ym_cell/CellResistance.h"
#include "ym_logic/LogExpr.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellTiming CellTiming.h "ym_cell/CellTiming.h"
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

  /// @brief ID番号を返す．
  virtual
  ymuint
  id() const = 0;

  /// @brief 型の取得
  virtual
  tCellTimingType
  type() const = 0;

  /// @brief タイミングセンスの取得
  virtual
  tCellTimingSense
  timing_sense() const = 0;

  /// @brief タイミング条件式の取得
  /// @note ない場合には定数1の式が返される．
  virtual
  LogExpr
  timing_cond() const = 0;

  /// @brief 次の要素の取得
  virtual
  const CellTiming*
  next() const = 0;


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
