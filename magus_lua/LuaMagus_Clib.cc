
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

  int n = lua.get_top();
  if ( n != 1 && n != 2 ) {
    lua.push_boolean(false);
    lua.push_string("Error: new_clib() expects one or two arguments.");
    return 2;
  }

  // 最初の引数はファイル名とみなす．
  if ( !lua.is_string(1) ) {
    lua.push_boolean(false);
    lua.push_string("Error: new_clib(): Arg#1 should be a string.");
    return 2;
  }
  string filename = lua.to_string(1);

  // 2番目の引数がある場合にはファイルの形式を表す文字列とみなす．
  // ない場合にはファイルの形式は拡張子から推測する．
  bool mislib = false;
  bool liberty = false;
  if ( n == 1 ) {
    auto ext = filename.substr(filename.find_last_of('.') + 1);
    mislib = ( ext == "genlib" );
    liberty = ( ext == "lib" );
    if ( !mislib && !liberty ) {
      lua.push_boolean(false);
      lua.push_string("Error: new_clib(): Arg#1's extension should be \".genlib\" or \".lib\".");
      return 2;
    }
  }
  else {
    if ( !lua.is_string(2) ) {
      lua.push_boolean(false);
      lua.push_string("Error: new_clib(): Arg#2 should be a string.");
      return 2;
    }
    string type = lua.to_string(2);
    mislib = ( type == "mislib" );
    liberty = ( type == "liberty" );
    if ( !mislib && !liberty ) {
      lua.push_boolean(false);
      lua.push_string("Error: new_clib(): Arg#2 should be \"mislib\" or \"liberty\".");
      return 2;
    }
  }

  ClibCellLibrary src;
  try {
    if ( mislib ) {
      src = ClibCellLibrary::read_mislib(filename);
    }
    if ( liberty ) {
      src = ClibCellLibrary::read_liberty(filename);
    }
  }
  catch ( ClibError& error ) {
    lua.push_boolean(false);
    lua.push_string("Error: new_clib(): read failed.");
    return 2;
  }

  // メモリ領域は Lua で確保する．
  void* p = lua.new_userdata(sizeof(ClibCellLibrary));
  auto lib = new (p) ClibCellLibrary{};

  // 読んだ内容をコピーする．
  *lib = src;

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


void
LuaMagus::init_Clib()
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
