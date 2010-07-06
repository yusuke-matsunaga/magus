
/// @file libym_mvn/MvInst.cc
/// @brief MvInst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvInst.h"
#include "ym_mvn/MvModule.h"
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
  MvNode(module),
  mNi(submodule->input_num()),
  mNo(submodule->output_num()),
  mSubmodule(submodule)
{
  mInput = new MvInputPin[mNi];
  for (ymuint i = 0; i < mNi; ++ i) {
    init_pin(&mInput[i], i, 0);
  }
  mOutput = new MvOutputPin[mNo];
  for (ymuint i = 0; i < mNo; ++ i) {
    init_pin(&mOutput[i], i, 0);
  }
}

// @brief デストラクタ
MvInst::~MvInst()
{
  delete [] mInput;
  delete [] mOutput;
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

// @brief 下位モジュールを得る．
// @note type() が kInst の時のみ意味を持つ．
const MvModule*
MvInst::module() const
{
  return mSubmodule;
}

END_NAMESPACE_YM_MVN
