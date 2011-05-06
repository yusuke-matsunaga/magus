
/// @file libym_ymsh/YmshAlias.cc
/// @brief YmshAlias の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/YmshAlias.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
// クラス YmshAlias
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name
YmshAlias::YmshAlias(const char* name) :
  mName(name)
{
}

// @brief デストラクタ
YmshAlias::~YmshAlias()
{
}

// @brief エイリアス名を返す．
const char*
YmshAlias::name() const
{
  return mName.c_str();
}

// @brief 値の配列を返す．
const vector<string>&
YmshAlias::value() const
{
  return mValue;
}

// @brief 値の配列を返す．
vector<string>&
YmshAlias::value()
{
  return mValue;
}

END_NAMESPACE_YM_YMSH
