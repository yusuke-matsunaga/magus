
/// @file YmslFalse.cc
/// @brief YmslFalse の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslFalse.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslFalse
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
YmslFalse::YmslFalse()
{
}

// @brief デストラクタ
YmslFalse::~YmslFalse()
{
}

// @brief 型を返す．
LeafType
YmslFalse::leaf_type() const
{
  return kFalse;
}

END_NAMESPACE_YM_YMSL
