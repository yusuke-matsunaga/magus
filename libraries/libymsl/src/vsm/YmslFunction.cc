
/// @file YmslFunction.cc
/// @brief YmslFunction の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslFunction.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslFunction
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] arg_num 引数の数
YmslFunction::YmslFunction(ShString name,
			   ymuint arg_num) :
  mName(name),
  mArgNum(arg_num)
{
}

// @brief デストラクタ
YmslFunction::~YmslFunction()
{
}

// @brief 関数名を返す．
ShString
YmslFunction::name() const
{
  return mName;
}

// @brief 引数の数
ymuint
YmslFunction::arg_num() const
{
  return mArgNum;
}

END_NAMESPACE_YM_YMSL
