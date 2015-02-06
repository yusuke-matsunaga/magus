
/// @file IrLoad.cc
/// @brief IrLoad の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrLoad.h"
#include "Var.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrLoad
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var 変数
IrLoad::IrLoad(const Var* var) :
  IrNode(kOpLoad, var->value_type()),
  mVar(var)
{
}

// @brief デストラクタ
IrLoad::~IrLoad()
{
}

// @brief 変数を返す．
//
// kOpVarRef, kOpLoad, kOpStore のみ有効
const Var*
IrLoad::var() const
{
  return mVar;
}

END_NAMESPACE_YM_YMSL
