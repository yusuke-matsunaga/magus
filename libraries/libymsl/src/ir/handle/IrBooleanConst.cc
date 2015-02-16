
/// @file IrBooleanConst.cc
/// @brief IrBooleanConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrBooleanConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrBooleanConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] const_val 定数値
IrBooleanConst::IrBooleanConst(ShString name,
			       Ymsl_BOOLEAN const_val) :
  mName(name),
  mConstVal(const_val)
{
}

// @brief デストラクタ
IrBooleanConst::~IrBooleanConst()
{
}

// @brief 名前を返す．
ShString
IrBooleanConst::name() const
{
  return mName;
}

// @brief 種類を返す．
IrHandle::HandleType
IrBooleanConst::handle_type() const
{
  return kBooleanConst;
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrBooleanConst::is_static() const
{
  return true;
}

// @brief ブール値を返す．
//
// kBooleanConst のみ有効
Ymsl_BOOLEAN
IrBooleanConst::boolean_val() const
{
  return mConstVal;
}

END_NAMESPACE_YM_YMSL
