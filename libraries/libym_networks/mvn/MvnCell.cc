
/// @file libym_networks/mvn/MvnCell.cc
/// @brief MvnCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnCell.h"
#include "ym_networks/MvnMgr.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] cell セル
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] pin_pos ピン位置を収めた配列
MvnCell::MvnCell(MvnModule* module,
		 const Cell* cell,
		 ymuint ni,
		 ymuint no,
		 const vector<ymuint32>& pin_pos) :
  MvnNodeBase(module, MvnNode::kCell, ni),
  mCell(cell)
{
}

// @brief デストラクタ
MvnCell::~MvnCell()
{
}

// @brief セルを得る．
// @note type() が kCell の時のみ意味をモツ．
const Cell*
MvnCell::cell() const
{
  return mCell;
}

// @brief セルノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] cell セル
MvnNode*
MvnMgr::new_cell(MvnModule* module,
		 const Cell* cell)
{
  ymuint ni = 0;
  ymuint no = 0;
  ymuint np = cell->pin_num();
  vector<ymuint32> pin_pos(np);
  for (ymuint i = 0; i < np; ++ i) {
    const CellPin* pin = cell->pin(i);
    if ( pin->is_input() ) {
      pin_pos[i] = (ni << 1);
      ++ ni;
    }
    else if ( pin->is_output() ) {
      pin_pos[i] = (no << 1) + 1;
      ++ no;
    }
    else if ( pin->is_inout() ) {
      cout << cell->name() << " has inout pins." << endl;
      assert_not_reached(__FILE__, __LINE__);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }

  MvnCell* node = new MvnCell(module, cell, ni, no, pin_pos);
  reg_node(node);

  for (ymuint i = 0; i < ni; ++ i) {
    node->_input(i)->mBitWidth = 1;
  }
  node->mBitWidth = 1;

  return node;
}

END_NAMESPACE_YM_NETWORKS_MVN
