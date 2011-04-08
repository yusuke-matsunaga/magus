
/// @file libym_mvn/MvnInput.cc
/// @brief MvnInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnInput.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvnInput::MvnInput(MvnModule* module,
		   ymuint bit_width) :
  MvnNode(module, 0, 1)
{
  set_opin_bit_width(0, bit_width);
}

// @brief デストラクタ
MvnInput::~MvnInput()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnInput::type() const
{
  return kInput;
}

END_NAMESPACE_YM_MVN
