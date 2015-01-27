
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
// @param[in] num モジュール数
// @param[in] module_list モジュールのリスト
// @param[in] loc ファイル位置
AstImport::AstImport(ymuint num,
		     AstSymbol** module_list,
		     const FileRegion& loc) :
  AstStatement(loc),
  mNum(num),
  mModuleList(module_list)
{
}

// @brief デストラクタ
AstImport::~AstImport()
{
}

// @brief 種類を返す．
StmtType
AstImport::stmt_type() const
{
  return kImport;
}

// @brief インポートするモジュール数を返す．
//
// kImport のみ有効
ymuint
AstImport::import_num() const
{
  return mNum;
}

// @brief インポートするモジュール名を返す．
// @param[in] pos 位置 ( 0 <= pos < inport_num() )
//
// kImport のみ有効
const AstSymbol*
AstImport::import_module(ymuint pos) const
{
  ASSERT_COND( pos < import_num() );
  return mModuleList[pos * 2 + 0];
}

// @brief インポートするモジュールのエイリアスを返す．
// @param[in] pos 位置 ( 0 <= pos < inport_num() )
//
// kImport のみ有効
const AstSymbol*
AstImport::import_alias(ymuint pos) const
{
  ASSERT_COND( pos < import_num() );
  return mModuleList[pos * 2 + 1];
}

END_NAMESPACE_YM_YMSL
