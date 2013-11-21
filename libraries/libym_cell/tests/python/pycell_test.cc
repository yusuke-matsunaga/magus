
/// @file pycell_test.cc
/// @brief pycell のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_cell/pycell.h"

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

  // pycell モジュールを初期化する．
  cell_init();

  int stat = Py_Main(argc, argv);

  Py_Finalize();

  return stat;
}
