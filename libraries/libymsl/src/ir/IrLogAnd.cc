
/// @file IrLogAnd.cc
/// @brief IrLogAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrLogAnd.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrLogAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1, src2 オペランド
IrLogAnd::IrLogAnd(IrNode* src1,
	       IrNode* src2) :
  mSrc1(src1),
  mSrc2(src2)
{
}

// @brief デストラクタ
IrLogAnd::~IrLogAnd()
{
}

// @brief opcode を返す．
OpCode
IrLogAnd::opcode() const
{
  return kOpLogAnd;
}

END_NAMESPACE_YM_YMSL
