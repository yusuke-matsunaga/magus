
/// @file VsmFunction.cc
/// @brief VsmFunction の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmFunction.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス VsmFunction
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
VsmFunction::VsmFunction(ShString name) :
  mName(name)
{
}

// @brief デストラクタ
VsmFunction::~VsmFunction()
{
}

// @brief 関数名を返す．
ShString
VsmFunction::name() const
{
  return mName;
}

END_NAMESPACE_YM_YMSL
