
/// @file AstBoolConst.cc
/// @brief AstBoolConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstBoolConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstBoolConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr_type 式の種類 (kTrue/kFalse)
// @param[in] loc ファイル位置
AstBoolConst::AstBoolConst(ExprType expr_type,
			   const FileRegion& loc) :
  AstExpr(loc),
  mExprType(expr_type)
{
}

// @brief デストラクタ
AstBoolConst::~AstBoolConst()
{
}

// @brief 種類を返す．
ExprType
AstBoolConst::expr_type() const
{
  return mExprType;
}

#if 0
// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstBoolConst::print(ostream& s) const
{
  if ( mExprType == kTrue ) {
    s << "true";
  }
  else {
    s << "false";
  }
}
#endif

END_NAMESPACE_YM_YMSL
