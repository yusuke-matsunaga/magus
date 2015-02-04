
/// @file IrIntConst.cc
/// @brief IrIntConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrIntConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrIntConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
// @param[in] val 値
IrIntConst::IrIntConst(const Type* type,
		       int val) :
  IrNode(kOpIntConst, type),
  mVal(val)
{
}

// @brief デストラクタ
IrIntConst::~IrIntConst()
{
}

// @brief 整数値を返す．
int
IrIntConst::int_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL