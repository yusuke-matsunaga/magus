
/// @file CiFFCell.cc
/// @brief CiFFCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiFFCell.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiFFCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] library 親のセルライブラリ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristate_array トライステート条件の論理式の配列
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] clocked_on_also "clocked_on_also" 関数の式
// @param[in] alloc メモリアロケータ
// *1: - false 論理式なし
//     - true 論理式あり
CiFFCell::CiFFCell(CiLibrary* library,
		   ymuint id,
		   const ShString& name,
		   CellArea area,
		   ymuint ni,
		   ymuint no,
		   ymuint nio,
		   ymuint nb,
		   ymuint nc,
		   const vector<bool>& output_array,
		   const vector<LogExpr>& logic_array,
		   const vector<LogExpr>& tristate_array,
		   const LogExpr& next_state,
		   const LogExpr& clocked_on,
		   const LogExpr& clocked_on_also,
		   AllocBase& alloc) :
  CiCell(library, id, name, area,
	 ni, no, nio, 0, nb, nc,
	 output_array,
	 logic_array,
	 tristate_array,
	 alloc),
  mNextState(next_state),
  mClock(clocked_on),
  mClock2(clocked_on_also)
{
}

// @brief デストラクタ
CiFFCell::~CiFFCell()
{
}

// @brief FFセルの時に true を返す．
bool
CiFFCell::is_ff() const
{
  return true;
}

// @brief FFセルの場合に次状態関数を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiFFCell::next_state_expr() const
{
  return mNextState;
}

// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiFFCell::clock_expr() const
{
  return mClock;
}

// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiFFCell::clock2_expr() const
{
  return mClock2;
}


//////////////////////////////////////////////////////////////////////
// クラス CiFFRCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] library 親のセルライブラリ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristate_array トライステート条件の論理式の配列
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] clocked_on_also "clocked_on_also" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] alloc メモリアロケータ
// *1: - false 論理式なし
//     - true 論理式あり
CiFFRCell::CiFFRCell(CiLibrary* library,
		     ymuint id,
		     const ShString& name,
		     CellArea area,
		     ymuint ni,
		     ymuint no,
		     ymuint nio,
		     ymuint nb,
		     ymuint nc,
		     const vector<bool>& output_array,
		     const vector<LogExpr>& logic_array,
		     const vector<LogExpr>& tristate_array,
		     const LogExpr& next_state,
		     const LogExpr& clocked_on,
		     const LogExpr& clocked_on_also,
		     const LogExpr& clear,
		     AllocBase& alloc) :
  CiFFCell(library, id, name, area,
	   ni, no, nio, nb, nc,
	   output_array,
	   logic_array,
	   tristate_array,
	   next_state,
	   clocked_on,
	   clocked_on_also,
	   alloc),
  mClear(clear)
{
}

// @brief デストラクタ
CiFFRCell::~CiFFRCell()
{
}

// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
bool
CiFFRCell::has_clear() const
{
  return true;
}

// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
// @note クリア端子がない場合の返り値は不定
LogExpr
CiFFRCell::clear_expr() const
{
  return mClear;
}


//////////////////////////////////////////////////////////////////////
// クラス CiFFSCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] library 親のセルライブラリ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristate_array トライステート条件の論理式の配列
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] clocked_on_also "clocked_on_also" 関数の式
// @param[in] preset "preset" 関数の式
// @param[in] alloc メモリアロケータ
// *1: - false 論理式なし
//     - true 論理式あり
CiFFSCell::CiFFSCell(CiLibrary* library,
		     ymuint id,
		     const ShString& name,
		     CellArea area,
		     ymuint ni,
		     ymuint no,
		     ymuint nio,
		     ymuint nb,
		     ymuint nc,
		     const vector<bool>& output_array,
		     const vector<LogExpr>& logic_array,
		     const vector<LogExpr>& tristate_array,
		     const LogExpr& next_state,
		     const LogExpr& clocked_on,
		     const LogExpr& clocked_on_also,
		     const LogExpr& preset,
		     AllocBase& alloc) :
  CiFFCell(library, id, name, area,
	   ni, no, nio, nb, nc,
	   output_array,
	   logic_array,
	   tristate_array,
	   next_state,
	   clocked_on,
	   clocked_on_also,
	   alloc),
  mPreset(preset)
{
}

// @brief デストラクタ
CiFFSCell::~CiFFSCell()
{
}

// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
bool
CiFFSCell::has_preset() const
{
  return true;
}

// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
// @note プリセット端子がない場合の返り値は不定
LogExpr
CiFFSCell::preset_expr() const
{
  return mPreset;
}


//////////////////////////////////////////////////////////////////////
// クラス CiFFSRCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] library 親のセルライブラリ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristate_array トライステート条件の論理式の配列
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] clocked_on_also "clocked_on_also" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
// @param[in] clear_preset_var1 clear と preset が同時にオンになったときの値1
// @param[in] clear_preset_var2 clear と preset が同時にオンになったときの値1
// @param[in] alloc メモリアロケータ
// *1: - false 論理式なし
//     - true 論理式あり
CiFFSRCell::CiFFSRCell(CiLibrary* library,
		       ymuint id,
		       const ShString& name,
		       CellArea area,
		       ymuint ni,
		       ymuint no,
		       ymuint nio,
		       ymuint nb,
		       ymuint nc,
		       const vector<bool>& output_array,
		       const vector<LogExpr>& logic_array,
		       const vector<LogExpr>& tristate_array,
		       const LogExpr& next_state,
		       const LogExpr& clocked_on,
		       const LogExpr& clocked_on_also,
		       const LogExpr& clear,
		       const LogExpr& preset,
		       ymuint clear_preset_var1,
		       ymuint clear_preset_var2,
		       AllocBase& alloc) :
  CiFFRCell(library, id, name, area,
	    ni, no, nio, nb, nc,
	    output_array,
	    logic_array,
	    tristate_array,
	    next_state,
	    clocked_on,
	    clocked_on_also,
	    clear,
	    alloc),
  mPreset(preset)
{
  mClearPresetVal[0] = clear_preset_var1;
  mClearPresetVal[1] = clear_preset_var2;
}

// @brief デストラクタ
CiFFSRCell::~CiFFSRCell()
{
}

// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
bool
CiFFSRCell::has_preset() const
{
  return true;
}

// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
// @note プリセット端子がない場合の返り値は不定
LogExpr
CiFFSRCell::preset_expr() const
{
  return mPreset;
}

// @brief clear_preset_var1 の取得
// @retval 0 "L"
// @retval 1 "H"
// @note FFセルとラッチセルの時に意味を持つ．
ymuint
CiFFSRCell::clear_preset_var1() const
{
  return mClearPresetVal[0];
}

// @brief clear_preset_var2 の取得
// @retval 0 "L"
// @retval 1 "H"
// @note FFセルとラッチセルの時に意味を持つ．
ymuint
CiFFSRCell::clear_preset_var2() const
{
  return mClearPresetVal[1];
}

END_NAMESPACE_YM_CELL
