
/// @file Cell.sip
/// @brief Cell の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class Cell /Abstract/
{
%TypeHeaderCode
#include "cell/Cell.h"
using nsYm::Cell;
%End

public:

  /// @brief ID番号の取得
  /// @note ここで返される番号は CellLibrary::cell() の引数に対応する．
  unsigned int
  id() const;

  /// @brief 名前の取得
  const char*
  name() const;
  %MethodCode
  sipRes = sipCpp->name().c_str();
  %End

  /// @brief 面積の取得
  CellArea
  area() const;

  /// @brief ピン数の取得
  unsigned int
  pin_num() const;

  /// @brief ピンの取得
  /// @param[in] id ピン番号 ( 0 <= id < pin_num() )
  const CellPin*
  pin(unsigned int id) const;

  /// @brief 名前からピンの取得
  /// @param[in] name ピン名
  /// @return name という名前をピンを返す．
  /// @note なければ NULL を返す．
  const CellPin*
  pin(const char* name) const;

  /// @brief 入力ピン数の取得
  unsigned int
  input_num() const;

  /// @brief 出力ピン数の取得
  unsigned int
  output_num() const;

  /// @brief 入出力ピン数の取得
  unsigned int
  inout_num() const;

  /// @brief 内部ピン数の取得
  unsigned int
  internal_num() const;

  /// @brief 入力ピン+入出力ピン数の取得
  /// @note input_num() + inout_num() に等しい．
  unsigned int
  input_num2() const;

  /// @brief 入力ピンの取得
  /// @param[in] input_id 入力番号 ( 0 <= pos < input_num2() )
  /// @note pos >= input_num() の場合には入出力ピンが返される．
  const CellPin*
  input(unsigned int input_id) const;

  /// @brief 出力ピン+入出力ピン数の取得
  /// @note output_num() + inout_num() に等しい．
  unsigned int
  output_num2() const;

  /// @brief 出力ピンの取得
  /// @param[in] output_id 出力番号 ( 0 <= pos < output_num2() )
  /// @note pos >= output_num() の場合には入出力ピンが返される．
  const CellPin*
  output(unsigned int output_id) const;

  /// @brief 内部ピンの取得
  /// @param[in] internal_id 内部ピン番号 ( 0 <= internal_id < internal_num() )
  const CellPin*
  internal(unsigned int internal_id) const;

  /// @brief バス数の取得
  unsigned int
  bus_num() const;

  /// @brief バスの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < bus_num() )
  const CellBus*
  bus(unsigned int pos) const;

  /// @brief 名前からバスの取得
  /// @param[in] name バス名
  /// @return name という名前のバスを返す．
  /// @note なければ NULL を返す．
  const CellBus*
  bus(const char* name) const;

  /// @brief バンドル数の取得
  unsigned int
  bundle_num() const;

  /// @brief バンドルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < bundle_num() )
  const CellBundle*
  bundle(unsigned int pos) const;

  /// @brief 名前からバンドルの取得
  const CellBundle*
  bundle(const char* name) const;


public:
  //////////////////////////////////////////////////////////////////////
  // タイミング情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief タイミング情報の数の取得
  unsigned int
  timing_num() const;

  /// @brief タイミング情報の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < timing_num() )
  const CellTiming*
  timing(unsigned int pos) const;

  /// @brief 条件に合致するタイミング情報の数の取得
  /// @param[in] ipos 開始ピン番号 ( 0 <= ipos < input_num2() )
  /// @param[in] opos 終了ピン番号 ( 0 <= opos < output_num2() )
  /// @param[in] sense タイミング情報の摘要条件
  unsigned int
  timing_num(unsigned int ipos,
	     unsigned int opos,
	     tCellTimingSense sense) const;

  /// @brief 条件に合致するタイミング情報の取得
  /// @param[in] ipos 開始ピン番号 ( 0 <= ipos < input_num2() )
  /// @param[in] opos 終了ピン番号 ( 0 <= opos < output_num2() )
  /// @param[in] sense タイミング情報の摘要条件
  /// @param[in] pos 位置番号 ( 0 <= pos < timing_num(ipos, opos, sense) )
  /// @return 条件に合致するタイミング情報を返す．
  const CellTiming*
  timing(unsigned int ipos,
	 unsigned int opos,
	 tCellTimingSense sense,
	 unsigned int pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 属している CellGroup を返す．
  const CellGroup*
  cell_group() const;

  /// @brief 組み合わせ論理セルの時に true を返す．
  bool
  is_logic() const;

  /// @brief FFセルの時に true を返す．
  bool
  is_ff() const;

  /// @brief ラッチセルの時に true を返す．
  bool
  is_latch() const;

  /// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
  bool
  is_fsm() const;

  /// @brief 出力の論理式を持っている時に true を返す．
  /// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
  bool
  has_logic(unsigned int pin_id) const;

  /// @brief 全ての出力が論理式を持っているときに true を返す．
  bool
  has_logic() const;

  /// @brief 論理セルの場合に出力の論理式を返す．
  /// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
  /// @note 論理式中の変数番号は入力ピン番号に対応する．
  Expr
  logic_expr(unsigned int pin_id) const;

  /// @brief 出力がトライステート条件を持っている時に true を返す．
  /// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
  bool
  has_tristate(unsigned int pin_id) const;

  /// @brief トライステートセルの場合にトライステート条件式を返す．
  /// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
  /// @note 論理式中の変数番号は入力ピン番号に対応する．
  /// @note 通常の論理セルの場合には定数0を返す．
  Expr
  tristate_expr(unsigned int pin_id) const;

  /// @brief FFセルの場合に次状態関数を表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  Expr
  next_state_expr() const;

  /// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  Expr
  clock_expr() const;

  /// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  Expr
  clock2_expr() const;

  /// @brief ラッチセルの場合にデータ入力関数を表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  Expr
  data_in_expr() const;

  /// @brief ラッチセルの場合にイネーブル条件を表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  Expr
  enable_expr() const;

  /// @brief ラッチセルの場合に2つめのイネーブル条件を表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  Expr
  enable2_expr() const;

  /// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
  bool
  has_clear() const;

  /// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
  /// @note クリア端子がない場合の返り値は不定
  Expr
  clear_expr() const;

  /// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
  bool
  has_preset() const;

  /// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
  /// @note プリセット端子がない場合の返り値は不定
  Expr
  preset_expr() const;

  /// @brief clear_preset_var1 の取得
  /// @retval 0 "L"
  /// @retval 1 "H"
  /// @note FFセルとラッチセルの時に意味を持つ．
  unsigned int
  clear_preset_var1() const;

  /// @brief clear_preset_var2 の取得
  /// @retval 0 "L"
  /// @retval 1 "H"
  /// @note FFセルとラッチセルの時に意味を持つ．
  unsigned int
  clear_preset_var2() const;


private:

  ~Cell();

};
