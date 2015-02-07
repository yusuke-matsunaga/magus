
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

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrReturn::is_static() const
{
  if ( return_val() != NULL ) {
    return return_val()->is_static();
  }
  return true;
}

// @brief 返り値
IrNode*
IrReturn::return_val() const
{
  return mReturnVal;
}

END_NAMESPACE_YM_YMSL
