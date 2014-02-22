
/// @file MvnCellNode.cc
/// @brief MvnCellNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MvnCellNode.h"
#include "networks/MvnMgr.h"
#include "cell/Cell.h"
#include "cell/CellPin.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnCellNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] cell セル
MvnCellNode::MvnCellNode(MvnModule* module,
			 const Cell* cell) :
  MvnNodeBase(module, MvnNode::kCell, 0),
  mCell(cell)
{
}

// @brief デストラクタ
MvnCellNode::~MvnCellNode()
{
}

// @brief セルを得る．
// @note type() が kCell の時のみ意味を持つ．
const Cell*
MvnCellNode::cell() const
{
  return mCell;
}

// @brief セルの出力ピン番号を返す．
// @note type() が kCell の時のみ意味を持つ．
ymuint
MvnCellNode::cell_opin_pos() const
{
  return 0;
}

// @brief 多出力セルノードの場合の代表ノードを返す．
// @note type() が kCell の時のみ意味を持つ．
// @note 1出力セルノードの時には自分自身を返す．
const MvnNode*
MvnCellNode::cell_node() const
{
  return this;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnExtCellNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] cell_node 代表ノード
// @param[in] opos 出力ピン番号
MvnExtCellNode::MvnExtCellNode(MvnModule* module,
			       MvnNode* cell_node,
			       ymuint opos) :
  MvnNodeBase(module, MvnNode::kCell, 0),
  mCellNode(cell_node),
  mOpos(opos)
{
}

// @brief デストラクタ
MvnExtCellNode::~MvnExtCellNode()
{
}

// @brief セルを得る．
// @note type() が kCell の時のみ意味を持つ．
const Cell*
MvnExtCellNode::cell() const
{
  return mCellNode->cell();
}

// @brief セルの出力ピン番号を返す．
// @note type() が kCell の時のみ意味を持つ．
ymuint
MvnExtCellNode::cell_opin_pos() const
{
  return mOpos;
}

// @brief 多出力セルノードの場合の代表ノードを返す．
// @note type() が kCell の時のみ意味を持つ．
// @note 1出力セルノードの時には自分自身を返す．
const MvnNode*
MvnExtCellNode::cell_node() const
{
  return mCellNode;
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

  MvnCellNode* node = new MvnCellNode(module, cell);
  reg_node(node);

  for (ymuint i = 0; i < ni; ++ i) {
    node->_input(i)->mBitWidth = 1;
  }
  node->mBitWidth = 1;

  return node;
}

END_NAMESPACE_YM_NETWORKS_MVN
