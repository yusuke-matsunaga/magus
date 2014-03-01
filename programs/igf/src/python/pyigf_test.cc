
/// @file pyigf_utils.cc
/// @brief pyigf のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "utils/pyutils.h"
#include "pyigf.h"


int
main(int argc,
     char** argv)
{

  // プログラム名をセットする．
  Py_SetProgramName(argv[0]);

  // Python インタプリタを初期化する．
  Py_Initialize();

  // pyutils モジュールを初期化する．
  utils_init();

  // pyigf モジュールを初期化する．
  igf_init();

  int stat = Py_Main(argc, argv);

  Py_Finalize();

  return stat;
}
