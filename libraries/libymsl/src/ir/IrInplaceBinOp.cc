
/// @file IrInplaceBinOp.cc
/// @brief IrInplaceBinOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrInplaceBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrInplaceBinOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] lhs_addr 左辺値
// @param[in] opr1 オペランド
IrInplaceBinOp::IrInplaceBinOp(OpCode opcode,
			       IrNode* lhs_addr,
			       IrNode* opr1) :
  IrNode(opcode, NULL),
  mAddress(lhs_addr),
  mOperand(opr1)
{
}

// @brief デストラクタ
IrInplaceBinOp::~IrInplaceBinOp()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrInplaceBinOp::is_static() const
{
  return false;
}

// @brief ロード/ストア対象のアドレスを得る．
//
// kOpLoad, kOpStore, kOpInc, kOpDec のみ有効
IrNode*
IrInplaceBinOp::address() const
{
  return mAddress;
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
IrInplaceBinOp::operand_num() const
{
  return 1;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num() )
//
// 演算子のみ有効
IrNode*
IrInplaceBinOp::operand(ymuint pos) const
{
  ASSERT_COND( pos == 0 );
  return mOperand;
}

END_NAMESPACE_YM_YMSL
