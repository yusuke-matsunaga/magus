
/// @file LabelHandle.cc
/// @brief LabelHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LabelHandle.h"
//#include "Label.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス LabelHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
LabelHandle::LabelHandle(Label* label) :
  mLabel(label)
{
}

// @brief デストラクタ
LabelHandle::~LabelHandle()
{
}

// @brief 名前を返す．
ShString
LabelHandle::name() const
{
  //return mLabel->name();
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
Label*
LabelHandle::label() const
{
  return mLabel;
}

END_NAMESPACE_YM_YMSL
