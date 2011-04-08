
/// @file libym_mvn/MvnOutput.cc
/// @brief MvnOutput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnOutput.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnOutput::MvnOutput(MvnModule* module,
		     ymuint bit_width) :
  MvnNode(module, 1, 1)
{
  set_ipin_bit_width(0, bit_width);
  set_opin_bit_width(0, bit_width);
}

// @brief デストラクタ
MvnOutput::~MvnOutput()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnOutput::type() const
{
  return kOutput;
}

END_NAMESPACE_YM_MVN
