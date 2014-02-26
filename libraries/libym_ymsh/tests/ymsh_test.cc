
/// @file ymsh_test.cc
/// @brief Ymsh のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymsh/Ymsh.h"


int
main(int argc,
     char** argv)
{
  nsYm::Ymsh ymsh;

  ymsh.run();

  return 0;
}
