
/// @file BooleanConst.cc
/// @brief BooleanConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "BooleanConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス BooleanConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
// @param[in] val 値
BooleanConst::BooleanConst(const Type* type,
			   bool val) :
  ConstVal(type),
  mVal(val)
{
}

// @brief デストラクタ
BooleanConst::~BooleanConst()
{
}

// @brief ブール値を返す．
//
// value_type() が boolean_type の時有効
Ymsl_BOOLEAN
BooleanConst::boolean_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL
