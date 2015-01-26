
/// @file IrNamedtype.cc
/// @brief IrNamedtype の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNamedType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrNamedtype
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
IrNamedType::IrNamedType(ShString name) :
  mName(name)
{
}

// @brief デストラクタ
IrNamedType::~IrNamedType()
{
}

// @brief 型名を得る．
//
// enum のみ有効
ShString
IrNamedType::type_name() const
{
  return mName;
}

END_NAMESPACE_YM_YMSL
