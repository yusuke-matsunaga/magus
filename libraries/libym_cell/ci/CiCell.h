#ifndef LIBYM_CELL_CI_CICELL_H
#define LIBYM_CELL_CI_CICELL_H

/// @file libym_cell/ci/CiCelll.h
/// @brief CiCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/Cell.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

class CiPin;
class CiInputPin;
class CiOutputPin;
class CiInoutPin;
class CiBus;
class CiBundle;

//////////////////////////////////////////////////////////////////////
/// @class CiCell CiCell.h "ci/CiCell.h"
/// @brief Cell の実装クラス
//////////////////////////////////////////////////////////////////////
class CiCell :
  public Cell
{
  friend class CiLibrary;

protected:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  CiCell(ymuint id,
	 const ShString& name,
	 CellArea area);

  /// @brief デストラクタ
  virtual
  ~CiCell();


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

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

  /// @brief ピン数の取得
  virtual
  ymuint
  pin_num() const;

  /// @brief ピンの取得
  /// @param[in] pin_id ピン番号 ( 0 <= pin_id < pin_num()
  virtual
  const CellPin*
  pin(ymuint pin_id) const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 名前
  ShString mName;

  // 面積
  CellArea mArea;

  // 総ピン数
  ymuint32 mPinNum;

  // ピンの配列
  CiPin** mPinArray;

  // バス数
  ymuint32 mBusNum;

  // バスピンの配列
  CiBus* mBusArray;

  // バンドル数
  ymuint32 mBundleNum;

  // バンドルピンの配列
  CiBundle* mBundleArray;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLogicCell CiCell.h "CiCell.h"
/// @brief 組合せ論理セルを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLogicCell :
  public CiCell
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  CiLogicCell(ymuint id,
	      const ShString& name,
	      CellArea area);

  /// @brief デストラクタ
  virtual
  ~CiLogicCell();


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tType
  type() const;

  /// @brief 組み合わせ論理セルの時に true を返す．
  /// @note type() == kLogic と等価
  virtual
  bool
  is_logic() const;

};


//////////////////////////////////////////////////////////////////////
/// @class CiFLCell CiCell.h "CiCell.h"
/// @brief FFセルとラッチセルの基底クラス
//////////////////////////////////////////////////////////////////////
class CiFLCell :
  public CiCell
{
protected:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] var1, var2 状態変数名
  /// @param[in] clear "clear" 関数の式
  /// @param[in] preset "preset" 関数の式
  /// @param[in] clear_preset_var1 "clear_preset_var1" の値
  /// @param[in] clear_preset_var2 "clear_preset_var2" の値
  CiFLCell(ymuint id,
	   const ShString& name,
	   CellArea area,
	   const ShString& var1,
	   const ShString& var2,
	   const LogExpr& clear,
	   const LogExpr& preset,
	   ymuint clear_preset_var1,
	   ymuint clear_preset_var2);

  /// @brief デストラクタ
  virtual
  ~CiFLCell();


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 状態変数1
  ShString mVar1;

  // 状態変数2
  ShString mVar2;

  // clear 関数を表す式
  LogExpr mClear;

  // preset 関数を表す式
  LogExpr mPreset;

  // clear_preset_var1/celar_preset_var2 の値
  ymuint32 mClearPresetVal;

};


//////////////////////////////////////////////////////////////////////
/// @class CiFFCell CiCell.h "CiCell.h"
/// @brief FFセルを表すクラス
//////////////////////////////////////////////////////////////////////
class CiFFCell :
  public CiFLCell
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] var1, var2 状態変数名
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  /// @param[in] clocked_on_also "clocked_on_also" 関数の式
  /// @param[in] clear "clear" 関数の式
  /// @param[in] preset "preset" 関数の式
  /// @param[in] clear_preset_var1 "clear_preset_var1" の値
  /// @param[in] clear_preset_var2 "clear_preset_var2" の値
  CiFFCell(ymuint id,
	   const ShString& name,
	   CellArea area,
	   const ShString& var1,
	   const ShString& var2,
	   const LogExpr& next_state,
	   const LogExpr& clocked_on,
	   const LogExpr& clocked_on_also,
	   const LogExpr& clear,
	   const LogExpr& preset,
	   ymuint clear_preset_var1,
	   ymuint clear_preset_var2);

  /// @brief デストラクタ
  virtual
  ~CiFFCell();


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tType
  type() const;

  /// @brief FFセルの時に true を返す．
  /// @note type() == kFF と等価
  virtual
  bool
  is_ff() const;

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

  /// @brief clocked_on_also 関数の取得
  /// @note FFセルの時に意味を持つ．
  virtual
  LogExpr
  clocked_on_also() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // next_state 関数を表す式
  LogExpr mNextState;

  // clocked_on 関数を表す式
  LogExpr mClockedOn;

  // clocked_on_also 関数を表す式
  LogExpr mClockedOnAlso;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLatchCell CiCell.h "CiCell.h"
/// @brief ラッチセルを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLatchCell :
  public CiFLCell
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] var1, var2 状態変数名
  /// @param[in] data_in "data_in" 関数の式
  /// @param[in] enable "enable" 関数の式
  /// @param[in] enable_also "enable_also" 関数の式
  /// @param[in] clear "clear" 関数の式
  /// @param[in] preset "preset" 関数の式
  /// @param[in] clear_preset_var1 "clear_preset_var1" の値
  /// @param[in] clear_preset_var2 "clear_preset_var2" の値
  CiLatchCell(ymuint id,
	      const ShString& name,
	      CellArea area,
	      const ShString& var1,
	      const ShString& var2,
	      const LogExpr& data_in,
	      const LogExpr& enable,
	      const LogExpr& enable_also,
	      const LogExpr& clear,
	      const LogExpr& preset,
	      ymuint clear_preset_var1,
	      ymuint clear_preset_var2);

  /// @brief デストラクタ
  virtual
  ~CiLatchCell();


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tType
  type() const;

  /// @brief ラッチセルの時に true を返す．
  /// @note type() == kLatch と等価
  virtual
  bool
  is_latch() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // data_in 関数の取得
  LogExpr mDataIn;

  // enable 関数の取得
  LogExpr mEnable;

  // enable_also 関数の取得
  LogExpr mEnableAlso;

};


END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_CI_CICELL_H
