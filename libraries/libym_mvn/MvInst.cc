
/// @file libym_mvn/MvInst.cc
/// @brief MvInst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvInst.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvInst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] submodule 子のモジュール
MvInst::MvInst(MvModule* module,
	       MvModule* submodule) :
  MvNode(module)
{
  ymuint ni;
  ymuint no;
  mInput = new MvInputPin[ni];
  for (ymuint i = 0; i < ni; ++ i) {
    init_pin(&mInput[i], i, 0);
  }
  mOutput = new MvOutputPin[no];
  for (ymuint i = 0; i < no; ++ i) {
    init_pin(&mOutput[i], i, 0);
  }
}

// @brief デストラクタ
MvInst::~MvInst()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvInst::type() const
{
  return kInst;
}

// @brief 入力ピン数を得る．
ymuint
MvInst::input_num() const
{
  return mNi;
}

/// @brief 入力ピンを得る．
/// @param[in] pos 位置 ( 0 <= pos < input_num() )
MvInputPin*
MvInst::input(ymuint pos)
{
  assert_cond( pos < mNi, __FILE__, __LINE__);
  return &mInput[pos];
}

// @brief 出力ピン数を得る．
ymuint
MvInst::output_num() const
{
  return mNo;
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
MvOutputPin*
MvInst::output(ymuint pos)
{
  assert_cond( pos < mNo, __FILE__, __LINE__);
  return &mOutput[pos];
}

END_NAMESPACE_YM_MVN
