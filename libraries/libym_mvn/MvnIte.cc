
/// @file libym_mvn/MvnIte.cc
/// @brief MvnIte の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnIte.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnIte
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnIte::MvnIte(MvnModule* module,
	       ymuint bit_width) :
  MvnTernaryOp(module, 1, bit_width, bit_width, bit_width)
{
}

// @brief デストラクタ
MvnIte::~MvnIte()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnIte::type() const
{
  return kIte;
}

// @brief condition ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_ite(MvnModule* module,
		ymuint bit_width)
{
  MvnNode* node = new MvnIte(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 3, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
