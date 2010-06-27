
/// @file libym_mvn/MvUnaryOp.cc
/// @brief MvUnaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 出力のビット幅
MvUnaryOp::MvUnaryOp(MvModule* module,
		     ymuint bit_width1,
		     ymuint bit_width2) :
  MvNode(module)
{
  init_pin(&mInput, 0, bit_width1);
  init_pin(&mOutput, 0, bit_width2);
}

// @brief デストラクタ
MvUnaryOp::~MvUnaryOp()
{
}

// @brief 入力ピン数を得る．
ymuint
MvUnaryOp::input_num() const
{
  return 1;
}

/// @brief 入力ピンを得る．
/// @param[in] pos 位置 ( 0 <= pos < input_num() )
MvInputPin*
MvUnaryOp::input(ymuint pos)
{
  assert_cond( pos == 0 , __FILE__, __LINE__);
  return &mInput;
}

// @brief 出力ピン数を得る．
ymuint
MvUnaryOp::output_num() const
{
  return 1;
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
MvOutputPin*
MvUnaryOp::output(ymuint pos)
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return &mOutput;
}

END_NAMESPACE_YM_MVN
