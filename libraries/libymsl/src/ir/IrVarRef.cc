
/// @file IrVarRef.cc
/// @brief IrVarRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrVarRef.h"
#include "Var.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrVarRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var 変数
IrVarRef::IrVarRef(const Var* var) :
  IrNode(kOpVarRef, var->value_type()),
  mVar(var)
{
}

// @brief デストラクタ
IrVarRef::~IrVarRef()
{
}

// @brief 変数を返す．
const Var*
IrVarRef::var() const
{
  return mVar;
}

END_NAMESPACE_YM_YMSL
