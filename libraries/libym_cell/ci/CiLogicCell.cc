
/// @file CiLogicCell.cc
/// @brief CiLogicCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiLogicCell.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiLogicCell
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
CiLogicCell::CiLogicCell(ymuint id,
			 const ShString& name,
			 CellArea area,
			 ymuint ni,
			 ymuint no,
			 ymuint nio,
			 ymuint nb,
			 ymuint nc,
			 AllocBase& alloc,
			 const vector<LogExpr>& logic_array) :
  CiCell(id, name, area, ni, no, nio, nb, nc, alloc, logic_array)
{
}

// @brief デストラクタ
CiLogicCell::~CiLogicCell()
{
}

// @brief 組み合わせ論理セルの時に true を返す．
bool
CiLogicCell::is_logic() const
{
  return true;
}

END_NAMESPACE_YM_CELL
