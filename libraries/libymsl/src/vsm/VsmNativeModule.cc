
/// @file VsmNativeModule.cc
/// @brief VsmNativeModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmNativeModule.h"
#include "Vsm.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス VsmNativeModule
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module_builder モジュール用のビルダー
// @param[in] toplevel_builder トップレベルのコードビルダー
VsmNativeModule::VsmNativeModule(VsmModule::Builder& module_builder,
				 VsmCodeList::Builder& toplevel_builder) :
  VsmModule(module_builder),
  mToplevelCode(toplevel_builder)
{
}

// @brief デストラクタ
VsmNativeModule::~VsmNativeModule()
{
}

// @brief トップレベルの実行を行う．
// @param[in] vsm 仮想マシン
void
VsmNativeModule::execute_toplevel(Vsm& vsm) const
{
  vsm.execute(mToplevelCode, 0);
}

END_NAMESPACE_YM_YMSL
