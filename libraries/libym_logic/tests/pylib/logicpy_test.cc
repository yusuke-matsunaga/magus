
/// @file logicpy_test.cc
/// @brief logicpy のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "logicpy.h"


int
main(int argc,
     char** argv)
{
  extern void logic_init();

  // プログラム名をセットする．
  Py_SetProgramName(argv[0]);

  // Python インタプリタを初期化する．
  Py_Initialize();

  // logicpy モジュールを初期化する．
  logic_init();

  int stat = Py_Main(argc, argv);

  Py_Finalize();

  return stat;
}
