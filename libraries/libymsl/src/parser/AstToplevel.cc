
/// @file AstToplevel.cc
/// @brief AstToplevel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstToplevel.h"

#include "YmslCodeList.h"
#include "YmslScope.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstToplevel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt_list ステートメントリスト
// @param[in] loc ファイル位置
AstToplevel::AstToplevel(AstStmtList* stmt_list,
			 const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc)
{
}

// @brief デストラクタ
AstToplevel::~AstToplevel()
{
}

END_NAMESPACE_YM_YMSL
