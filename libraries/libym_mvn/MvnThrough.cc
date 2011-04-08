
/// @file libym_mvn/MvnThrough.cc
/// @brief MvnThrough の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnThrough.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnThrough::MvnThrough(MvnModule* module,
		       ymuint bit_width) :
  MvnUnaryOp(module, bit_width, bit_width)
{
}

// @brief デストラクタ
MvnThrough::~MvnThrough()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnThrough::type() const
{
  return kThrough;
}

// @brief through ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_through(MvnModule* module,
		    ymuint bit_width)
{
  MvnNode* node = new MvnThrough(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
