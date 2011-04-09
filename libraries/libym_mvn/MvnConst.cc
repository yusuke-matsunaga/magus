
/// @file libym_mvn/MvnConst.cc
/// @brief MvnConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConst.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] val 値
MvnConst::MvnConst(MvnModule* module,
		   const vector<ymuint32>& val) :
  MvnNodeBase(module, MvnNode::kConst, 0, 1),
  mVal(val)
{
}

// @brief デストラクタ
MvnConst::~MvnConst()
{
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kConst の時のみ意味を持つ．
void
MvnConst::const_value(vector<ymuint32>& val) const
{
  val = mVal;
}

// @brief constant ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @param[in] val 値
MvnNode*
MvnMgr::new_const(MvnModule* module,
		  ymuint bit_width,
		  const vector<ymuint32>& val)
{
  MvnNode* node = new MvnConst(module, val);
  reg_node(node);

  node->_output(0)->mBitWidth = bit_width;

  return node;
}

END_NAMESPACE_YM_MVN
