
/// @file IrConstHandle.cc
/// @brief IrConstHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrConstHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrConstHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] const_val 定数値
IrConstHandle::IrConstHandle(ShString name,
			     const ConstVal* const_val) :
  mName(name),
  mConstVal(const_val)
{
}

// @brief デストラクタ
IrConstHandle::~IrConstHandle()
{
}

// @brief 名前を返す．
ShString
IrConstHandle::name() const
{
  return mName;
}

// @brief 種類を返す．
IrHandle::HandleType
IrConstHandle::handle_type() const
{
  return kConstant;
}

// @brief 定数値を返す．
//
// 他の要素の場合には NULL を返す．
const ConstVal*
IrConstHandle::constant() const
{
  return mConstVal;
}

END_NAMESPACE_YM_YMSL
