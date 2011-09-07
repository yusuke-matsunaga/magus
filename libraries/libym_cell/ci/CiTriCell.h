#ifndef CITRICELL_H
#define CITRICELL_H

/// @file CiTriCell.h
/// @brief CiTriCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiCell.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiTriCell CiTriCell.h "CiTriCell.h"
/// @brief トライステートセルを表す CiCell の派生クラス
//////////////////////////////////////////////////////////////////////
class CiTriCell :
  public CiCell
{
  friend class CiLibrary;

private:

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
  CiTriCell(ymuint id,
	    const ShString& name,
	    CellArea area,
	    ymuint ni,
	    ymuint no,
	    ymuint nio,
	    ymuint nb,
	    ymuint nc,
	    AllocBase& alloc,
	    const vector<LogExpr>& logic_array,
	    const vector<LogExpr>& tristate_array);

  /// @brief デストラクタ
  virtual
  ~CiTriCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief トライステートセルの場合に true を返す．
  /// @note もちろん論理セルでもある．
  /// @note 複数出力のうち1つでもトライステートなら true を返す．
  virtual
  bool
  is_tristate() const;

  /// @brief トライステートセルの場合にトライステート条件式を返す．
  /// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
  /// @note 論理式中の変数番号は入力ピン番号に対応する．
  /// @note is_tristate() が true の時のみ意味を持つ．
  /// @note 通常の論理セルの場合には定数0を返す．
  virtual
  LogExpr
  tristate_expr(ymuint pin_id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピンの three_state 条件を設定する．
  /// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
  /// @param[in] expr three_state 条件を表す論理式
  /// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
  ///  + cell->output_num() を使う．
  virtual
  void
  set_tristate_expr(ymuint opin_id,
		    const LogExpr& expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力のトライステート条件を格納する配列
  // サイズは output_num2()
  LogExpr* mTristateArray;

};

END_NAMESPACE_YM_CELL

#endif // CITRICELL_H
