
/// @file libym_mvn/MvnRelOp.cc
/// @brief MvnRelOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnRelOp.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnEq
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnEq::MvnEq(MvnModule* module,
	     ymuint bit_width) :
  MvnBinaryOp(module, bit_width, bit_width, 1)
{
}

// @brief デストラクタ
MvnEq::~MvnEq()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnEq::type() const
{
  return kEq;
}

// @brief equal ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_equal(MvnModule* module,
		  ymuint bit_width)
{
  MvnNode* node = new MvnEq(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnLt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnLt::MvnLt(MvnModule* module,
	     ymuint bit_width) :
  MvnBinaryOp(module, bit_width, bit_width, 1)
{
}

// @brief デストラクタ
MvnLt::~MvnLt()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnLt::type() const
{
  return kLt;
}

// @brief less than ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_lt(MvnModule* module,
	       ymuint bit_width)
{
  MvnNode* node = new MvnLt(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
