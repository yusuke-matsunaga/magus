#ifndef YM_UTILS_CPPUTEST_H
#define YM_UTILS_CPPUTEST_H

/// @file ym_utils_CppUTest.h
/// @brief libym_utils 用の CppUTest ヘッダ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"


//////////////////////////////////////////////////////////////////////
// インポートするテストグループをここに列挙する．
//////////////////////////////////////////////////////////////////////

IMPORT_TEST_GROUP(libym_utils_CombiGenTestGroup);
IMPORT_TEST_GROUP(libym_utils_FileTestGroup);
IMPORT_TEST_GROUP(libym_utils_FragAllocTestGroup);
IMPORT_TEST_GROUP(libym_utils_PermGenTestGroup);
IMPORT_TEST_GROUP(libym_utils_SimpleAllocTestGroup);
IMPORT_TEST_GROUP(libym_utils_UnitAllocTestGroup);
IMPORT_TEST_GROUP(libym_utils_HeapTreeTestGroup);
IMPORT_TEST_GROUP(libym_utils_BtgMatchTestGroup);

#endif // YM_UTILS_CPPUTEST_H
