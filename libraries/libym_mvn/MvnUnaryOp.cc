
/// @file libym_mvn/MvnUnaryOp.cc
/// @brief MvnUnaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 出力のビット幅
MvnUnaryOp::MvnUnaryOp(MvnModule* module,
		       ymuint bit_width1,
		       ymuint bit_width2) :
  MvnNode(module, 1, 1)
{
  set_ipin_bit_width(0, bit_width1);
  set_opin_bit_width(0, bit_width2);
}

// @brief デストラクタ
MvnUnaryOp::~MvnUnaryOp()
{
}

END_NAMESPACE_YM_MVN
