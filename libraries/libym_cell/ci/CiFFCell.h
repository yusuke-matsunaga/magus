#ifndef CIFFCELL_H
#define CIFFCELL_H

/// @file CiFFCell.h
/// @brief CiFFCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
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
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] alloc メモリアロケータ
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  CiFFCell(ymuint id,
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
	   const LogExpr& clocked_on);

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
  LogExpr
  next_state_expr() const;

  /// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  LogExpr
  clock_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // next_state 論理式
  LogExpr mNextState;

  // clock 論理式
  LogExpr mClock;

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
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] alloc メモリアロケータ
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  /// @param[in] clear "clear" 関数の式
  CiFFRCell(ymuint id,
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
	    const LogExpr& clear);

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
  LogExpr
  clear_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // clear 論理式
  LogExpr mClear;

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
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] alloc メモリアロケータ
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  /// @param[in] preset "preset" 関数の式
  CiFFSCell(ymuint id,
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
	    const LogExpr& preset);

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
  LogExpr
  preset_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // preset 論理式
  LogExpr mPreset;

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
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] area 面積
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  /// @param[in] nio 入出力ピン数
  /// @param[in] nb バス数
  /// @param[in] nc バンドル数
  /// @param[in] alloc メモリアロケータ
  /// @param[in] logic_array 出力の論理式の配列
  /// @param[in] next_state "next_state" 関数の式
  /// @param[in] clocked_on "clocked_on" 関数の式
  /// @param[in] clear "clear" 関数の式
  /// @param[in] preset "preset" 関数の式
  CiFFSRCell(ymuint id,
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
	     const LogExpr& preset);

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
  LogExpr
  preset_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // preset 論理式
  LogExpr mPreset;

};

END_NAMESPACE_YM_CELL

#endif // CIFFCELL_H
