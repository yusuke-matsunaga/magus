
/// @file YmslAstSymbol.cc
/// @brief YmslAstSymbol の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstSymbol.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslAstSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAstSymbol::YmslAstSymbol(const char* val,
			     const FileRegion& loc) :
  YmslAstImpl(loc),
  mVal(val)
{
}

// @brief デストラクタ
YmslAstSymbol::~YmslAstSymbol()
{
}

// @brief 型を得る．
AstType
YmslAstSymbol::type() const
{
  return kAstSymbol;
}

// @brief 文字列型の値を返す．
const char*
YmslAstSymbol::str_val() const
{
  return mVal;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstSymbol::print(ostream& s) const
{
  s << str_val();
}

END_NAMESPACE_YM_YMSL
