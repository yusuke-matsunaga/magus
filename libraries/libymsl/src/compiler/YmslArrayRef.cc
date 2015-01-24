
/// @file YmslArrayRef.cc
/// @brief YmslArrayRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslArrayRef.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslArrayRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] body 本体
// @param[in] index インデックス
YmslArrayRef::YmslArrayRef(YmslLeaf* body,
			   YmslExpr* index) :
  mBody(body),
  mIndex(index)
{
}

// @brief デストラクタ
YmslArrayRef::~YmslArrayRef()
{
}

// @brief 型を返す．
LeafType
YmslArrayRef::leaf_type() const
{
  return kArrayRef;
}

// @brief 本体を返す．
//
// kMemberRef, kArrayRef, kFuncCall のみ有効
YmslLeaf*
YmslArrayRef::body() const
{
  return mBody;
}

// @brief 配列のインデックスを得る．
//
// kArrayRef のみ有効
YmslExpr*
YmslArrayRef::array_index() const
{
  return mIndex;
}

END_NAMESPACE_YM_YMSL
