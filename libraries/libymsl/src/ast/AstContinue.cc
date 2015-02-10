
/// @file AstContinue.cc
/// @brief AstContinue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstContinue.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstContinue
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstContinue::AstContinue(const FileRegion& loc) :
  AstStatement(loc)
{
}

// @brief デストラクタ
AstContinue::~AstContinue()
{
}

// @brief 種類を返す．
AstStatement::Type
AstContinue::stmt_type() const
{
  return kContinue;
}

END_NAMESPACE_YM_YMSL
