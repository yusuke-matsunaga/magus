
/// @file IrStore.cc
/// @brief IrStore の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrStore.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrStore
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1, src2 オペランド
IrStore::IrStore(IrNode* src1,
		 IrNode* src2) :
  mSrc1(src1),
  mSrc2(src2)
{
}

// @brief デストラクタ
IrStore::~IrStore()
{
}

// @brief opcode を返す．
OpCode
IrStore::opcode() const
{
  return kOpStore;
}

END_NAMESPACE_YM_YMSL
