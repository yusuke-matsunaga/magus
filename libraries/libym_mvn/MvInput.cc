
/// @file libym_mvn/MvInput.cc
/// @brief MvInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvInput.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvInput::MvInput(MvModule* module,
		 ymuint bit_width) :
  MvNode(module, 0, 1)
{
  set_opin_bit_width(0, bit_width);
}

// @brief デストラクタ
MvInput::~MvInput()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvInput::type() const
{
  return kInput;
}

END_NAMESPACE_YM_MVN
