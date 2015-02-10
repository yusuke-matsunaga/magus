
/// @file AstLabel.cc
/// @brief AstLabel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstLabel.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstLabel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstLabel::AstLabel(AstSymbol* label,
		   const FileRegion& loc) :
  AstStatement(loc),
  mLabel(label)
{
}

// @brief デストラクタ
AstLabel::~AstLabel()
{
}

// @brief 種類を返す．
AstStatement::Type
AstLabel::stmt_type() const
{
  return kLabel;
}

// @brief ラベルを得る．
//
// kGoto, kLabel のみ有効
const AstSymbol*
AstLabel::label() const
{
  return mLabel;
}

END_NAMESPACE_YM_YMSL
