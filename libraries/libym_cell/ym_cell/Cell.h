#ifndef YM_CELL_CELL_H
#define YM_CELL_CELL_H

/// @file　ym_cell/Cell.h
/// @brief Cell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Cell.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"
#include "ym_logic/LogExpr.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class Cell Cell.h <ym_cell/Cell.h>
/// @brief セル本体のクラス
//////////////////////////////////////////////////////////////////////
class Cell
{
public:
  //////////////////////////////////////////////////////////////////////
  // 列挙型の定義
  //////////////////////////////////////////////////////////////////////


  /// @brief セルの種類を表す型
  enum tType {
    /// @brief 組み合わせ論理
    kLogic,
    /// @brief FF
    kFF,
    /// @brief ラッチ
    kLatch,
    /// @brief FSM (statetable 記述)
    kFSM
  };

public:

  /// @brief コンストラクタ．
  Cell() { }

  /// @brief デストラクタ
  virtual
  ~Cell() { }


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号の取得
  /// @note ここで返される番号は CellLibrary::cell() の引数に対応する．
  virtual
  ymuint
  id() const = 0;

  /// @brief 名前の取得
  virtual
  string
  name() const = 0;

  /// @brief 面積の取得
  virtual
  CellArea
  area() const = 0;

  /// @brief 型の取得
  virtual
  tType
  type() const = 0;

  /// @brief 組み合わせ論理セルの時に true を返す．
  /// @note type() == kLogic と等価
  virtual
  bool
  is_logic() const = 0;

  /// @brief FFセルの時に true を返す．
  /// @note type() == kFF と等価
  virtual
  bool
  is_ff() const = 0;

  /// @brief ラッチセルの時に true を返す．
  /// @note type() == kLatch と等価
  virtual
  bool
  is_latch() const = 0;

  /// @brief FSMセルの時に true を返す．
  /// @note type() == kFSM と等価
  virtual
  bool
  is_fsm() const = 0;

  /// @brief 状態変数1の名前を返す．
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  string
  var1_name() const = 0;

  /// @brief 状態変数2の名前を返す．
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  string
  var2_name() const = 0;

  /// @brief next_state 関数の取得
  /// @note FFセルの時に意味を持つ．
  virtual
  LogExpr
  next_state() const = 0;

  /// @brief clocked_on 関数の取得
  /// @note FFセルの時に意味を持つ．
  virtual
  LogExpr
  clocked_on() const = 0;

  /// @brief clocked_on_also 関数の取得
  /// @note FFセルの時に意味を持つ．
  virtual
  LogExpr
  clocked_on_also() const = 0;

  /// @brief data_in 関数の取得
  virtual
  LogExpr
  data_in() const = 0;

  /// @brief enable 関数の取得
  virtual
  LogExpr
  enable() const = 0;

  /// @brief enable_also 関数の取得
  virtual
  LogExpr
  enable_also() const = 0;

  /// @brief clear 関数の取得
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  LogExpr
  clear() const = 0;

  /// @brief preset 関数の取得
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  LogExpr
  preset() const = 0;

  /// @brief clear_preset_var1 の取得
  /// @retval 0 "L"
  /// @retval 1 "H"
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  ymuint
  clear_preset_var1() const = 0;

  /// @brief clear_preset_var2 の取得
  /// @retval 0 "L"
  /// @retval 1 "H"
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  ymuint
  clear_preset_var2() const = 0;

  /// @brief ピン数の取得
  virtual
  ymuint
  pin_num() const = 0;

  /// @brief ピンの取得
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < pin_num()
  virtual
  const CellPin*
  pin(ymuint pin_id) const = 0;

  /// @brief 名前からピンの取得
  /// @param[in] name ピン名
  /// @return name という名前をピンを返す．
  /// @note なければ NULL を返す．
  virtual
  const CellPin*
  pin(const string& name) const = 0;

  /// @brief バス数の取得
  virtual
  ymuint
  bus_num() const = 0;

  /// @brief バスの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < bus_num() )
  virtual
  const CellBus*
  bus(ymuint pos) const = 0;

  /// @brief 名前からバスの取得
  /// @param[in] name バス名
  /// @return name という名前のバスを返す．
  /// @note なければ NULL を返す．
  virtual
  const CellBus*
  bus(const string& name) const = 0;

  /// @brief バンドル数の取得
  virtual
  ymuint
  bundle_num() const = 0;

  /// @brief バンドルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < bundle_num() )
  virtual
  const CellBundle*
  bundle(ymuint pos) const = 0;

  /// @brief 名前からバンドルの取得
  virtual
  const CellBundle*
  bundle(const string& name) const = 0;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELL_H
