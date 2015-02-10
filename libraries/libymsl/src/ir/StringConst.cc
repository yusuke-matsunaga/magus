
/// @file StringConst.cc
/// @brief StringConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "StringConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス StringConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] type 型
StringConst::StringConst(const Type* type,
			 const char* val) :
  ConstVal(type),
  mVal(val)
{
}

// @brief デストラクタ
StringConst::~StringConst()
{
}

// @brief 文字列を返す．
const char*
StringConst::string_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL
