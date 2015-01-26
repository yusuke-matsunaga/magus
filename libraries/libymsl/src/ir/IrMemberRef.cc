
/// @file IrMemberRef.cc
/// @brief IrMemberRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMemberRef.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMemberRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] body 本体
// @param[in] offset メンバのオフセット
IrMemberRef::IrMemberRef(IrExpr* body,
			     ymuint offset) :
  mBody(body),
  mOffset(offset)
{
}

// @brief デストラクタ
IrMemberRef::~IrMemberRef()
{
}

// @brief 型を返す．
ExprType
IrMemberRef::expr_type() const
{
  return kMemberRef;
}

// @brief 本体を返す．
//
// kMemberRef, kArrayRef のみ有効
IrExpr*
IrMemberRef::body() const
{
  return mBody;
}

// @brief メンバのオフセットを得る．
//
// kMemberRef のみ有効
ymuint
IrMemberRef::member_offset() const
{
  return mOffset;
}

END_NAMESPACE_YM_YMSL
