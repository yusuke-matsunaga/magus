
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
YmslPrint::YmslPrint(ShString name) :
  YmslBuiltinFunc(name, 1)
{
}

// @brief デストラクタ
YmslPrint::~YmslPrint()
{
}

// @brief 本当の実行関数
// @param[in] arg_list 引数のリスト
// @param[in] ret_val 返り値を格納する変数
//
// 実際の派生クラスが実装する必要がある．
void
YmslPrint::_execute(const vector<YmslValue>& arg_list,
		    YmslValue& ret_val) const
{
  cout << arg_list[0].int_value << endl;
}

END_NAMESPACE_YM_YMSL
