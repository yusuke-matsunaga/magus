
/// @file IrStringConst.cc
/// @brief IrStringConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrStringConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrStringConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] const_val 定数値
IrStringConst::IrStringConst(ShString name,
			     Ymsl_STRING const_val) :
  mName(name),
  mConstVal(const_val)
{
}

// @brief デストラクタ
IrStringConst::~IrStringConst()
{
}

// @brief 名前を返す．
ShString
IrStringConst::name() const
{
  return mName;
}

// @brief 種類を返す．
IrHandle::HandleType
IrStringConst::handle_type() const
{
  return kStringConst;
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrStringConst::is_static() const
{
  return true;
}

// @brief ブール値を返す．
//
// kStringConst のみ有効
Ymsl_STRING
IrStringConst::string_val() const
{
  return mConstVal;
}

END_NAMESPACE_YM_YMSL
