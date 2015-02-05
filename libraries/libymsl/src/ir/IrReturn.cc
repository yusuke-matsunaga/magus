
/// @file IrReturn.cc
/// @brief IrReturn の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrReturn.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrReturn
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ret_val 返り値を表す式
IrReturn::IrReturn(IrNode* ret_val) :
  IrNode(kOpReturn, NULL),
  mReturnVal(ret_val)
{
}

// @brief デストラクタ
IrReturn::~IrReturn()
{
}

// @brief 返り値
IrNode*
IrReturn::return_val() const
{
  return mReturnVal;
}

END_NAMESPACE_YM_YMSL
