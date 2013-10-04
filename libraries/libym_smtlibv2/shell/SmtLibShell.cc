
/// @file SmtLibShell.cc
/// @brief SmtLibShell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtLibShell.h"
#include "SmtLibMgr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtLibShell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtLibShell::SmtLibShell()
{
  mMgr = new SmtLibMgr;
}

// @brief デストラクタ
SmtLibShell::~SmtLibShell()
{
  delete mMgr;
}

END_NAMESPACE_YM_SMTLIBV2
