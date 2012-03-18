
/// @file src/pt/PtBinaryOp.cc
/// @brief PtBinaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtBinaryOp.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] opr1, opr2 オペランド
PtBinaryOp::PtBinaryOp(const FileRegion& file_region,
		       PtNode* opr1,
		       PtNode* opr2) :
  PtNode(file_region)
{
  mOpr[0] = opr1;
  mOpr[1] = opr2;
}

// @brief デストラクタ
PtBinaryOp::~PtBinaryOp()
{
}

// @brief オペランド数を返す．
ymuint
PtBinaryOp::operand_num() const
{
  return 2;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
PtNode*
PtBinaryOp::operand(ymuint pos) const
{
  return mOpr[pos & 1];
}

END_NAMESPACE_YM_BB
