
/// @file LabelHandle.cc
/// @brief LabelHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LabelHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス LabelHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] label ラベル
LabelHandle::LabelHandle(ShString name,
			 IrNode* label) :
  mName(name),
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
  return mName;
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
IrNode*
LabelHandle::label() const
{
  return mLabel;
}

END_NAMESPACE_YM_YMSL
