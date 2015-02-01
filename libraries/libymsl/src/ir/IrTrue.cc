
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

END_NAMESPACE_YM_YMSL
