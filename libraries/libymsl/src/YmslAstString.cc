
/// @file YmslAstString.cc
/// @brief YmslAstString の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslAstString
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAstString::YmslAstString(const char* val,
			     const FileRegion& loc) :
  YmslAstImpl(loc),
  mVal(val)
{
}

// @brief デストラクタ
YmslAstString::~YmslAstString()
{
}

// @brief 型を得る．
AstType
YmslAstString::type() const
{
  return kAstString;
}

// @brief 文字列型の値を返す．
const char*
YmslAstString::str_val() const
{
  return mVal;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstString::print(ostream& s) const
{
  s << str_val();
}

END_NAMESPACE_YM_YMSL
