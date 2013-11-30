#ifndef YM_LOGIC_CPPUTEST_H
#define YM_LOGIC_CPPUTEST_H

/// @file ym_logic_CppUTest.h
/// @brief libym_logic の CppUTest 用ヘッダ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"


//////////////////////////////////////////////////////////////////////
// インポートするテストグループをここに列挙する．
//////////////////////////////////////////////////////////////////////

IMPORT_TEST_GROUP(libym_logic_LogExprTestGroup);
IMPORT_TEST_GROUP(libym_logic_SmtSolverTestGroup);

#endif // YM_LOGIC_CPPUTEST_H
