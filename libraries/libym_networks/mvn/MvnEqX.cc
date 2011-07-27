
/// @file libym_networks/MvnEqX.cc
/// @brief MvnEqX の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnEqX.h"
#include "ym_networks/MvnMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] val 値
MvnEqX::MvnEqX(MvnModule* module,
	       const vector<ymuint32>& val) :
  MvnNodeBase(module, MvnNode::kEqX, 2, 1),
  mVal(val)
{
}

// @brief デストラクタ
MvnEqX::~MvnEqX()
{
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kEqX の時のみ意味を持つ．
void
MvnEqX::xmask(vector<ymuint32>& val) const
{
  val = mVal;
}

// @brief eqx ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @param[in] val 値
MvnNode*
MvnMgr::new_equalx(MvnModule* module,
		   ymuint bit_width,
		   const vector<ymuint32>& val)
{
  MvnNode* node = new MvnEqX(module, val);
  reg_node(node);

  node->_input(0)->mBitWidth = bit_width;
  node->_input(1)->mBitWidth = bit_width;
  node->_output(0)->mBitWidth = 1;

  return node;
}

END_NAMESPACE_YM_NETWORKS
