
/// @file IrTrue.cc
/// @brief IrTrue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrTrue.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrTrue
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
IrTrue::IrTrue(const Type* type) :
  IrNode(kOpTrue, type)
{
}

// @brief デストラクタ
IrTrue::~IrTrue()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrTrue::is_static() const
{
  return true;
}

END_NAMESPACE_YM_YMSL
