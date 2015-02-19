
/// @file YmslNativeFunc.cc
/// @brief YmslNativeFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslNativeFunc.h"
#include "YVSM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslNativeFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] arg_num 引数の数
// @param[in] code_list_builder コードリストの初期化用オブジェクト
YmslNativeFunc::YmslNativeFunc(ShString name,
			       ymuint arg_num,
			       const CodeList::Builder& code_list_builder) :
  YmslFunction(name, arg_num),
  mCodeList(code_list_builder)
{
}

// @brief デストラクタ
YmslNativeFunc::~YmslNativeFunc()
{
}

// @brief 組み込み関数の時 true を返す．
bool
YmslNativeFunc::is_builtin() const
{
  return false;
}

// @brief 組み込み関数の時の実行関数
// @param[in] vsm 仮想マシン
// @param[in] base ベースレジスタ
void
YmslNativeFunc::execute(YVSM& vsm,
			Ymsl_INT base) const
{
  vsm.execute(mCodeList, base);
}

END_NAMESPACE_YM_YMSL
