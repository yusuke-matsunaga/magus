#ifndef CIFFCELL_H
#define CIFFCELL_H

/// @file CiFFCell.h
/// @brief CiFFCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CiCell.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiFFCell CiFFCell.h "CiFFCell.h"
/// @brief FFセルを表す CiCell の派生クラス
//////////////////////////////////////////////////////////////////////
class CiFFCell :
  public CiCell
{
  friend class CiLibrary;

protected:

  /// @brief コンストラクタ
  /// @param[in] library 親のセルライブラリ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] output_array 出力の情報の配列(*1)
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] tristate_array トライステート条件の論理式の配列
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  /// @param[in] clocked_on_also "clocked_on_also" 関数の式
  /// @param[in] alloc メモリアロケータ
  /// *1: - false 論理式なし
  ///     - true 論理式あり
  CiFFCell(CiLibrary* library,
	   ymuint id,
	   const ShString& name,
	   CellArea area,
	   ymuint ni,
	   ymuint no,
	   ymuint nio,
	   ymuint nb,
	   ymuint nc,
	   const vector<bool>& output_array,
	   const vector<Expr>& logic_array,
	   const vector<Expr>& tristate_array,
	   const Expr& next_state,
	   const Expr& clocked_on,
	   const Expr& clocked_on_also,
	   Alloc& alloc);

  /// @brief デストラクタ
  virtual
  ~CiFFCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief FFセルの時に true を返す．
  virtual
  bool
  is_ff() const;

  /// @brief FFセルの場合に次状態関数を表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  Expr
  next_state_expr() const;

  /// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  Expr
  clock_expr() const;

  /// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  Expr
  clock2_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // next_state 論理式
  Expr mNextState;

  // clock 論理式
  Expr mClock;

  // clock2 論理式
  Expr mClock2;

};


//////////////////////////////////////////////////////////////////////
/// @class CiFFRCell CiFFCell.h "CiFFCell.h"
/// @brief クリア付きのFFセルを表す CiCell の派生クラス
//////////////////////////////////////////////////////////////////////
class CiFFRCell :
  public CiFFCell
{
  friend class CiLibrary;

protected:

  /// @brief コンストラクタ
  /// @param[in] library 親のセルライブラリ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] output_array 出力の情報の配列(*1)
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] tristate_array トライステート条件の論理式の配列
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  /// @param[in] clocked_on_also "clocked_on_also" 関数の式
  /// @param[in] clear "clear" 関数の式
  /// @param[in] alloc メモリアロケータ
  /// *1: - false 論理式なし
  ///     - true 論理式あり
  CiFFRCell(CiLibrary* library,
	    ymuint id,
	    const ShString& name,
	    CellArea area,
	    ymuint ni,
	    ymuint no,
	    ymuint nio,
	    ymuint nb,
	    ymuint nc,
	    const vector<bool>& output_array,
	    const vector<Expr>& logic_array,
	    const vector<Expr>& tristate_array,
	    const Expr& next_state,
	    const Expr& clocked_on,
	    const Expr& clocked_on_also,
	    const Expr& clear,
	    Alloc& alloc);

  /// @brief デストラクタ
  virtual
  ~CiFFRCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
  virtual
  bool
  has_clear() const;

  /// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
  /// @note クリア端子がない場合の返り値は不定
  virtual
  Expr
  clear_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // clear 論理式
  Expr mClear;

};


//////////////////////////////////////////////////////////////////////
/// @class CiFFSCell CiFFCell.h "CiFFCell.h"
/// @brief プリセット付きのFFセルを表す CiCell の派生クラス
//////////////////////////////////////////////////////////////////////
class CiFFSCell :
  public CiFFCell
{
  friend class CiLibrary;

protected:

  /// @brief コンストラクタ
  /// @param[in] library 親のセルライブラリ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] output_array 出力の情報の配列(*1)
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] tristate_array トライステート条件の論理式の配列
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  /// @param[in] clocked_on_also "clocked_on_also" 関数の式
  /// @param[in] preset "preset" 関数の式
  /// @param[in] alloc メモリアロケータ
  /// *1: - false 論理式なし
  ///     - true 論理式あり
  CiFFSCell(CiLibrary* library,
	    ymuint id,
	    const ShString& name,
	    CellArea area,
	    ymuint ni,
	    ymuint no,
	    ymuint nio,
	    ymuint nb,
	    ymuint nc,
	    const vector<bool>& output_array,
	    const vector<Expr>& logic_array,
	    const vector<Expr>& tristate_array,
	    const Expr& next_state,
	    const Expr& clocked_on,
	    const Expr& clocked_on_also,
	    const Expr& preset,
	    Alloc& alloc);

  /// @brief デストラクタ
  virtual
  ~CiFFSCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
  virtual
  bool
  has_preset() const;

  /// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
  /// @note プリセット端子がない場合の返り値は不定
  virtual
  Expr
  preset_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // preset 論理式
  Expr mPreset;

};


//////////////////////////////////////////////////////////////////////
/// @class CiFFSRCell CiFFCell.h "CiFFCell.h"
/// @brief クリア付きとプリセット付きのFFセルを表す CiCell の派生クラス
//////////////////////////////////////////////////////////////////////
class CiFFSRCell :
  public CiFFRCell
{
  friend class CiLibrary;

protected:

  /// @brief コンストラクタ
  /// @param[in] library 親のセルライブラリ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] output_array 出力の情報の配列(*1)
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] tristate_array トライステート条件の論理式の配列
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  /// @param[in] clocked_on_also "clocked_on_also" 関数の式
  /// @param[in] clear "clear" 関数の式
  /// @param[in] preset "preset" 関数の式
  /// @param[in] clear_preset_var1 clear と preset が同時にオンになったときの値1
  /// @param[in] clear_preset_var2 clear と preset が同時にオンになったときの値2
  /// @param[in] alloc メモリアロケータ
  /// *1: - false 論理式なし
  ///     - true 論理式あり
  CiFFSRCell(CiLibrary* library,
	     ymuint id,
	     const ShString& name,
	     CellArea area,
	     ymuint ni,
	     ymuint no,
	     ymuint nio,
	     ymuint nb,
	     ymuint nc,
	     const vector<bool>& output_array,
	     const vector<Expr>& logic_array,
	     const vector<Expr>& tristate_array,
	     const Expr& next_state,
	     const Expr& clocked_on,
	     const Expr& clocked_on_also,
	     const Expr& clear,
	     const Expr& preset,
	     ymuint clear_preset_var1,
	     ymuint clear_preset_var2,
	     Alloc& alloc);

  /// @brief デストラクタ
  virtual
  ~CiFFSRCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
  virtual
  bool
  has_preset() const;

  /// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
  /// @note プリセット端子がない場合の返り値は不定
  virtual
  Expr
  preset_expr() const;

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

  // preset 論理式
  Expr mPreset;

  // clear_preser_var1/var2
  ymuint8 mClearPresetVal[2];

};

END_NAMESPACE_YM_CELL

#endif // CIFFCELL_H
