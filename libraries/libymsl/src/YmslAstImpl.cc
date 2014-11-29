
/// @file YmslAstImpl.cc
/// @brief YmslAstImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslAstImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
YmslAstImpl::YmslAstImpl(const FileRegion& loc) :
  mLoc(loc)
{
}

// @brief デストラクタ
YmslAstImpl::~YmslAstImpl()
{
}

// @brief ファイル位置を得る．
const FileRegion&
YmslAstImpl::file_region() const
{
  return mLoc;
}

// @brief 文字列型の値を返す．
const char*
YmslAstImpl::str_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 整数型の値を返す．
int
YmslAstImpl::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 浮動小数点型の値を返す．
double
YmslAstImpl::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 子供の数を返す．
ymuint
YmslAstImpl::child_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstImpl::child(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

END_NAMESPACE_YM_YMSL
