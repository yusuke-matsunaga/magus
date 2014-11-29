
/// @file YmslAstInt.cc
/// @brief YmslAstInt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstInt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslAstInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAstInt::YmslAstInt(int val,
		       const FileRegion& loc) :
  YmslAstImpl(loc),
  mVal(val)
{
}

// @brief デストラクタ
YmslAstInt::~YmslAstInt()
{
}

// @brief 型を得る．
AstType
YmslAstInt::type() const
{
  return kAstInt;
}

// @brief 整数型の値を返す．
int
YmslAstInt::int_val() const
{
  return mVal;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstInt::print(ostream& s) const
{
  s << int_val();
}

END_NAMESPACE_YM_YMSL
