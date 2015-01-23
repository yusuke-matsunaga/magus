
/// @file YmslPrimType.cc
/// @brief YmslPrimType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslPrimType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslPrimType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
YmslPrimType::YmslPrimType(TypeId type) :
  mType(type)
{
}

// @brief デストラクタ
YmslPrimType::~YmslPrimType()
{
}

// @brief 型を得る．
TypeId
YmslPrimType::type_id() const
{
  return mType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
YmslPrimType::print(ostream& s) const
{
  s << type_id();
}

END_NAMESPACE_YM_YMSL
