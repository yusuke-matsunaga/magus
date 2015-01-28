
/// @file PrimType.cc
/// @brief PrimType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "PrimType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス PrimType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
PrimType::PrimType(TypeId type) :
  mType(type)
{
}

// @brief デストラクタ
PrimType::~PrimType()
{
}

// @brief 型を得る．
TypeId
PrimType::type_id() const
{
  return mType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
PrimType::print(ostream& s) const
{
  s << type_id();
}

END_NAMESPACE_YM_YMSL
