
/// @file IrFalse.cc
/// @brief IrFalse の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFalse.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFalse
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
IrFalse::IrFalse(const Type* type) :
  IrNode(kOpFalse, type)
{
}

// @brief デストラクタ
IrFalse::~IrFalse()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrFalse::is_static() const
{
  return true;
}

END_NAMESPACE_YM_YMSL
