
/// @file libym_mvn/MvConcat.cc
/// @brief MvConcat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvConcat.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvConcat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] ni 入力数
MvConcat::MvConcat(MvModule* module,
		   ymuint ni) :
  MvNode(module),
  mNi(ni)
{
  mInput = new MvInputPin[ni];
  for (ymuint i = 0; i < ni; ++ i) {
    init_pin(&mInput[i], i, 0);
  }
  init_pin(&mOutput, 0, 0);
}

// @brief デストラクタ
MvConcat::~MvConcat()
{
  delete [] mInput;
}

// @brief ノードの種類を得る．
MvNode::tType
MvConcat::type() const
{
  return kConcat;
}

// @brief 入力ピン数を得る．
ymuint
MvConcat::input_num() const
{
  return mNi;
}

/// @brief 入力ピンを得る．
/// @param[in] pos 位置 ( 0 <= pos < input_num() )
MvInputPin*
MvConcat::input(ymuint pos)
{
  assert_cond( pos < mNi, __FILE__, __LINE__);
  return &mInput[pos];
}

// @brief 出力ピン数を得る．
ymuint
MvConcat::output_num() const
{
  return 1;
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
MvOutputPin*
MvConcat::output(ymuint pos)
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return &mOutput;
}

END_NAMESPACE_YM_MVN
