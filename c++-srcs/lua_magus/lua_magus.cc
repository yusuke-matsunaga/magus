
/// @file magus_lua.cc
/// @brief magus_lua の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"


int
main(
  int argc,
  char** argv
)
{
  MAGUS_NAMESPACE::LuaMagus lua;

  lua.open_Magus();

  return lua.main_loop(argc, argv);
}
