
/// @file IrInplaceUniOp.cc
/// @brief IrInplaceUniOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrInplaceUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrInplaceUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] lhs_addr 左辺値
IrInplaceUniOp::IrInplaceUniOp(OpCode opcode,
			       IrHandle* lhs_addr) :
  IrNode(opcode, NULL),
  mAddress(lhs_addr)
{
}

// @brief デストラクタ
IrInplaceUniOp::~IrInplaceUniOp()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrInplaceUniOp::is_static() const
{
  return false;
}

// @brief ロード/ストア対象のアドレスを得る．
//
// kOpLoad, kOpStore, kOpInc, kOpDec のみ有効
IrHandle*
IrInplaceUniOp::address() const
{
  return mAddress;
}

END_NAMESPACE_YM_YMSL
