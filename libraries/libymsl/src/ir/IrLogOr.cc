
/// @file IrLogOr.cc
/// @brief IrLogOr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrLogOr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrLogOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1, src2 オペランド
IrLogOr::IrLogOr(IrNode* src1,
		 IrNode* src2) :
  mSrc1(src1),
  mSrc2(src2)
{
}

// @brief デストラクタ
IrLogOr::~IrLogOr()
{
}

// @brief opcode を返す．
OpCode
IrLogOr::opcode() const
{
  return kOpLogOr;
}

END_NAMESPACE_YM_YMSL
