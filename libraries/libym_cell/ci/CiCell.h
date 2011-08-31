#ifndef CICELL_H
#define CICELL_H

/// @file CiCelll.h
/// @brief CiCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/Cell.h"
#include "ym_utils/ShString.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL

class CiPin;
class CiInputPin;
class CiOutputPin;
class CiInoutPin;
class CiBus;
class CiBundle;
class CiTiming;

//////////////////////////////////////////////////////////////////////
/// @class CiCell CiCell.h "CiCell.h"
/// @brief Cell の実装クラス
//////////////////////////////////////////////////////////////////////
class CiCell :
  public Cell
{
  friend class CiLibrary;

protected:

  /// @brief コンストラクタ
  CiCell();

  /// @brief デストラクタ
  virtual
  ~CiCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 基本情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 属している CellGroup を返す．
  virtual
  const CellGroup*
  cell_group() const;

  /// @brief ID番号の取得
  /// @note ここで返される番号は CellLibrary::cell() の引数に対応する．
  virtual
  ymuint
  id() const;

  /// @brief 名前の取得
  virtual
  string
  name() const;

  /// @brief 面積の取得
  virtual
  CellArea
  area() const;

  /// @brief 入力ピン数の取得
  virtual
  ymuint
  input_num() const;

  /// @brief 入力ピン+入出力ピン数の取得
  /// @note input_num() + inout_num() に等しい．
  virtual
  ymuint
  input_num2() const;

  /// @brief 入力ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
  /// @note pos >= input_num() の場合には入出力ピンが返される．
  virtual
  const CellPin*
  input(ymuint pos) const;

  /// @brief 出力ピン数の取得
  virtual
  ymuint
  output_num() const;

  /// @brief 出力ピン+入出力ピン数の取得
  /// @note output_num() + inout_num() に等しい．
  virtual
  ymuint
  output_num2() const;

  /// @brief 出力ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
  /// @note pos >= output_num() の場合には入出力ピンが返される．
  virtual
  const CellPin*
  output(ymuint pos) const;

  /// @brief 入出力ピン数の取得
  virtual
  ymuint
  inout_num() const;

  /// @brief 入出力ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < inout_num() )
  virtual
  const CellPin*
  inout(ymuint pos) const;

  /// @brief 名前からピンの取得
  /// @param[in] name ピン名
  /// @return name という名前をピンを返す．
  /// @note なければ NULL を返す．
  virtual
  const CellPin*
  pin(const string& name) const;

  /// @brief バス数の取得
  virtual
  ymuint
  bus_num() const;

  /// @brief バスの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < bus_num() )
  virtual
  const CellBus*
  bus(ymuint pos) const;

  /// @brief 名前からバスの取得
  /// @param[in] name バス名
  /// @return name という名前のバスを返す．
  /// @note なければ NULL を返す．
  virtual
  const CellBus*
  bus(const string& name) const;

  /// @brief バンドル数の取得
  virtual
  ymuint
  bundle_num() const;

  /// @brief バンドルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < bundle_num() )
  virtual
  const CellBundle*
  bundle(ymuint pos) const;

  /// @brief 名前からバンドルの取得
  virtual
  const CellBundle*
  bundle(const string& name) const;


public:
  //////////////////////////////////////////////////////////////////////
  // タイミング情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief タイミング情報の取得
  /// @param[in] ipos 開始ピン番号
  /// @param[in] opos 終了ピン番号
  /// @param[in] timing_sense タイミング情報の摘要条件
  /// @return 条件に合致するタイミング情報を返す．
  /// @note なければ NULL を返す．
  virtual
  const CellTiming*
  timing(ymuint ipos,
	 ymuint opos,
	 tCellTimingSense sense) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

#if 0
  /// @brief 組み合わせ論理セルの時に true を返す．
  /// @note type() == kLogic と等価
  virtual
  bool
  is_logic() const;

  /// @brief FFセルの時に true を返す．
  /// @note type() == kFF と等価
  virtual
  bool
  is_ff() const;

  /// @brief ラッチセルの時に true を返す．
  /// @note type() == kLatch と等価
  virtual
  bool
  is_latch() const;

  /// @brief FSMセルの時に true を返す．
  /// @note type() == kFSM と等価
  virtual
  bool
  is_fsm() const;

  /// @brief 状態変数1の名前を返す．
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  string
  var1_name() const;

  /// @brief 状態変数2の名前を返す．
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  string
  var2_name() const;

  /// @brief next_state 関数の取得
  /// @note FFセルの時に意味を持つ．
  virtual
  LogExpr
  next_state() const;

  /// @brief clocked_on 関数の取得
  /// @note FFセルの時に意味を持つ．
  virtual
  LogExpr
  clocked_on() const;

  /// @brief data_in 関数の取得
  virtual
  LogExpr
  data_in() const;

  /// @brief enable 関数の取得
  virtual
  LogExpr
  enable() const;

  /// @brief enable_also 関数の取得
  virtual
  LogExpr
  enable_also() const;

  /// @brief clocked_on_also 関数の取得
  /// @note FFセルの時に意味を持つ．
  virtual
  LogExpr
  clocked_on_also() const;

  /// @brief clear 関数の取得
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  LogExpr
  clear() const;

  /// @brief preset 関数の取得
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  LogExpr
  preset() const;

  /// @brief clear_preset_var1 の取得
  /// @retval 0 "L"
  /// @retval 1 "H"
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  ymuint
  clear_preset_var1() const;

  /// @brief clear_preset_var2 の取得
  /// @retval 0 "L"
  /// @retval 1 "H"
  /// @note FFセルとラッチセルの時に意味を持つ．
  virtual
  ymuint
  clear_preset_var2() const;
#endif


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] alloc メモリアロケータ
  void
  init(ymuint id,
       const ShString& name,
       CellArea area,
       ymuint ni,
       ymuint no,
       ymuint nio,
       ymuint nb,
       ymuint nc,
       AllocBase& alloc);

