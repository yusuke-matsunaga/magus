
/// @file AstOp.cc
/// @brief AstOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstOp
//////////////////////////////////////////////////////////////////////

// @breif コンストラクタ
// @param[in] opcode オペコード
// @param[in] loc ファイル位置
AstOp::AstOp(OpCode opcode,
	     const FileRegion& loc) :
  AstExpr(loc),
  mOpCode(opcode)
{
}

// @brief デストラクタ
AstOp::~AstOp()
{
}

// @brief オペコードを返す．
//
// 演算子のみ有効
OpCode
AstOp::opcode() const
{
  return mOpCode;
}

END_NAMESPACE_YM_YMSL
