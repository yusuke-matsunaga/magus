#ifndef CELL_CELLPIN_H
#define CELL_CELLPIN_H

/// @file YmCell/CellPin.h
/// @brief CellPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/cell_nsdef.h"
#include "YmLogic/expr_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellPin CellPin.h "YmCell/CellPin.h"
/// @brief セルのピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CellPin
{
protected:

  /// @brief コンストラクタ
  CellPin() { }

  /// @brief デストラクタ
  virtual
  ~CellPin() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン番号を返す．
  virtual
  ymuint
  pin_id() const = 0;

  /// @brief ピン名を返す．
  virtual
  string
  name() const = 0;

  /// @brief 方向を返す．
  virtual
  tCellPinDirection
  direction() const = 0;

  /// @brief 入力ピンの時に true を返す．
  /// @note direction() == kInput と等価
  virtual
  bool
  is_input() const = 0;

  /// @brief 出力ピンの時に true を返す．
  /// @note direction() == kOutput と等価
  virtual
  bool
  is_output() const = 0;

  /// @brief 入出力ピンの時に true を返す．
  /// @note direction() == kInout と等価
  virtual
  bool
  is_inout() const = 0;

  /// @brief 内部ピンの時に true を返す．
  /// @note direction() == kInternal と等価
  virtual
  bool
  is_internal() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン番号を返す．
  /// @note 入力ピンもしくは入出力ピンの時のみ意味を持つ．
  virtual
  ymuint
  input_id() const = 0;

  /// @brief 負荷容量を返す．
  virtual
  CellCapacitance
  capacitance() const = 0;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  CellCapacitance
  rise_capacitance() const = 0;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  CellCapacitance
  fall_capacitance() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピン番号を返す．
  /// @note 出力ピンもしくは入出力ピンの時のみ意味を持つ．
  virtual
  ymuint
  output_id() const = 0;

  /// @brief 論理式を持っているときに true を返す．
  virtual
  bool
  has_function() const = 0;

  /// @brief 機能を表す論理式を返す．
  virtual
  Expr
  function() const = 0;

  /// @brief three_state 属性を持っているときに true を返す．
  virtual
  bool
  has_three_state() const = 0;

  /// @brief three_state 論理式を返す．
  virtual
  Expr
  three_state() const = 0;

  /// @brief 最大ファンアウト容量を返す．
  virtual
  CellCapacitance
  max_fanout() const = 0;

  /// @brief 最小ファンアウト容量を返す．
  virtual
  CellCapacitance
  min_fanout() const = 0;

  /// @brief 最大負荷容量を返す．
  virtual
  CellCapacitance
  max_capacitance() const = 0;

  /// @brief 最小負荷容量を返す．
  virtual
  CellCapacitance
  min_capacitance() const = 0;

  /// @brief 最大遷移時間を返す．
  virtual
  CellTime
  max_transition() const = 0;

  /// @brief 最小遷移時間を返す．
  virtual
  CellTime
  min_transition() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部ピン番号を返す．
  /// @note 内部ピンの時のみ意味を持つ．
  virtual
  ymuint
  internal_id() const = 0;


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

#endif // CELL_CELLPIN_H
