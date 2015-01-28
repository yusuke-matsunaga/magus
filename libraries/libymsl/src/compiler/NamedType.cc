
/// @file Namedtype.cc
/// @brief Namedtype の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NamedType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Namedtype
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
NamedType::NamedType(ShString name) :
  mName(name)
{
}

// @brief デストラクタ
NamedType::~NamedType()
{
}

// @brief 型名を得る．
//
// enum のみ有効
ShString
NamedType::type_name() const
{
  return mName;
}

END_NAMESPACE_YM_YMSL
