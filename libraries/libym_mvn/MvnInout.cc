
/// @file libym_mvn/MvnInout.cc
/// @brief MvnInout の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnInout.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnInout::MvnInout(MvnModule* module,
		   ymuint bit_width) :
  MvnUnaryOp(module, bit_width, bit_width)
{
}

// @brief デストラクタ
MvnInout::~MvnInout()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnInout::type() const
{
  return kInout;
}

END_NAMESPACE_YM_MVN
