
/// @file CiTriCell.cc
/// @brief CiTriCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiTriCell.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiTriCell
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
// @param[in] tristated_array トライステート条件の論理式の配列
CiTriCell::CiTriCell(ymuint id,
		     const ShString& name,
		     CellArea area,
		     ymuint ni,
		     ymuint no,
		     ymuint nio,
		     ymuint nb,
		     ymuint nc,
		     AllocBase& alloc,
		     const vector<LogExpr>& logic_array,
		     const vector<LogExpr>& tristate_array) :
  CiCell(id, name, area, ni, no, nio, nb, nc, alloc, logic_array)
{
  ymuint no2 = no + nio;
  void* u = alloc.get_memory(sizeof(LogExpr) * no2);
  mTristateArray = new (u) LogExpr[no2];

  for (ymuint i = 0; i < no2; ++ i) {
    mTristateArray[i] = tristate_array[i];
  }
}

// @brief デストラクタ
CiTriCell::~CiTriCell()
{
}

// @brief トライステートセルの場合に true を返す．
// @note もちろん論理セルでもある．
// @note 複数出力のうち1つでもトライステートなら true を返す．
bool
CiTriCell::is_tristate() const
{
  return true;
}

// @brief トライステートセルの場合にトライステート条件式を返す．
// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
// @note 論理式中の変数番号は入力ピン番号に対応する．
// @note is_tristate() が true の時のみ意味を持つ．
// @note 通常の論理セルの場合には定数0を返す．
LogExpr
CiTriCell::tristate_expr(ymuint pin_id) const
{
  return mTristateArray[pin_id];
}

// @brief 出力ピンの three_state 条件を設定する．
// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
// @param[in] expr three_state 条件を表す論理式
// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->output_num() を使う．
void
CiTriCell::set_tristate_expr(ymuint opin_id,
			     const LogExpr& expr)
{
  assert_cond( opin_id < output_num2(), __FILE__, __LINE__);
  mTristateArray[opin_id] = expr;
}

END_NAMESPACE_YM_CELL
