
/// @file libym_mvn/MvConst.cc
/// @brief MvConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvConst.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
// @param[in] val 値
MvConst::MvConst(MvModule* module,
		 ymuint bit_width,
		 const vector<ymuint32>& val) :
  MvNode(module),
  mVal(val)
{
  init_pin(&mPin, 0, bit_width);
}

// @brief デストラクタ
MvConst::~MvConst()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvConst::type() const
{
  return kConst;
}

// @brief 入力ピン数を得る．
ymuint
MvConst::input_num() const
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
MvConst::output_num() const
{
  return 1;
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
MvOutputPin*
MvConst::output(ymuint pos)
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return &mPin;
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kConst の時のみ意味を持つ．
void
MvConst::const_value(vector<ymuint32>& val) const
{
  val = mVal;
}

END_NAMESPACE_YM_MVN
