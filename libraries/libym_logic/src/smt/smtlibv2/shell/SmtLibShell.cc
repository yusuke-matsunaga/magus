
/// @file SmtLibShell.cc
/// @brief SmtLibShell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibShell.h"
#include "ShellImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtLibShell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtLibShell::SmtLibShell()
{
  mImpl = new ShellImpl();
}

// @brief デストラクタ
SmtLibShell::~SmtLibShell()
{
  delete mImpl;
}

// @brief 実行する．
void
SmtLibShell::run()
{
  mImpl->run();
}

END_NAMESPACE_YM_SMTLIBV2
