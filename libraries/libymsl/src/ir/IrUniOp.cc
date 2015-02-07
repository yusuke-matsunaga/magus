
/// @file IrUniOp.cc
/// @brief IrUniOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] type 型
// @param[in] src1 オペランド
IrUniOp::IrUniOp(OpCode opcode,
		 const Type* type,
		 IrNode* src1) :
  IrNode(opcode, type),
  mSrc1(src1)
{
}

// @brief デストラクタ
IrUniOp::~IrUniOp()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrUniOp::is_static() const
{
  return operand(0)->is_static();
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
IrUniOp::operand_num() const
{
  return 1;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num() )
//
// 演算子のみ有効
IrNode*
IrUniOp::operand(ymuint pos) const
{
  ASSERT_COND( pos < operand_num() );
  return mSrc1;
}

END_NAMESPACE_YM_YMSL
