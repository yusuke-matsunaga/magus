
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
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] alloc メモリアロケータ
// @param[in] logic_array 出力の論理式の配列
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
CiFFCell::CiFFCell(ymuint id,
		   const ShString& name,
		   CellArea area,
		   ymuint ni,
		   ymuint no,
		   ymuint nio,
		   ymuint nb,
		   ymuint nc,
		   AllocBase& alloc,
		   const vector<LogExpr>& logic_array,
		   const LogExpr& next_state,
		   const LogExpr& clocked_on) :
  CiCell(id, name, area, ni, no, nio, nb, nc, alloc, logic_array),
  mNextState(next_state),
  mClock(clocked_on)
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


//////////////////////////////////////////////////////////////////////
// クラス CiFFRCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] alloc メモリアロケータ
// @param[in] logic_array 出力の論理式の配列
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] clear "clear" 関数の式
CiFFRCell::CiFFRCell(ymuint id,
		     const ShString& name,
		     CellArea area,
		     ymuint ni,
		     ymuint no,
		     ymuint nio,
		     ymuint nb,
		     ymuint nc,
		     AllocBase& alloc,
		     const vector<LogExpr>& logic_array,
		     const LogExpr& next_state,
		     const LogExpr& clocked_on,
		     const LogExpr& clear) :
  CiFFCell(id, name, area, ni, no, nio, nb, nc, alloc, logic_array,
	   next_state, clocked_on),
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
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] alloc メモリアロケータ
// @param[in] logic_array 出力の論理式の配列
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] preset "preset" 関数の式
CiFFSCell::CiFFSCell(ymuint id,
		     const ShString& name,
		     CellArea area,
		     ymuint ni,
		     ymuint no,
		     ymuint nio,
		     ymuint nb,
		     ymuint nc,
		     AllocBase& alloc,
		     const vector<LogExpr>& logic_array,
		     const LogExpr& next_state,
		     const LogExpr& clocked_on,
		     const LogExpr& preset) :
  CiFFCell(id, name, area, ni, no, nio, nb, nc, alloc, logic_array,
	   next_state, clocked_on),
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
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] alloc メモリアロケータ
// @param[in] logic_array 出力の論理式の配列
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
CiFFSRCell::CiFFSRCell(ymuint id,
		       const ShString& name,
		       CellArea area,
		       ymuint ni,
		       ymuint no,
		       ymuint nio,
		       ymuint nb,
		       ymuint nc,
		       AllocBase& alloc,
		       const vector<LogExpr>& logic_array,
		       const LogExpr& next_state,
		       const LogExpr& clocked_on,
		       const LogExpr& clear,
		       const LogExpr& preset) :
  CiFFRCell(id, name, area, ni, no, nio, nb, nc, alloc, logic_array,
	    next_state, clocked_on, clear),
  mPreset(preset)
{
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

END_NAMESPACE_YM_CELL
