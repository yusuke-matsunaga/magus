
/// @file libym_mvn/MvnRop.cc
/// @brief MvnRop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnRop.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnRand
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnRand::MvnRand(MvnModule* module,
		 ymuint bit_width) :
  MvnUnaryOp(module, bit_width, 1)
{
}

// @brief デストラクタ
MvnRand::~MvnRand()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnRand::type() const
{
  return kRand;
}

// @brief reduction and ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_rand(MvnModule* module,
		 ymuint bit_width)
{
  MvnNode* node = new MvnRand(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnRor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnRor::MvnRor(MvnModule* module,
	       ymuint bit_width) :
  MvnUnaryOp(module, bit_width, 1)
{
}

// @brief デストラクタ
MvnRor::~MvnRor()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnRor::type() const
{
  return kRor;
}

// @brief reduction or ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_ror(MvnModule* module,
		ymuint bit_width)
{
  MvnNode* node = new MvnRor(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnRxor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnRxor::MvnRxor(MvnModule* module,
		 ymuint bit_width) :
  MvnUnaryOp(module, bit_width, 1)
{
}

// @brief デストラクタ
MvnRxor::~MvnRxor()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnRxor::type() const
{
  return kRxor;
}

// @brief reduction xor ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_rxor(MvnModule* module,
		 ymuint bit_width)
{
  MvnNode* node = new MvnRxor(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
