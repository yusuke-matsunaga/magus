
/// @file AstToplevel.cc
/// @brief AstToplevel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstToplevel.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstToplevel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt_num 文のリストの要素数
// @param[in] stmt_list 文のリスト
// @param[in] loc ファイル位置
AstToplevel::AstToplevel(ymuint num,
			 AstStatement** stmt_list,
			 const FileRegion& loc) :
  AstBlockStmt(num, stmt_list, loc)
{
}

// @brief デストラクタ
AstToplevel::~AstToplevel()
{
}

// @brief 種類を返す．
StmtType
AstToplevel::stmt_type() const
{
  return kToplevel;
}

END_NAMESPACE_YM_YMSL
