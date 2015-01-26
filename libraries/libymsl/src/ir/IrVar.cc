
/// @file IrVar.cc
/// @brief IrVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrVar.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] value_type 型
// @param[in] index インデックス
IrVar::IrVar(ShString name,
		 const IrType* value_type,
		 ymuint index) :
  mName(name),
  mValueType(value_type),
  mIndex(index)
{
}

// @brief デストラクタ
IrVar::~IrVar()
{
}

// @brief 変数名を得る．
ShString
IrVar::name() const
{
  return mName;
}

// @brief 型を得る．
const IrType*
IrVar::value_type() const
{
  return mValueType;
}

// @brief 変数インデックスを得る．
ymuint
IrVar::index() const
{
  return mIndex;
}

END_NAMESPACE_YM_YMSL
