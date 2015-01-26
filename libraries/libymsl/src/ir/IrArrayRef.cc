
/// @file IrArrayRef.cc
/// @brief IrArrayRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrArrayRef.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrArrayRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] body 本体
// @param[in] index インデックス
IrArrayRef::IrArrayRef(IrExpr* body,
			   IrExpr* index) :
  mBody(body),
  mIndex(index)
{
}

// @brief デストラクタ
IrArrayRef::~IrArrayRef()
{
}

// @brief 型を返す．
ExprType
IrArrayRef::expr_type() const
{
  return kArrayRef;
}

// @brief 本体を返す．
//
// kMemberRef, kArrayRef のみ有効
IrExpr*
IrArrayRef::body() const
{
  return mBody;
}

// @brief 配列のインデックスを得る．
//
// kArrayRef のみ有効
IrExpr*
IrArrayRef::array_index() const
{
  return mIndex;
}

END_NAMESPACE_YM_YMSL
