
/// @file YmslTrue.cc
/// @brief YmslTrue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslTrue.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslTrue
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
YmslTrue::YmslTrue()
{
}

// @brief デストラクタ
YmslTrue::~YmslTrue()
{
}

// @brief 型を返す．
LeafType
YmslTrue::leaf_type() const
{
  return kTrue;
}

END_NAMESPACE_YM_YMSL
