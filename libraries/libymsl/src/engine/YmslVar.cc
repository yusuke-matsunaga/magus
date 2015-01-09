
/// @file YmslVar.cc
/// @brief YmslVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslVar.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] value_type 型
// @param[in] index インデックス
YmslVar::YmslVar(ShString name,
		 YmslType* value_type,
		 int index) :
  mName(name),
  mValueType(value_type),
  mIndex(index)
{
}

// @brief デストラクタ
YmslVar::~YmslVar()
{
}

// @brief 変数名を得る．
ShString
YmslVar::name() const
{
  return mName;
}

// @brief 型を得る．
const YmslType*
YmslVar::value_type() const
{
  return mValueType;
}

// @brief 変数インデックスを得る．
//
// 正ならグローバル変数
// 負ならローカル変数
int
YmslVar::index() const
{
  return mIndex;
}

END_NAMESPACE_YM_YMSL
