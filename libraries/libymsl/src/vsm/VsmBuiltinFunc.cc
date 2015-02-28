
/// @file VsmBuiltinFunc.cc
/// @brief VsmBuiltinFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmBuiltinFunc.h"
#include "Vsm.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス VsmBuiltinFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] type 型
VsmBuiltinFunc::VsmBuiltinFunc(ShString name,
			       const Type* type) :
  VsmFunction(name, type)
{
}

// @brief デストラクタ
VsmBuiltinFunc::~VsmBuiltinFunc()
{
}

// @brief 組み込み関数の時 true を返す．
bool
VsmBuiltinFunc::is_builtin() const
{
  return true;
}

// @brief 組み込み関数の時の実行関数
// @param[in] vsm 仮想マシン
// @param[in] base ベースレジスタ
void
VsmBuiltinFunc::execute(Vsm& vsm,
			Ymsl_INT base) const
{
  vector<VsmValue> arg_list(arg_num());
  for (ymuint i = 0; i < arg_num(); ++ i) {
    arg_list[i] = vsm.read_stack(base + i);
  }

  VsmValue ret_val = _execute(arg_list);

  vsm.write_stack(base, ret_val);
}

END_NAMESPACE_YM_YMSL
