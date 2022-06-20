
/// @file LuaMagus_Clib.cc
/// @brief LuaMagus の Clib 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"
#include "ym/ClibCellLibrary.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

// Clib 用のシグネチャ
const char* CLIB_SIGNATURE{"Magus.Clib"};

// ClibCellLibrary を作る．
ClibCellLibrary*
clib_new(
  Luapp& lua
)
{
  // メモリ領域は Lua で確保する．
  void* p = lua.new_userdata(sizeof(ClibCellLibrary));
  auto lib = new (p) ClibCellLibrary{};

  // ClibCellLibrary 用の metatable を取ってくる．
  luaL_getmetatable(lua.lua_state(), CLIB_SIGNATURE);

  // それを今作った userdata にセットする．
  lua.set_metatable(-2);

  return lib;
}

// ClibCellLibrary 用のデストラクタ
int
clib_gc(
  lua_State* L
)
{
  LuaMagus lua{L};

  auto clib = lua.to_clib(1);
  if ( clib ) {
    // デストラクタを明示的に起動する．
    clib->~ClibCellLibrary();
  }

  // メモリは Lua が開放する．
  return 0;
}

// ライブラリの内容を出力する．
int
clib_display(
  lua_State* L
)
{
  LuaMagus lua{L};

  auto clib = lua.to_clib(1);

  display_library(cout, *clib);

  return 0;
}

END_NONAMESPACE

// mislib 形式のファイルを読んで ClibCellLibrary を作る．
int
clib_read_mislib(
  lua_State* L
)
{
  LuaMagus lua{L};

  int n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: read_mislib() expects one argument.");
  }

  // 最初の引数はファイル名とみなす．
  if ( !lua.is_string(1) ) {
    return lua.error_end("Error: read_mislib(): Arg#1 should be a string.");
  }
  string filename = lua.to_string(1);

  try {
    // ファイルを読み込む．
    auto src = ClibCellLibrary::read_mislib(filename);

    // lua インタプリタの制御下にあるオブジェクトを作る．
    auto lib = clib_new(lua);

    // lib にコピーする．
    *lib = src;

    return 1;
  }
  catch ( ClibError& error ) {
    return lua.error_end("Error: new_clib(): read failed.");
  }
}

// liberty 形式のファイルを読んで ClibCellLibrary を作る．
int
clib_read_liberty(
  lua_State* L
)
{
  LuaMagus lua{L};

  int n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: read_mislib() expects one argument.");
  }

  // 最初の引数はファイル名とみなす．
  if ( !lua.is_string(1) ) {
    return lua.error_end("Error: read_mislib(): Arg#1 should be a string.");
  }
  string filename = lua.to_string(1);

  try {
    // ファイルを読み込む．
    auto src = ClibCellLibrary::read_liberty(filename);

    // lua インタプリタの制御下にあるオブジェクトを作る．
    auto lib = clib_new(lua);

    // lib にコピーする．
    *lib = src;

    return 1;
  }
  catch ( ClibError& error ) {
    return lua.error_end("Error: new_clib(): read failed.");
  }
}

void
LuaMagus::init_Clib(
  vector<struct luaL_Reg>& mylib
)
{
  static const struct luaL_Reg mt[] = {
    {"display", clib_display},
    {nullptr, nullptr}
  };

  // ClibCellLibrary に対応する Lua の metatable を作る．
  L_newmetatable(CLIB_SIGNATURE);

  // metatable 自身を __index に登録する．
  push_value(-1);
  set_field(-2, "__index");

  // デストラクタを __gc い登録する．
  push_cfunction(clib_gc);
  set_field(-2, "__gc");

  // メソッドテーブルを登録する．
  luaL_setfuncs(lua_state(), mt, 0);

  // 生成関数を登録する．
  mylib.push_back({"read_mislib", clib_read_mislib});
  mylib.push_back({"read_liberty", clib_read_liberty});
}


// @brief 対象が ClibCellLibrary の時 true を返す．
bool
LuaMagus::is_clib(
  int idx
)
{
  auto clib = to_clib(idx);
  return clib != nullptr;
}

// @brief 対象を ClibCellLibrary として取り出す．
ClibCellLibrary*
LuaMagus::to_clib(
  int idx
)
{
  auto p = luaL_checkudata(lua_state(), idx, CLIB_SIGNATURE);
  return reinterpret_cast<ClibCellLibrary*>(p);
}

END_NAMESPACE_MAGUS
