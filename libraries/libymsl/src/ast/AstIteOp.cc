
/// @file AstIteOp.cc
/// @brief AstIteOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstIteOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstIteOp
//////////////////////////////////////////////////////////////////////

// @breif コンストラクタ
// @param[in] opr1, opr2, opr3 オペランド
AstIteOp::AstIteOp(AstExpr* opr1,
		   AstExpr* opr2,
		   AstExpr* opr3) :
  AstOp(kOpIte, FileRegion(opr1->file_region(), opr3->file_region()))
{
  mOpr[0] = opr1;
  mOpr[1] = opr2;
  mOpr[2] = opr3;
}

// @brief デストラクタ
AstIteOp::~AstIteOp()
{
}

// @brief 種類を返す．
AstExpr::Type
AstIteOp::expr_type() const
{
  return kTriOp;
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
AstIteOp::operand_num() const
{
  return 3;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num()
//
// 演算子のみ有効
const AstExpr*
AstIteOp::operand(ymuint pos) const
{
  ASSERT_COND( pos < operand_num() );
  return mOpr[pos];
}

END_NAMESPACE_YM_YMSL
