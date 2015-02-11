
/// @file AstModule.cc
/// @brief AstModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstModule.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstModule
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module モジュール名
// @param[in] alias エイリアス名
// @param[in] loc ファイル位置
AstModule::AstModule(AstSymbol* module,
		     AstSymbol* alias,
		     const FileRegion& loc) :
  Ast(loc),
  mModuleName(module),
  mAliasName(alias)
{
}

// @brief デストラクタ
AstModule::~AstModule()
{
}

// @brief モジュール名を返す．
AstSymbol*
AstModule::module_name() const
{
  return mModuleName;
}

// @brief エイリアス名を返す．
//
// 場合によっては NULL を返す．
AstSymbol*
AstModule::alias_name() const
{
  return mAliasName;
}

END_NAMESPACE_YM_YMSL
