
/// @file YmslNamedtype.cc
/// @brief YmslNamedtype の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslNamedType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslNamedtype
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
YmslNamedType::YmslNamedType(ShString name) :
  mName(name)
{
}

// @brief デストラクタ
YmslNamedType::~YmslNamedType()
{
}

// @brief 型名を得る．
//
// enum のみ有効
ShString
YmslNamedType::type_name() const
{
  return mName;
}

END_NAMESPACE_YM_YMSL
