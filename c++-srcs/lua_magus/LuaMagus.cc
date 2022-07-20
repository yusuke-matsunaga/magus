
/// @file LuaMagus.cc
/// @brief LuaMagus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"
//#include "ym/LuaBdd.h"
#include "ym/LuaBnet.h"
#include "ym/LuaClib.h"


BEGIN_NAMESPACE_MAGUS

//@ brief Magus 拡張に関する初期化を行う．
void
LuaMagus::open_Magus()
{
  vector<struct luaL_Reg> mylib;
  //LuaBdd::init(lua_state(), mylib);
  LuaBnet::init(lua_state(), mylib);
  LuaClib::init(lua_state(), mylib);
  init_equiv(mylib);
  reg_module("magus", mylib);
}

END_NAMESPACE_MAGUS
