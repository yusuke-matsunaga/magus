
/// @file pylogic_test.cc
/// @brief pylogic のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/pylogic.h"

int
main(int argc,
     char** argv)
{

  // プログラム名をセットする．
  Py_SetProgramName(argv[0]);

  // Python インタプリタを初期化する．
  Py_Initialize();

  // pylogic モジュールを初期化する．
  logic_init();

  int stat = Py_Main(argc, argv);

  Py_Finalize();

  return stat;
}
