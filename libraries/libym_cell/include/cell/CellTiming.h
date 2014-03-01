#ifndef CELL_CELLTIMING_H
#define CELL_CELLTIMING_H

/// @file cell/CellTiming.h
/// @brief CellTiming のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "cell/cell_nsdef.h"
#include "cell/CellTime.h"
#include "cell/CellResistance.h"
#include "logic/expr_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellTiming CellTiming.h "cell/CellTiming.h"
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

  /// @brief ID番号の取得
  /// @note timing = cell->timing(id); の時，timing->id() = id となる．
  virtual
  ymuint
  id() const = 0;

  /// @brief 型の取得
  virtual
  tCellTimingType
  type() const = 0;

  /// @brief タイミング条件式の取得
  /// @note ない場合には定数1の式が返される．
  virtual
  Expr
  timing_cond() const = 0;


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


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ODO& s) const = 0;

};

END_NAMESPACE_YM_CELL

#endif // CELL_CELLTIMING_H
