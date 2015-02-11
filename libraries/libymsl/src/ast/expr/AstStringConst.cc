
/// @file AstStringConst.cc
/// @brief AstStringConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStringConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstStringConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstStringConst::AstStringConst(const char* val,
			       const FileRegion& loc) :
  AstExpr(loc),
  mVal(val)
{
}

// @brief デストラクタ
AstStringConst::~AstStringConst()
{
}

// @brief 種類を返す．
AstExpr::Type
AstStringConst::expr_type() const
{
  return kStringConst;
}

// @brief 文字列値を返す．
//
// kStringConst のみ有効
const char*
AstStringConst::string_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL
