
/// @file YmslAstFloat.cc
/// @brief YmslAstFloat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstFloat.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslAstFloat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
YmslAstFloat::YmslAstFloat(double val,
			   const FileRegion& loc) :
  YmslAstImpl(loc),
  mVal(val)
{
}

// @brief デストラクタ
YmslAstFloat::~YmslAstFloat()
{
}

// @brief 型を得る．
AstType
YmslAstFloat::type() const
{
  return kAstFloat;
}

// @brief 浮動小数点型の値を返す．
double
YmslAstFloat::float_val() const
{
  return mVal;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstFloat::print(ostream& s) const
{
  s << float_val();
}

END_NAMESPACE_YM_YMSL
