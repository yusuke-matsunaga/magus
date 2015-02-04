
/// @file IrJump.cc
/// @brief IrJump の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrJump.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrJump
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] label ジャンプ先のラベル
// @param[in] cond 条件
IrJump::IrJump(OpCode opcode,
	       IrNode* label,
	       IrNode* cond) :
  IrNode(opcode, NULL),
  mJumpAddr(label),
  mCond(cond)
{
}

// @brief デストラクタ
IrJump::~IrJump()
{
}

// @brief ジャンプ先のノードを得る．
//
// kOpJump, kOpBranchXXX のみ有効
IrNode*
IrJump::jump_addr() const
{
  return mJumpAddr;
}

// @brief 分岐条件
IrNode*
IrJump::branch_cond() const
{
  return mCond;
}

END_NAMESPACE_YM_YMSL
