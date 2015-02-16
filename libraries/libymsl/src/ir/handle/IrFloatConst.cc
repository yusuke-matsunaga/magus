
/// @file IrFloatConst.cc
/// @brief IrFloatConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrFloatConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFloatConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] const_val 定数値
IrFloatConst::IrFloatConst(ShString name,
			   Ymsl_FLOAT const_val) :
  mName(name),
  mConstVal(const_val)
{
}

// @brief デストラクタ
IrFloatConst::~IrFloatConst()
{
}

// @brief 名前を返す．
ShString
IrFloatConst::name() const
{
  return mName;
}

// @brief 種類を返す．
IrHandle::HandleType
IrFloatConst::handle_type() const
{
  return kFloatConst;
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
//
// kFloatConst のみ有効
Ymsl_FLOAT
IrFloatConst::float_val() const
{
  return mConstVal;
}

END_NAMESPACE_YM_YMSL
