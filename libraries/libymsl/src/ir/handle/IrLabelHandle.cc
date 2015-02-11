
/// @file IrLabelHandle.cc
/// @brief IrLabelHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrLabelHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrLabelHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] label ラベル
IrLabelHandle::IrLabelHandle(ShString name,
			     IrNode* label) :
  mName(name),
  mLabel(label)
{
}

// @brief デストラクタ
IrLabelHandle::~IrLabelHandle()
{
}

// @brief 名前を返す．
ShString
IrLabelHandle::name() const
{
  return mName;
}

// @brief 種類を返す．
IrHandle::HandleType
IrLabelHandle::handle_type() const
{
  return kLabel;
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
IrNode*
IrLabelHandle::label() const
{
  return mLabel;
}

END_NAMESPACE_YM_YMSL
