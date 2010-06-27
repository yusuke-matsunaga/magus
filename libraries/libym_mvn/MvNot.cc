
/// @file libym_mvn/MvNot.cc
/// @brief MvNot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvNot.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvNot::MvNot(MvModule* module,
	     ymuint bit_width) :
  MvUnaryOp(module, bit_width, bit_width)
{
}

// @brief デストラクタ
MvNot::~MvNot()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvNot::type() const
{
  return kNot;
}

END_NAMESPACE_YM_MVN
