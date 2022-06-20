
/// @file LuaMagus.cc
/// @brief LuaMagus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

struct luaL_Reg* magus_table = nullptr;
SizeType table_size = 0;

int
open_magus(
  lua_State* L
)
{
  luaL_checkversion(L);
  lua_createtable(L, 0, table_size);
  luaL_setfuncs(L, magus_table, 0);
  return 1;
}

END_NONAMESPACE


//@ brief Magus 拡張に関する初期化を行う．
//
// この関数を呼ばないと Luapp と同等になる．
void
LuaMagus::open_Magus()
{
  vector<struct luaL_Reg> mylib;
  init_Bnet(mylib);
  init_Clib(mylib);

  L_requiref("_G", luaopen_base, 1);
  L_requiref("package", luaopen_package, 1);
  pop(2);

  // C API には struct luaL_reg の配列が必要
  table_size = mylib.size();
  magus_table = new luaL_Reg[table_size + 1];
  for ( SizeType i = 0; i < table_size; ++ i ) {
    magus_table[i] = mylib[i];
  }
  magus_table[table_size] = {NULL, NULL};

  reg_module("magus", open_magus);

}

END_NAMESPACE_MAGUS
