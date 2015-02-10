
/// @file AstBinOp.cc
/// @brief AstBinOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstBinOp
//////////////////////////////////////////////////////////////////////

// @breif コンストラクタ
// @param[in] opcode オペコード
// @param[in] left, right オペランド
AstBinOp::AstBinOp(OpCode opcode,
		   AstExpr* left,
		   AstExpr* right) :
  AstOp(opcode, FileRegion(left->file_region(), right->file_region())),
  mLeft(left),
  mRight(right)
{
}

// @brief デストラクタ
AstBinOp::~AstBinOp()
{
}

// @brief 種類を返す．
AstExpr::Type
AstBinOp::expr_type() const
{
  return kBinOp;
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
AstBinOp::operand_num() const
{
  return 2;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num()
//
// 演算子のみ有効
const AstExpr*
AstBinOp::operand(ymuint pos) const
{
  switch ( pos ) {
  case 0: return mLeft;
  case 1: return mRight;
  default: ASSERT_NOT_REACHED; break;
  }
  return NULL;
}

END_NAMESPACE_YM_YMSL
