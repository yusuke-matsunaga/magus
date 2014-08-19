
/// @file CiFsmCell.cc
/// @brief CiFsmCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CiFsmCell.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiFsmCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] library 親のセルライブラリ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nit 内部ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristated_array トライステート条件の論理式の配列
// @param[in] alloc メモリアロケータ
// *1: - false 論理式なし
//     - true 論理式あり
CiFsmCell::CiFsmCell(CiLibrary* library,
		     ymuint id,
		     const ShString& name,
		     CellArea area,
		     ymuint ni,
		     ymuint no,
		     ymuint nio,
		     ymuint nit,
		     ymuint nb,
		     ymuint nc,
		     const vector<bool>& output_array,
		     const vector<Expr>& logic_array,
		     const vector<Expr>& tristate_array,
		     Alloc& alloc) :
  CiCell(library, id, name, area,
	 ni, no, nio, nit, nb, nc,
	 output_array,
	 logic_array,
	 tristate_array,
	 alloc)
{
}

// @brief デストラクタ
CiFsmCell::~CiFsmCell()
{
}

// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
bool
CiFsmCell::is_fsm() const
{
  return true;
}

END_NAMESPACE_YM_CELL
