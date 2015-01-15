
/// @file YmslSimpleType.cc
/// @brief YmslSimpleType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslSimpleType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslSimpleType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
YmslSimpleType::YmslSimpleType(TypeId type) :
  mType(type)
{
}

// @brief デストラクタ
YmslSimpleType::~YmslSimpleType()
{
}

// @brief 型を得る．
TypeId
YmslSimpleType::type_id() const
{
  return mType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
YmslSimpleType::print(ostream& s) const
{
  s << type_id();
}

END_NAMESPACE_YM_YMSL
