
/// @file gdsprint/gdsparse.cc
/// @brief GDS-II ファイルのパーサーテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"
#include "ym_gds/GdsScanner.h"
#include "ym_gds/GdsRecMgr.h"


BEGIN_NAMESPACE_YM_GDS_PARSER

int
yyparse(GdsRecMgr& mgr,
	GdsScanner& scanner);

END_NAMESPACE_YM_GDS_PARSER

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm::nsGds;

  GdsRecMgr mgr;
  GdsScanner scanner(cin);
  //GdsDumper dumper(cout);

  nsParser::yyparse(mgr, scanner);

  return 0;
}
