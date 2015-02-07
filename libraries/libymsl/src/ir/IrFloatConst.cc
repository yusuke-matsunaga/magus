
/// @file IrFloatConst.cc
/// @brief IrFloatConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFloatConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFloatConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
// @param[in] val 値
IrFloatConst::IrFloatConst(const Type* type,
			   double val) :
  IrNode(kOpFloatConst, type),
  mVal(val)
{
}

// @brief デストラクタ
IrFloatConst::~IrFloatConst()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrFloatConst::is_static() const
{
  return true;
}

// @brief 実数値を返す．
double
IrFloatConst::float_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL
