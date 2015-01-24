
/// @file YmslStringConst.cc
/// @brief YmslStringConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslStringConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslStringConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
YmslStringConst::YmslStringConst(const char* val) :
  mVal(val)
{
}

// @brief デストラクタ
YmslStringConst::~YmslStringConst()
{
}

// @brief 種類を返す．
LeafType
YmslStringConst::leaf_type() const
{
  return kStringConst;
}

// @brief 文字列を返す．
//
// kStringConst の時のみ有効
const char*
YmslStringConst::string_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL
