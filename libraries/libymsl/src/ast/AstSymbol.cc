
/// @file AstSymbol.cc
/// @brief AstSymbol の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstSymbol.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstSymbol::AstSymbol(ShString val,
		     const FileRegion& loc) :
  Ast(loc),
  mVal(val)
{
}

// @brief デストラクタ
AstSymbol::~AstSymbol()
{
}

// @brief 文字列型の値を返す．
ShString
AstSymbol::str_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL
