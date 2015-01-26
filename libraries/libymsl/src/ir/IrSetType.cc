
/// @file IrSetType.cc
/// @brief IrSetType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrSetType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrSetType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elem_type 要素の型
IrSetType::IrSetType(const IrType* elem_type) :
  mElemType(elem_type)
{
}

// @brief デストラクタ
IrSetType::~IrSetType()
{
}

// @brief 型を得る．
TypeId
IrSetType::type_id() const
{
  return kSetType;
}

// @brief 要素の型を得る．
//
// array/set/map のみ有効
const IrType*
IrSetType::elem_type() const
{
  return mElemType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
IrSetType::print(ostream& s) const
{
  s << "set(";
  mElemType->print(s);
  s << ")";
}

END_NAMESPACE_YM_YMSL
