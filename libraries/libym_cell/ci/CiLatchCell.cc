
/// @file CiLatchCell.cc
/// @brief CiLatchCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiLatchCell.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiLatchCell
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
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristated_array トライステート条件の論理式の配列
// @param[in] data_in "data_in" 関数の式
// @param[in] enable "enable" 関数の式
// @param[in] enable_also "enable_also" 関数の式
// @param[in] alloc メモリアロケータ
// *1: - false 論理式なし
//     - true 論理式あり
CiLatchCell::CiLatchCell(ymuint id,
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
			 const LogExpr& data_in,
			 const LogExpr& enable,
			 const LogExpr& enable_also,
			 AllocBase& alloc) :
  CiCell(id, name, area,
	 ni, no, nio, nb, nc,
	 output_array,
	 logic_array,
	 tristate_array,
	 alloc),
  mDataIn(data_in),
  mEnable(enable),
  mEnable2(enable_also)
{
}

// @brief デストラクタ
CiLatchCell::~CiLatchCell()
{
}

// @brief ラッチセルの時に true を返す．
bool
CiLatchCell::is_latch() const
{
  return true;
}

// @brief ラッチセルの場合にデータ入力関数を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiLatchCell::data_in_expr() const
{
  return mDataIn;
}

// @brief ラッチセルの場合にイネーブル条件を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiLatchCell::enable_expr() const
{
  return mEnable;
}

// @brief ラッチセルの場合に2つめのイネーブル条件を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiLatchCell::enable2_expr() const
{
  return mEnable2;
}


//////////////////////////////////////////////////////////////////////
// クラス CiLatchRCell
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
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristated_array トライステート条件の論理式の配列
// @param[in] data_in "data_in" 関数の式
// @param[in] enable "enable" 関数の式
// @param[in] enable_also "enable_also" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] alloc メモリアロケータ
// *1: - false 論理式なし
//     - true 論理式あり
CiLatchRCell::CiLatchRCell(ymuint id,
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
			   const LogExpr& data_in,
			   const LogExpr& enable,
			   const LogExpr& enable_also,
			   const LogExpr& clear,
			   AllocBase& alloc) :
  CiLatchCell(id, name, area,
	      ni, no, nio, nb, nc,
	      output_array,
	      logic_array,
	      tristate_array,
	      data_in,
	      enable,
	      enable_also,
	      alloc),
  mClear(clear)
{
}

// @brief デストラクタ
CiLatchRCell::~CiLatchRCell()
{
}

// @brief ラッチセルの場合にクリア端子を持っていたら true を返す．
bool
CiLatchRCell::has_clear() const
{
  return true;
}

// @brief ラッチセルの場合にクリア条件を表す論理式を返す．
// @note クリア端子がない場合の返り値は不定
LogExpr
CiLatchRCell::clear_expr() const
{
  return mClear;
}


//////////////////////////////////////////////////////////////////////
// クラス CiLatchSCell
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
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristated_array トライステート条件の論理式の配列
// @param[in] data_in "data_in" 関数の式
// @param[in] enable "enable" 関数の式
// @param[in] enable_also "enable_also" 関数の式
// @param[in] preset "preset" 関数の式
// @param[in] alloc メモリアロケータ
// *1: - false 論理式なし
//     - true 論理式あり
CiLatchSCell::CiLatchSCell(ymuint id,
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
			   const LogExpr& data_in,
			   const LogExpr& enable,
			   const LogExpr& enable_also,
			   const LogExpr& preset,
			   AllocBase& alloc) :
  CiLatchCell(id, name, area,
	      ni, no, nio, nb, nc,
	      output_array,
	      logic_array,
	      tristate_array,
	      data_in,
	      enable,
	      enable_also,
	      alloc),
  mPreset(preset)
{
}

// @brief デストラクタ
CiLatchSCell::~CiLatchSCell()
{
}

// @brief ラッチセルの場合にプリセット端子を持っていたら true を返す．
bool
CiLatchSCell::has_preset() const
{
  return true;
}

// @brief ラッチセルの場合にプリセット条件を表す論理式を返す．
// @note プリセット端子がない場合の返り値は不定
LogExpr
CiLatchSCell::preset_expr() const
{
  return mPreset;
}


//////////////////////////////////////////////////////////////////////
// クラス CiLatchSRCell
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
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristated_array トライステート条件の論理式の配列
// @param[in] data_in "data_in" 関数の式
// @param[in] enable "enable" 関数の式
// @param[in] enable_also "enable_also" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
// @param[in] clear_preset_var1 clear と preset が同時にオンになったときの値1
// @param[in] clear_preset_var2 clear と preset が同時にオンになったときの値2
// @param[in] alloc メモリアロケータ
// *1: - false 論理式なし
//     - true 論理式あり
CiLatchSRCell::CiLatchSRCell(ymuint id,
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
			     const LogExpr& data_in,
			     const LogExpr& enable,
			     const LogExpr& enable_also,
			     const LogExpr& clear,
			     const LogExpr& preset,
			     ymuint clear_preset_var1,
			     ymuint clear_preset_var2,
			     AllocBase& alloc) :
  CiLatchRCell(id, name, area,
	       ni, no, nio, nb, nc,
	       output_array,
	       logic_array,
	       tristate_array,
	       data_in,
	       enable,
	       enable_also,
	       clear,
	       alloc),
  mPreset(preset)
{
  mClearPresetVal[0] = clear_preset_var1;
  mClearPresetVal[1] = clear_preset_var2;
}

// @brief デストラクタ
CiLatchSRCell::~CiLatchSRCell()
{
}

// @brief ラッチセルの場合にプリセット端子を持っていたら true を返す．
bool
CiLatchSRCell::has_preset() const
{
  return true;
}

// @brief ラッチセルの場合にプリセット条件を表す論理式を返す．
// @note プリセット端子がない場合の返り値は不定
LogExpr
CiLatchSRCell::preset_expr() const
{
  return mPreset;
}

// @brief clear_preset_var1 の取得
// @retval 0 "L"
// @retval 1 "H"
// @note FFセルとラッチセルの時に意味を持つ．
ymuint
CiLatchSRCell::clear_preset_var1() const
{
  return mClearPresetVal[0];
}

// @brief clear_preset_var2 の取得
// @retval 0 "L"
// @retval 1 "H"
// @note FFセルとラッチセルの時に意味を持つ．
ymuint
CiLatchSRCell::clear_preset_var2() const
{
  return mClearPresetVal[1];
}

END_NAMESPACE_YM_CELL
