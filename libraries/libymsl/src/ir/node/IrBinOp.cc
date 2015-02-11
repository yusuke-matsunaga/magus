
/// @file IrBinOp.cc
/// @brief IrBinOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrBinOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] type 型
// @param[in] src1, src2 オペランド
IrBinOp::IrBinOp(OpCode opcode,
		 const Type* type,
		 IrNode* src1,
		 IrNode* src2) :
  IrOp(kBinOp, opcode, type)
{
  mOperand[0] = src1;
  mOperand[1] = src2;
}

// @brief デストラクタ
IrBinOp::~IrBinOp()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrBinOp::is_static() const
{
  return operand(0)->is_static() && operand(1)->is_static();
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
IrBinOp::operand_num() const
{
  return 2;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num() )
//
// 演算子のみ有効
IrNode*
IrBinOp::operand(ymuint pos) const
{
  ASSERT_COND( pos < operand_num() );
  return mOperand[pos];
}

END_NAMESPACE_YM_YMSL
