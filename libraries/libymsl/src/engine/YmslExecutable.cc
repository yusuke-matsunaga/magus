
/// @file YmslExecutable.cc
/// @brief YmslExecutable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslExecutable.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslExecutable
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslExecutable::YmslExecutable()
{
}

// @brief デストラクタ
YmslExecutable::~YmslExecutable()
{
}

// @brief top-level の関数を得る．
YmslFunc*
YmslExecutable::top_level() const
{
  return func(0);
}

// @brief 関数を得る．
// @param[in] findex 関数インデックス
YmslFunc*
YmslExecutable::func(ymuint findex) const
{
  return mFuncTable[findex];
}

// @brief グローバル変数を得る．
// @param[in] gindex 変数インデックス
YmslVar*
YmslExecutable::global_var(ymuint gindex) const
{
  return mGlobalVarTable[gindex];
}

END_NAMESPACE_YM_YMSL
