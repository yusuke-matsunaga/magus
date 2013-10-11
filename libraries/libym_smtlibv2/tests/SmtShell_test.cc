
/// @file libym_smtlibv2/test/SmtShell_test.cc
/// @brief SmtShell のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtShell.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

void
SmtShell_test()
{
  SmtShell shell;

  try {
    shell.run();
  }
  catch (AssertError& ae) {
    cerr << ae << endl;
  }
}

END_NAMESPACE_YM_SMTLIBV2


int
main(int argc,
     char** argv)
{
  using namespace std;

  nsYm::nsSmtLibV2::SmtShell_test();

  return 0;
}
