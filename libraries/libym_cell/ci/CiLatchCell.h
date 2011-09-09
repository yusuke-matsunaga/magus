#ifndef CILATCHCELL_H
#define CILATCHCELL_H

/// @file CiLatchCell.h
/// @brief CiLatchCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiCell.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiLatchCell CiLatchCell.h "CiLatchCell.h"
/// @brief ラッチセルを表す CiCell の派生クラス
//////////////////////////////////////////////////////////////////////
class CiLatchCell :
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
  /// @param[in] tristated_array トライステート条件の論理式の配列
  /// @param[in] data_in "data_in" 関数の式
  /// @param[in] enable "enable" 関数の式
  /// @param[in] enable_also "enable_also" 関数の式
  CiLatchCell(ymuint id,
	      const ShString& name,
	      CellArea area,
	      ymuint ni,
	      ymuint no,
	      ymuint nio,
	      ymuint nb,
	      ymuint nc,
	      AllocBase& alloc,
	      const vector<LogExpr>& logic_array,
	      const vector<LogExpr>& tristate_array,
	      const LogExpr& data_in,
	      const LogExpr& enable,
	      const LogExpr& enable_also);

  /// @brief デストラクタ
  virtual
  ~CiLatchCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチセルの時に true を返す．
  virtual
  bool
  is_latch() const;

  /// @brief ラッチセルの場合にデータ入力関数を表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  LogExpr
  data_in_expr() const;

  /// @brief ラッチセルの場合にイネーブル条件を表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  LogExpr
  enable_expr() const;

  /// @brief ラッチセルの場合に2つめのイネーブル条件を表す論理式を返す．
  /// @note それ以外の型の場合の返り値は不定
  virtual
  LogExpr
  enable2_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // data_in 論理式
  LogExpr mDataIn;

  // enable 論理式
  LogExpr mEnable;

  // enable2 論理式
  LogExpr mEnable2;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLatchRCell CiLatchCell.h "CiLatchCell.h"
/// @brief クリア付きのラッチセルを表す CiCell の派生クラス
//////////////////////////////////////////////////////////////////////
class CiLatchRCell :
  public CiLatchCell
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
  /// @param[in] tristated_array トライステート条件の論理式の配列
  /// @param[in] data_in "data_in" 関数の式
  /// @param[in] enable "enable" 関数の式
  /// @param[in] enable_also "enable_also" 関数の式
  /// @param[in] clear "clear" 関数の式
  CiLatchRCell(ymuint id,
	       const ShString& name,
	       CellArea area,
	       ymuint ni,
	       ymuint no,
	       ymuint nio,
	       ymuint nb,
	       ymuint nc,
	       AllocBase& alloc,
	       const vector<LogExpr>& logic_array,
	       const vector<LogExpr>& tristate_array,
	       const LogExpr& data_in,
	       const LogExpr& enable,
	       const LogExpr& enable_also,
	       const LogExpr& clear);

  /// @brief デストラクタ
  virtual
  ~CiLatchRCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチセルの場合にクリア端子を持っていたら true を返す．
  virtual
  bool
  has_clear() const;

  /// @brief ラッチセルの場合にクリア条件を表す論理式を返す．
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
/// @class CiLatchSCell CiLatchCell.h "CiLatchCell.h"
/// @brief プリセット付きのラッチセルを表す CiCell の派生クラス
//////////////////////////////////////////////////////////////////////
class CiLatchSCell :
  public CiLatchCell
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
  /// @param[in] tristated_array トライステート条件の論理式の配列
  /// @param[in] data_in "data_in" 関数の式
  /// @param[in] enable "enable" 関数の式
  /// @param[in] enable_also "enable_also" 関数の式
  /// @param[in] preset "preset" 関数の式
  CiLatchSCell(ymuint id,
	       const ShString& name,
	       CellArea area,
	       ymuint ni,
	       ymuint no,
	       ymuint nio,
	       ymuint nb,
	       ymuint nc,
	       AllocBase& alloc,
	       const vector<LogExpr>& logic_array,
	       const vector<LogExpr>& tristate_array,
	       const LogExpr& data_in,
	       const LogExpr& enable,
	       const LogExpr& enable_also,
	       const LogExpr& preset);

  /// @brief デストラクタ
  virtual
  ~CiLatchSCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチセルの場合にプリセット端子を持っていたら true を返す．
  virtual
  bool
  has_preset() const;

  /// @brief ラッチセルの場合にプリセット条件を表す論理式を返す．
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
/// @class CiLatchSRCell CiLatchCell.h "CiLatchCell.h"
/// @brief クリア付きとプリセット付きのラッチセルを表す CiCell の派生クラス
//////////////////////////////////////////////////////////////////////
class CiLatchSRCell :
  public CiLatchRCell
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
  /// @param[in] tristated_array トライステート条件の論理式の配列
  /// @param[in] data_in "data_in" 関数の式
  /// @param[in] enable "enable" 関数の式
  /// @param[in] enable_also "enable_also" 関数の式
  /// @param[in] clear "clear" 関数の式
  /// @param[in] preset "preset" 関数の式
  CiLatchSRCell(ymuint id,
		const ShString& name,
		CellArea area,
		ymuint ni,
		ymuint no,
		ymuint nio,
		ymuint nb,
		ymuint nc,
		AllocBase& alloc,
		const vector<LogExpr>& logic_array,
		const vector<LogExpr>& tristate_array,
		const LogExpr& data_in,
		const LogExpr& enable,
		const LogExpr& enable_also,
		const LogExpr& clear,
		const LogExpr& preset);

  /// @brief デストラクタ
  virtual
  ~CiLatchSRCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチセルの場合にプリセット端子を持っていたら true を返す．
  virtual
  bool
  has_preset() const;

  /// @brief ラッチセルの場合にプリセット条件を表す論理式を返す．
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

#endif // CILATCHCELL_H
