
/// @file libym_smtlibv2/test/SmtLibShell_test.cc
/// @brief SmtLibShell のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtLibShell.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

void
SmtLibShell_test()
{
  SmtLibShell shell;

  StreamMsgHandler mh(&cerr);

  MsgMgr::reg_handler(&mh);

  shell.run();
}

END_NAMESPACE_YM_SMTLIBV2


int
main(int argc,
     char** argv)
{
  using namespace std;

  nsYm::nsSmtLibV2::SmtLibShell_test();

  return 0;
}
