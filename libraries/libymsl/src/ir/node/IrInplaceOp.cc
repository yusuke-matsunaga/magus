
/// @file IrInplaceOp.cc
/// @brief IrInplaceOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrInplaceOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrInplaceOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] irtype IR型
// @param[in] opcode オペコード
// @param[in] lhs_addr 左辺値
IrInplaceOp::IrInplaceOp(IrType irtype,
			 OpCode opcode,
			 IrHandle* lhs_addr) :
  IrOp(irtype, opcode, NULL),
  mAddress(lhs_addr)
{
}

// @brief デストラクタ
IrInplaceOp::~IrInplaceOp()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrInplaceOp::is_static() const
{
  return false;
}

// @brief ロード/ストア対象のアドレスを得る．
//
// kOpLoad, kOpStore, kOpInc, kOpDec のみ有効
IrHandle*
IrInplaceOp::address() const
{
  return mAddress;
}

END_NAMESPACE_YM_YMSL
