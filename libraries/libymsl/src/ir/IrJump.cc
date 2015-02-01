
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
IrJump::IrJump(OpCode opcode) :
  IrNode(opcode, NULL)
{
  mJumpAddr = NULL;
}

// @brief デストラクタ
IrJump::~IrJump()
{
}

// @brief opcode を返す．
OpCode
IrJump::opcode() const
{
  return mOpCode;
}

// @brief ジャンプ先のノードを得る．
//
// kOpJump, kOpBranchXXX のみ有効
IrNode*
IrJump::jump_addr() const
{
  return mJumpAddr;
}

// @brief ジャンプ先のノードを設定する．
// @param[in] node ジャンプ先のノード
//
// kOpJump, kOpBranchXXX のみ有効
void
IrJump::set_addr(IrNode* node)
{
  ASSERT_COND( mJumpAddr == NULL );
  mJumpAddr = node;
}

END_NAMESPACE_YM_YMSL
