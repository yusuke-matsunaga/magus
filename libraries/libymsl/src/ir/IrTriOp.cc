
/// @file IrTriOp.cc
/// @brief IrTriOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrTriOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrTriOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] type 型
// @param[in] src1, src2, src3 オペランド
IrTriOp::IrTriOp(OpCode opcode,
		 const Type* type,
		 IrNode* src1,
		 IrNode* src2,
		 IrNode* src3) :
  IrNode(opcode, type)
{
  mOperand[0] = src1;
  mOperand[1] = src2;
  mOperand[2] = src3;
}

// @brief デストラクタ
IrTriOp::~IrTriOp()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrTriOp::is_static() const
{
  return operand(0)->is_static() && operand(1)->is_static() && operand(2)->is_static();
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
IrTriOp::operand_num() const
{
  return 3;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num() )
//
// 演算子のみ有効
IrNode*
IrTriOp::operand(ymuint pos) const
{
  ASSERT_COND( pos < operand_num() );
  return mOperand[pos];
}

END_NAMESPACE_YM_YMSL
