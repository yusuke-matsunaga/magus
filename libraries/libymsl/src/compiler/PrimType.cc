
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
  Type(type)
{
}

// @brief デストラクタ
PrimType::~PrimType()
{
}

// @brief 指定された型にキャスト可能な場合に true を返す．
// @param[in] type 指定された型
bool
PrimType::castable_to(const Type* type) const
{
  switch ( type->type_id() ) {
  case kBooleanType:
    switch ( type_id() ) {
    case kBooleanType:
    case kIntType:
    case kFloatType:
      return true;

    default:
      return false;
    }

  case kIntType:
    switch ( type_id() ) {
    case kBooleanType:
    case kIntType:
      return true;

    default:
      return false;
    }

  case kFloatType:
    switch ( type_id() ) {
    case kIntType:
    case kFloatType:
      return true;

    default:
      return false;
    }

  case kStringType:
    switch ( type_id() ) {
    case kStringType:
      return true;

    default:
      return false;
    }

  default:
    break;
  }
  return false;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
PrimType::print(ostream& s) const
{
  s << type_id();
}

END_NAMESPACE_YM_YMSL
