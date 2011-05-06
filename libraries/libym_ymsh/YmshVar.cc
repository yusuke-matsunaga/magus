
/// @file libym_ymsh/YmshVar.cc
/// @brief YmshVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/YmshVar.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
// クラス YmshVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name
YmshVar::YmshVar(const char* name) :
  mName(name)
{
}

// @brief デストラクタ
YmshVar::~YmshVar()
{
}

// @brief 変数名を返す．
const char*
YmshVar::name() const
{
  return mName.c_str();
}

// @brief 値の配列を返す．
const vector<string>&
YmshVar::value() const
{
  return mValue;
}

// @brief 値の配列を返す．
vector<string>&
YmshVar::value()
{
  return mValue;
}

END_NAMESPACE_YM_YMSH
