
/// @file AstArrayRef.cc
/// @brief AstArrayRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstArrayRef.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstArrayRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] body 本体の式
// @param[in] index インデックス
// @param[in] loc ファイル位置
AstArrayRef::AstArrayRef(AstLeaf* body,
			 AstExpr* index,
			 const FileRegion& loc) :
  AstLeaf(loc),
  mBody(body),
  mIndex(index)
{
}

// @brief デストラクタ
AstArrayRef::~AstArrayRef()
{
}

// @brief 種類を返す．
LeafType
AstArrayRef::leaf_type() const
{
  return kArrayRef;
}

// @brief 配列本体を返す．
//
// kArrayRef のみ有効
const AstLeaf*
AstArrayRef::body() const
{
  return mBody;
}

// @brief インデックスを返す．
//
// kArrayRef のみ有効
const AstExpr*
AstArrayRef::index() const
{
  return mIndex;
}

END_NAMESPACE_YM_YMSL
