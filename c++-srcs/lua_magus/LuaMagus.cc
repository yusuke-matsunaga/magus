
/// @file LuaMagus.cc
/// @brief LuaMagus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"
#include "ym/LuaBdd.h"
#include "ym/LuaBnet.h"
#include "ym/LuaClib.h"


BEGIN_NAMESPACE_MAGUS

//@ brief Magus 拡張に関する初期化を行う．
void
LuaMagus::open_Magus()
{
  vector<struct luaL_Reg> mylib;
  LuaBdd::init(lua_state(), mylib);
  LuaBnet::init(lua_state(), mylib);
  LuaClib::init(lua_state(), mylib);
  init_equiv(mylib);
  reg_module("magus", mylib);
}

// @brief 対象を BnNetwork として取り出す．
BnNetwork*
LuaMagus::to_bnet(
  int idx
)
{
  return LuaBnet::to_bnet(lua_state(), idx);
}

// @brief 対象を ClibCellLibrary として取り出す．
ClibCellLibrary*
LuaMagus::to_clib(
  int idx
)
{
  return LuaClib::to_clib(lua_state(), idx);
}

END_NAMESPACE_MAGUS
