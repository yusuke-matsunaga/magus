
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
  IrInplaceOp(kInplaceUniOp, opcode, lhs_addr)
{
}

// @brief デストラクタ
IrInplaceUniOp::~IrInplaceUniOp()
{
}

END_NAMESPACE_YM_YMSL
