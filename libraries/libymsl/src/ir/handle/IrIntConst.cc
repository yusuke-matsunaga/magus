
/// @file IrIntConst.cc
/// @brief IrIntConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrIntConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrIntConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] const_val 定数値
IrIntConst::IrIntConst(ShString name,
		       Ymsl_INT const_val) :
  mName(name),
  mConstVal(const_val)
{
}

// @brief デストラクタ
IrIntConst::~IrIntConst()
{
}

// @brief 名前を返す．
ShString
IrIntConst::name() const
{
  return mName;
}

// @brief 種類を返す．
IrHandle::HandleType
IrIntConst::handle_type() const
{
  return kIntConst;
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrIntConst::is_static() const
{
  return true;
}

// @brief 整数値を返す．
//
// kIntConst のみ有効
Ymsl_INT
IrIntConst::int_val() const
{
  return mConstVal;
}

END_NAMESPACE_YM_YMSL
