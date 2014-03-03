
/// @file CellPin.sip
/// @brief CellPin の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellPin /Abstract/
{
%TypeHeaderCode
#include "cell/CellPin.h"
using nsYm::CellPin;
%End

public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン番号を返す．
  unsigned int
  pin_id() const;

  /// @brief ピン名を返す．
  const char*
  name() const;
  %MethodCode
  sipRes = sipCpp->name().c_str();
  %End

  /// @brief 方向を返す．
  //tCellPinDirection
  //direction() const;

  /// @brief 入力ピンの時に true を返す．
  /// @note direction() == kInput と等価
  bool
  is_input() const;

  /// @brief 出力ピンの時に true を返す．
  /// @note direction() == kOutput と等価
  bool
  is_output() const;

  /// @brief 入出力ピンの時に true を返す．
  /// @note direction() == kInout と等価
  bool
  is_inout() const;

  /// @brief 内部ピンの時に true を返す．
  /// @note direction() == kInternal と等価
  bool
  is_internal() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン番号を返す．
  /// @note 入力ピンもしくは入出力ピンの時のみ意味を持つ．
  unsigned int
  input_id() const;

  /// @brief 負荷容量を返す．
  CellCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  CellCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  CellCapacitance
  fall_capacitance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピン番号を返す．
  /// @note 出力ピンもしくは入出力ピンの時のみ意味を持つ．
  unsigned int
  output_id() const;

  /// @brief 論理式を持っているときに true を返す．
  bool
  has_function() const;

  /// @brief 機能を表す論理式を返す．
  Expr
  function() const;

  /// @brief three_state 属性を持っているときに true を返す．
  bool
  has_three_state() const;

  /// @brief three_state 論理式を返す．
  Expr
  three_state() const;

  /// @brief 最大ファンアウト容量を返す．
  CellCapacitance
  max_fanout() const;

  /// @brief 最小ファンアウト容量を返す．
  CellCapacitance
  min_fanout() const;

  /// @brief 最大負荷容量を返す．
  CellCapacitance
  max_capacitance() const;

  /// @brief 最小負荷容量を返す．
  CellCapacitance
  min_capacitance() const;

  /// @brief 最大遷移時間を返す．
  CellTime
  max_transition() const;

  /// @brief 最小遷移時間を返す．
  CellTime
  min_transition() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部ピン番号を返す．
  /// @note 内部ピンの時のみ意味を持つ．
  unsigned int
  internal_id() const;


private:

  ~CellPin();

};