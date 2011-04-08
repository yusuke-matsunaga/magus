
/// @file libym_mvn/MvnPartSelect.cc
/// @brief MvnPartSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnPartSelect.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 範囲指定入力(base位置)のビット幅
// @param[in] bit_width3 範囲のビット幅(出力のビット幅)
MvnPartSelect::MvnPartSelect(MvnModule* module,
			     ymuint bit_width1,
			     ymuint bit_width2,
			     ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnPartSelect::~MvnPartSelect()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnPartSelect::type() const
{
  return kPartSelect;
}

// @brief 可変 part-select ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 範囲指定入力のビット幅
// @param[in] bit_width3 範囲のビット幅(出力のビット幅)
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_partselect(MvnModule* module,
		       ymuint bit_width1,
		       ymuint bit_width2,
		       ymuint bit_width3)
{
  MvnNode* node = new MvnPartSelect(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
