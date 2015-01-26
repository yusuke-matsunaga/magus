
/// @file IrPrimType.cc
/// @brief IrPrimType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrPrimType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrPrimType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
IrPrimType::IrPrimType(TypeId type) :
  mType(type)
{
}

// @brief デストラクタ
IrPrimType::~IrPrimType()
{
}

// @brief 型を得る．
TypeId
IrPrimType::type_id() const
{
  return mType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
IrPrimType::print(ostream& s) const
{
  s << type_id();
}

END_NAMESPACE_YM_YMSL
