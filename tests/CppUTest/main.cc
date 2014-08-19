
/// @file main.cc
/// @brief テストプログラムのメイン関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/CommandLineTestRunner.h"

#if HAVE_LIBYM_UTILS
#include "CppUTest/ym_utils_CppUTest.h"
#endif

#if HAVE_LIBYM_UTILS
#include "CppUTest/ym_logic_CppUTest.h"
#endif


int
main(int argc,
     char** argv)
{
  RUN_ALL_TESTS(argc, argv);
}
