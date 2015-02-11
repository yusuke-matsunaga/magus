
/// @file AstUniOp.cc
/// @brief AstUniOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] opr オペランド
// @param[in] loc ファイル位置
AstUniOp::AstUniOp(OpCode opcode,
		   AstExpr* opr,
		   const FileRegion& loc) :
  AstOp(opcode, loc),
  mOperand(opr)
{
}

// @brief デストラクタ
AstUniOp::~AstUniOp()
{
}

// @brief 種類を返す．
AstExpr::Type
AstUniOp::expr_type() const
{
  return kUniOp;
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
AstUniOp::operand_num() const
{
  return 1;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num()
//
// 演算子のみ有効
const AstExpr*
AstUniOp::operand(ymuint pos) const
{
  ASSERT_COND( pos <= operand_num() );
  return mOperand;
}

END_NAMESPACE_YM_YMSL
