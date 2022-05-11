
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
int
clib_new(
  lua_State* L
)
{
  LuaMagus lua{L};

  // メモリ領域は Lua で確保する．
  void* p = lua.new_userdata(sizeof(ClibCellLibrary));

  int n = lua.get_top();
  if ( n == 1 ) {
    // 引数なしの場合
    new (p) ClibCellLibrary{};
  }
  else {
    // 引数ありの場合
    // ...
  }

  // ClibCellLibrary 用の metatable を取ってくる．
  luaL_getmetatable(L, CLIB_SIGNATURE);

  // それを今作った userdata にセットする．
  lua.set_metatable(-2);

  return 1;
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

// mislib ファイルを読み込む．
int
clib_read_mislib(
  lua_State* L
)
{
  LuaMagus lua{L};

  auto clib = lua.to_clib(1);

  int n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: ClibCellLibrary:read_mislib() expects one argument.");
  }

  // 引数はファイル名のみ
  auto filename = lua.to_string(2);

  auto src = ClibCellLibrary::read_mislib(filename);
  if ( src.cell_num() == 0 ) {
    return lua.error_end("Error: ClibCellLibrary:read_mislib() failed.");
  }

  // 今読み込んだセルライブラリをコピーする．
  *clib = src;

  lua.push_boolean(true);
  return 1;
}

// liberty ファイルを読み込む．
int
clib_read_liberty(
  lua_State* L
)
{
  LuaMagus lua{L};

  auto clib = lua.to_clib(1);

  int n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: ClibCellLibrary:read_liberty() expects one argument.");
  }

  // 引数はファイル名のみ
  auto filename = lua.to_string(2);

  auto src = ClibCellLibrary::read_liberty(filename);
  if ( src.cell_num() == 0 ) {
    return lua.error_end("Error: ClibCellLibrary:read_liberty() failed.");
  }

  // 今読み込んだセルライブラリをコピーする．
  *clib = src;

  lua.push_boolean(true);
  return 1;
}

END_NONAMESPACE


void
LuaMagus::init_Clib()
{
  static const struct luaL_Reg mt[] = {
    {"read_mislib", clib_read_mislib},
    {"read_liberty", clib_read_liberty},
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
  push_cfunction(clib_new);
  set_global("new_clib");
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
