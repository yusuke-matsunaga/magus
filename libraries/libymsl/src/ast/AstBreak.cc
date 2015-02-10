
/// @file AstBreak.cc
/// @brief AstBreak の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstBreak.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstBreak
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstBreak::AstBreak(const FileRegion& loc) :
  AstStatement(loc)
{
}

// @brief デストラクタ
AstBreak::~AstBreak()
{
}

// @brief 種類を返す．
AstStatement::Type
AstBreak::stmt_type() const
{
  return kBreak;
}

END_NAMESPACE_YM_YMSL
