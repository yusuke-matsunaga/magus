
/// @file libym_mvn/MvTernaryOp.cc
/// @brief MvTernaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvTernaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvTernaryOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 入力3のビット幅
// @param[in] bit_width4 出力のビット幅
MvTernaryOp::MvTernaryOp(MvModule* module,
			 ymuint bit_width1,
			 ymuint bit_width2,
			 ymuint bit_width3,
			 ymuint bit_width4) :
  MvNode(module)
{
  init_pin(&mInput[0], 0, bit_width1);
  init_pin(&mInput[1], 1, bit_width2);
  init_pin(&mInput[2], 2, bit_width3);
  init_pin(&mOutput, 0, bit_width4);
}

// @brief デストラクタ
MvTernaryOp::~MvTernaryOp()
{
}

// @brief 入力ピン数を得る．
ymuint
MvTernaryOp::input_num() const
{
  return 3;
}

/// @brief 入力ピンを得る．
/// @param[in] pos 位置 ( 0 <= pos < input_num() )
MvInputPin*
MvTernaryOp::input(ymuint pos)
{
  assert_cond( pos < 3, __FILE__, __LINE__);
  return &mInput[pos];
}

// @brief 出力ピン数を得る．
ymuint
MvTernaryOp::output_num() const
{
  return 1;
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
MvOutputPin*
MvTernaryOp::output(ymuint pos)
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return &mOutput;
}

END_NAMESPACE_YM_MVN
