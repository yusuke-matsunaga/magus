
/// @file IrOp.cc
/// @brief IrOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] irtype IR型
// @param[in] opcode オペコード
// @param[in] type 値の型
IrOp::IrOp(IrType irtype,
	   OpCode opcode,
	   const Type* type) :
  IrNode(irtype, type),
  mOpCode(opcode)
{
}

// @brief デストラクタ
IrOp::~IrOp()
{
}

// @brief オペコードを返す．
//
// 演算子のみ有効
OpCode
IrOp::opcode() const
{
  return mOpCode;
}

END_NAMESPACE_YM_YMSL
