
/// @file AstNullStmt.cc
/// @brief AstNullStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstNullStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstNullStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstNullStmt::AstNullStmt(const FileRegion& loc) :
  AstStatement(loc)
{
}

// @brief デストラクタ
AstNullStmt::~AstNullStmt()
{
}

// @brief 種類を返す．
AstStatement::Type
AstNullStmt::stmt_type() const
{
  return kNullStmt;
}

END_NAMESPACE_YM_YMSL
