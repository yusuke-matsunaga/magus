
/// @file YmslBuiltinFunc.cc
/// @brief YmslBuiltinFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslBuiltinFunc.h"
#include "YmslVSM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslBuiltinFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] arg_num 引数の数
YmslBuiltinFunc::YmslBuiltinFunc(ShString name,
				 ymuint arg_num) :
  YmslFunction(name, arg_num)
{
}

// @brief デストラクタ
YmslBuiltinFunc::~YmslBuiltinFunc()
{
}

// @brief 組み込み関数の時 true を返す．
bool
YmslBuiltinFunc::is_builtin() const
{
  return true;
}

// @brief 組み込み関数の時の実行関数
// @param[in] vsm 仮想マシン
// @param[in] base ベースレジスタ
void
YmslBuiltinFunc::execute(YmslVSM& vsm,
			 Ymsl_INT base) const
{
  vector<YmslValue> arg_list(arg_num());
  for (ymuint i = 0; i < arg_num(); ++ i) {
    arg_list[i] = vsm.read_stack(base + i);
  }

  YmslValue ret_val;
  _execute(arg_list, ret_val);

  vsm.write_stack(base, ret_val);
}

END_NAMESPACE_YM_YMSL
