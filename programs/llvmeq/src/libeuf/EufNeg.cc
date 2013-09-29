
/// @file EufNeg.cc
/// @brief EufNeg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufNeg.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufNeg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] operand オペランド
EufNeg::EufNeg(ymuint id,
	       EufNode* operand) :
  EufBin(id, operand, operand)
{
}

// @brief デストラクタ
EufNeg::~EufNeg()
{
}

// @brief 型を得る．
EufNode::tType
EufNeg::type() const
{
  return kNeg;
}

END_NAMESPACE_YM_LLVMEQ

