
/// @file AstImport.cc
/// @brief AstImport の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstImport.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstImport
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module モジュール名
// @param[in] alias エイリアス
// @param[in] loc ファイル位置
AstImport::AstImport(AstSymbol* module,
		     AstSymbol* alias,
		     const FileRegion& loc) :
  AstStatement(loc),
  mModule(module),
  mAlias(alias)
{
}

// @brief デストラクタ
AstImport::~AstImport()
{
}

// @brief 種類を返す．
AstStatement::Type
AstImport::stmt_type() const
{
  return kImport;
}

// @brief インポートするモジュール名を返す．
//
// kImport のみ有効
const AstSymbol*
AstImport::import_module() const
{
  return mModule;
}

// @brief インポートするモジュールのエイリアスを返す．
//
// kImport のみ有効
const AstSymbol*
AstImport::import_alias() const
{
  return mAlias;
}

END_NAMESPACE_YM_YMSL
