
/// @file libym_mvn/MvnCmpl.cc
/// @brief MvnCmpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnCmpl.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnCmpl::MvnCmpl(MvnModule* module,
		 ymuint bit_width) :
  MvnUnaryOp(module, bit_width, bit_width)
{
}

// @brief デストラクタ
MvnCmpl::~MvnCmpl()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnCmpl::type() const
{
  return kCmpl;
}

// @brief cmplノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_cmpl(MvnModule* module,
		 ymuint bit_width)
{
  MvnNode* node = new MvnCmpl(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
