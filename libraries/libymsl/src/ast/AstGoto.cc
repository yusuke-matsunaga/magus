
/// @file AstGoto.cc
/// @brief AstGoto の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstGoto.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstGoto
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstGoto::AstGoto(AstSymbol* label,
		 const FileRegion& loc) :
  AstStatement(loc),
  mLabel(label)
{
}

// @brief デストラクタ
AstGoto::~AstGoto()
{
}

// @brief 種類を返す．
AstStatement::Type
AstGoto::stmt_type() const
{
  return kGoto;
}

// @brief ラベルを得る．
//
// kGoto のみ有効
const AstSymbol*
AstGoto::label() const
{
  return mLabel;
}

END_NAMESPACE_YM_YMSL
