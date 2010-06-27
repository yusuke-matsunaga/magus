
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
// @param[in] pos 位置
// @param[in] bit_width ビット幅
MvInput::MvInput(MvModule* module,
		 ymuint pos,
		 ymuint bit_width) :
  MvNode(module),
  mPos(pos)
{
  init_pin(&mPin, 0, bit_width);
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

// @brief 入力ピン数を得る．
ymuint
MvInput::input_num() const
{
  return 0;
}

/// @brief 入力ピンを得る．
/// @param[in] pos 位置 ( 0 <= pos < input_num() )
MvInputPin*
input(ymuint pos)
{
  return NULL;
}

// @brief 出力ピン数を得る．
ymuint
MvInput::output_num() const
{
  return 1;
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
MvOutputPin*
MvInput::output(ymuint pos)
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return &mPin;
}

// @brief 入力ノード/出力ノードの場合に位置を返す．
// @note type() が kInput と kOutput の時のみ意味を持つ．
ymuint
MvInput::pos() const
{
  return mPos;
}

END_NAMESPACE_YM_MVN
