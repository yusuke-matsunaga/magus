
/// @file LuaMagus.cc
/// @brief LuaMagus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"
#include "ym/LuaBdd.h"


BEGIN_NAMESPACE_MAGUS

extern
void
init_equiv(
  lua_State* L
);


// @brief Magus 拡張に関する初期化を行う．
void
LuaMagus::open_Magus()
{
  L_openlibs();

  create_module({});
  reg_module(nullptr, "magus");
  LuaBdd::init(lua_state(), "magus", "bdd");
  LuaBnet::init(lua_state(), "magus", "bnet");
  LuaClib::init(lua_state(), "magus", "clib");
  init_equiv(lua_state());
}

END_NAMESPACE_MAGUS
