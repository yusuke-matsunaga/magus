
/// @file libym_mvn/MvnBitSelect.cc
/// @brief MvnBitSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnBitSelect.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnBitSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 ビット指定入力のビット幅
MvnBitSelect::MvnBitSelect(MvnModule* module,
			   ymuint bit_width1,
			   ymuint bit_width2) :
  MvnBinaryOp(module, bit_width1, bit_width2, 1)
{
}

// @brief デストラクタ
MvnBitSelect::~MvnBitSelect()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnBitSelect::type() const
{
  return kBitSelect;
}

// @brief 可変 bit-selectノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 ビット指定入力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_bitselect(MvnModule* module,
		      ymuint bit_width1,
		      ymuint bit_width2)
{
  MvnNode* node = new MvnBitSelect(module, bit_width1, bit_width2);
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
