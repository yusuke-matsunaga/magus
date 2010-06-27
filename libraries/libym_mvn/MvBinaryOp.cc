
/// @file libym_mvn/MvBinaryOp.cc
/// @brief MvBinaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvBinaryOp::MvBinaryOp(MvModule* module,
		       ymuint bit_width1,
		       ymuint bit_width2,
		       ymuint bit_width3) :
  MvNode(module)
{
  init_pin(&mInput[0], 0, bit_width1);
  init_pin(&mInput[1], 1, bit_width2);
  init_pin(&mOutput, 0, bit_width3);
}

// @brief デストラクタ
MvBinaryOp::~MvBinaryOp()
{
}

// @brief 入力ピン数を得る．
ymuint
MvBinaryOp::input_num() const
{
  return 2;
}

/// @brief 入力ピンを得る．
/// @param[in] pos 位置 ( 0 <= pos < input_num() )
MvInputPin*
MvBinaryOp::input(ymuint pos)
{
  assert_cond( pos < 2, __FILE__, __LINE__);
  return &mInput[pos];
}

// @brief 出力ピン数を得る．
ymuint
MvBinaryOp::output_num() const
{
  return 1;
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
MvOutputPin*
MvBinaryOp::output(ymuint pos)
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return &mOutput;
}

END_NAMESPACE_YM_MVN
