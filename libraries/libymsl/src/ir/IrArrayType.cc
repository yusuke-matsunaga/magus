
/// @file IrArrayType.cc
/// @brief IrArrayType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrArrayType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrArrayType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elem_type 要素の型
IrArrayType::IrArrayType(const IrType* elem_type) :
  mElemType(elem_type)
{
}

// @brief デストラクタ
IrArrayType::~IrArrayType()
{
}

// @brief 型を得る．
TypeId
IrArrayType::type_id() const
{
  return kArrayType;
}

// @brief 要素の型を得る．
//
// array/set/map のみ有効
const IrType*
IrArrayType::elem_type() const
{
  return mElemType;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
IrArrayType::print(ostream& s) const
{
  s << "array(";
  mElemType->print(s);
  s << ")";
}

END_NAMESPACE_YM_YMSL
