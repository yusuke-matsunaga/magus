
/// @file src/pt/PtUnaryOp.cc
/// @brief PtUnaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtUnaryOp.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] opr1 オペランド
PtUnaryOp::PtUnaryOp(const FileRegion& file_region,
		     PtNode* opr1) :
  PtNode(file_region),
  mOpr1(opr1)
{
}

// @brief デストラクタ
PtUnaryOp::~PtUnaryOp()
{
}

// @brief オペランド数を返す．
ymuint
PtUnaryOp::operand_num() const
{
  return 1;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
PtNode*
PtUnaryOp::operand(ymuint pos) const
{
  return mOpr1;
}

END_NAMESPACE_YM_BB

