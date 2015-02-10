
/// @file AstIntCont.cc
/// @brief AstIntConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstIntConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstIntConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstIntConst::AstIntConst(int val,
			 const FileRegion& loc) :
  AstExpr(loc),
  mVal(val)
{
}

// @brief デストラクタ
AstIntConst::~AstIntConst()
{
}

// @brief 種類を返す．
AstExpr::Type
AstIntConst::expr_type() const
{
  return kIntConst;
}

// @brief 整数値を返す．
//
// kIntConst のみ有効
int
AstIntConst::int_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL
