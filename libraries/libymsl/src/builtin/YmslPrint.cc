
/// @file YmslPrint.cc
/// @brief YmslPrint の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslPrint.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslPrint
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
YmslPrint::YmslPrint(ShString name,
		     const vector<YmslVar*>& arg_list) :
  YmslBuiltinFunc(name, arg_list)
{
}

// @brief デストラクタ
YmslPrint::~YmslPrint()
{
}

// @brief 組み込み関数の時の実行関数
// @param[in] exectable 実行環境
void
YmslPrint::execute(YmslExecutable& executable) const
{
}

END_NAMESPACE_YM_YMSL
