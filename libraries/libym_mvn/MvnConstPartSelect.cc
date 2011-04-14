
/// @file libym_mvn/MvnConstPartSelect.cc
/// @brief MvnConstPartSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConstPartSelect.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] msb 範囲指定の MSB
// @param[in] lsb 範囲指定の LSB
MvnConstPartSelect::MvnConstPartSelect(MvnModule* module,
				       ymuint msb,
				       ymuint lsb) :
  MvnNodeBase(module, MvnNode::kConstPartSelect, 1, 1),
  mMsb(msb),
  mLsb(lsb)
{
}

// @brief デストラクタ
MvnConstPartSelect::~MvnConstPartSelect()
{
}

// @brief 範囲指定の MSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnConstPartSelect::msb() const
{
  return mMsb;
}

// @brief 範囲指定の LSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnConstPartSelect::lsb() const
{
  return mLsb;
}

// @brief part-select ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] msb 範囲指定の MSB
// @param[in] lsb 範囲指定の LSB
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_constpartselect(MvnModule* module,
			    ymuint msb,
			    ymuint lsb,
			    ymuint bit_width)
{
  MvnNode* node = new MvnConstPartSelect(module, msb, lsb);
  reg_node(node);

  node->_input(0)->mBitWidth = bit_width;
  node->_output(0)->mBitWidth = msb - lsb + 1;

  return node;
}

END_NAMESPACE_YM_MVN