#if 0
  /// @brief 出力ピンの機能を設定する．
  /// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
  /// @param[in] function 機能を表す論理式
  /// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->output_num() を使う．
  virtual
  void
  set_logic_function(ymuint opin_id,
		     const LogExpr& function);

  /// @brief 出力ピンの three_state 条件を設定する．
  /// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
  /// @param[in] expr three_state 条件を表す論理式
  /// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->output_num() を使う．
  virtual
  void
  set_tristate_function(ymuint opin_id,
			const LogExpr& expr);
#endif

  /// @brief タイミング情報を設定する．
  /// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
  /// @param[in] ipin_id 関連する入力(入出力)ピン番号 ( *2 )
  /// @param[in] sense タイミング条件
  /// @param[in] timing 設定するタイミング情報
  /// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->output_num() を使う．
  /// @note ( *2 ) ipin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->input_num() を使う．
  void
  set_timing(ymuint opin_id,
	     ymuint ipin_id,
	     tCellTimingSense sense,
	     CiTiming* timing);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルグループ
  CellGroup* mCellGroup;

  // ID番号
  ymuint32 mId;

  // 名前
  ShString mName;

  // 面積
  CellArea mArea;

  // 入力ピン数
  ymuint32 mInputNum;

  // 入力ピンの配列
  CiPin** mInputArray;

  // 出力ピン数
  ymuint32 mOutputNum;

  // 出力ピンの配列
  CiPin** mOutputArray;

  // 入出力ピン数
  ymuint32 mInoutNum;

  // 入出力ピンの配列
  CiPin** mInoutArray;

  // バス数
  ymuint32 mBusNum;

  // バスピンの配列
  CiBus* mBusArray;

  // バンドル数
  ymuint32 mBundleNum;

  // バンドルピンの配列
  CiBundle* mBundleArray;

  // タイミング情報を格納する配列
  // サイズは(入力数＋入出力数) x (出力数+入出力ピン数)  x 2
  CiTiming** mTimingArray;

};

END_NAMESPACE_YM_CELL

#endif // CICELL_H
