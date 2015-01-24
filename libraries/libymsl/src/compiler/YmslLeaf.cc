
/// @file YmslLeaf.cc
/// @brief YmslLeaf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslLeaf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslLeaf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslLeaf::YmslLeaf()
{
}

// @brief デストラクタ
YmslLeaf::~YmslLeaf()
{
}

// @brief 本体を返す．
//
// kMemberRef, kArrayRef, kFuncCall のみ有効
YmslLeaf*
YmslLeaf::body() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief メンバのオフセットを得る．
//
// kMemberRef のみ有効
ymuint
YmslLeaf::member_offset() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 配列のインデックスを得る．
//
// kArrayRef のみ有効
YmslExpr*
YmslLeaf::array_index() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数の引数の数を得る．
//
// kFuncCall のみ有効
ymuint
YmslLeaf::arglist_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 関数の引数を得る．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
YmslExpr*
YmslLeaf::arglist_elem(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 整数値を得る．
//
// kIntConst のみ有効
int
YmslLeaf::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 実数値を得る．
//
// kFloatConst のみ有効
double
YmslLeaf::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列を得る．
//
// kStringConst のみ有効
const char*
YmslLeaf::string_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

END_NAMESPACE_YM_YMSL
