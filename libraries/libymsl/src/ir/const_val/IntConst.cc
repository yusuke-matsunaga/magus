
/// @file IntConst.cc
/// @brief IntConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IntConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IntConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
// @param[in] val 値
IntConst::IntConst(const Type* type,
		   Ymsl_INT val) :
  ConstVal(type),
  mVal(val)
{
}

// @brief デストラクタ
IntConst::~IntConst()
{
}

// @brief 整数値を返す．
Ymsl_INT
IntConst::int_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL
