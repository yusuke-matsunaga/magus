
/// @file libym_networks/MvnCaseEq.cc
/// @brief MvnCaseEq の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnCaseEq.h"
#include "ym_networks/MvnMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] input_num 入力数
// @param[in] val 値
MvnCaseEq::MvnCaseEq(MvnModule* module,
		     const vector<ymuint32>& val) :
  MvnNodeBase(module, MvnNode::kCaseEq, 2, 1),
  mVal(val)
{
}

// @brief デストラクタ
MvnCaseEq::~MvnCaseEq()
{
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kEqX の時のみ意味を持つ．
void
MvnCaseEq::xmask(vector<ymuint32>& val) const
{
  val = mVal;
}

// @brief eqx ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @param[in] xmask Xマスク値
MvnNode*
MvnMgr::new_caseeq(MvnModule* module,
		   ymuint bit_width,
		   const vector<ymuint32>& xmask)
{
  bool has_x = false;
  for (vector<ymuint32>::const_iterator p = xmask.begin();
       p != xmask.end(); ++ p) {
    if ( *p ) {
      has_x = true;
      break;
    }
  }
  if ( has_x ) {
    MvnNode* node = new MvnCaseEq(module, xmask);
    reg_node(node);

    node->_input(0)->mBitWidth = bit_width;
    node->_input(1)->mBitWidth = bit_width;
    node->_output(0)->mBitWidth = 1;

    return node;
  }
  else {
    return new_equal(module, bit_width);
  }
}

END_NAMESPACE_YM_NETWORKS_MVN
