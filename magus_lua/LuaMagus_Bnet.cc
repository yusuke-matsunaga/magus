
/// @file LuaMagus_Beet.cc
/// @brief LuaMagus の Bnet 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"
#include "ym/BnNetwork.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

// Bnet 用のシグネチャ
const char* BNET_SIGNATURE{"Magus.Bnet"};

// BnNetwork を作る．
int
bnet_new(
  lua_State* L
)
{
  LuaMagus lua{L};

  // メモリ領域は Lua で確保する．
  void* p = lua.new_userdata(sizeof(BnNetwork));

  int n = lua.get_top();
  if ( n == 1 ) {
    // 引数なしの場合
    new (p) BnNetwork{};
  }
  else {
    // 引数ありの場合
    // ...
  }

  // BnNetwork 用の metatable を取ってくる．
  luaL_getmetatable(L, BNET_SIGNATURE);

  // それを今作った userdata にセットする．
  lua.set_metatable(-2);

  return 1;
}

// BnNetwork 用のデストラクタ
int
bnet_gc(
  lua_State* L
)
{
  LuaMagus lua{L};

  auto bnet = lua.to_bnet(1);
  if ( bnet ) {
    // デストラクタを明示的に起動する．
    bnet->~BnNetwork();
  }

  // メモリは Lua が開放する．
  return 0;
}

// blif ファイルを読み込む．
int
bnet_read_blif(
  lua_State* L
)
{
  LuaMagus lua{L};

  auto bnet = lua.to_bnet(1);

  int n = lua.get_top();
  if ( n == 2 ) {
    // ファイル名のみ
    auto filename = lua.to_string(2);
    auto src = BnNetwork::read_blif(filename);
    if ( src.node_num() == 0 ) {
      return lua.error_end("Error during BnNetwork::read_blif");
    }

    // 今読み込んだネットワークの内容をムーブする．
    bnet->move(std::move(src));
  }
  else if ( n == 3 ) {
    // ファイル名とセルライブラリ
    auto filename = lua.to_string(2);
    //bnet = BnNetwork::read_blif(filename, cell_library);
  }
  else {
    return lua.error_end("Error: BnNetwork:read_blif() expects one or two arguments.");
  }

  lua.push_boolean(true);
  return 1;
}

// iscas89 ファイルを読み込む．
int
bnet_read_iscas89(
  lua_State* L
)
{
  LuaMagus lua{L};

  auto bnet = lua.to_bnet(1);

  int n = lua.get_top();
  if ( n == 2 ) {
    // ファイル名を引数にとる．
    auto filename = lua.to_string(2);
    auto src = BnNetwork::read_iscas89(filename);
    if ( src.node_num() == 0 ) {
      return lua.error_end("Error during BnNetwork::read_iscas89");
    }

    // 今読み込んだネットワークの内容をムーブする．
    bnet->move(std::move(src));
  }
  else {
    return lua.error_end("Error: BnNetwork:read_iscas89() expects one argument.");
  }

  lua.push_boolean(true);
  return 1;
}

// blif 形式でファイルに書き出す．
int
bnet_write_blif(
  lua_State* L
)
{
  LuaMagus lua{L};

  int n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: BnNetwork:write_blif() expects one argument.");
  }

  auto bnet = lua.to_bnet(1);
  auto filename = lua.to_string(2);

  bnet->write_blif(filename);

  lua.push_boolean(true);
  return 1;
}

// iscas89 形式でファイルに書き出す．
int
bnet_write_iscas89(
  lua_State* L
)
{
  LuaMagus lua{L};

  int n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: BnNetwork:write_iscas89() expects one argument.");
  }

  auto bnet = lua.to_bnet(1);
  auto filename = lua.to_string(2);

  bnet->write_iscas89(filename);

  lua.push_boolean(true);
  return 1;
}

// 内容をクリアする．
int
bnet_clear(
  lua_State* L
)
{
  LuaMagus lua{L};

  int n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: BnNetwork:clear() expects no arguments.");
  }

  auto bnet = lua.to_bnet(1);

  bnet->clear();

  lua.push_boolean(true);
  return 1;
}

// 内容をコピーする．
int
bnet_copy(
  lua_State* L
)
{
  LuaMagus lua{L};

  int n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: BnNetwork:copy() expects two arguments.");
  }

  auto bnet = lua.to_bnet(1);
  auto src = lua.to_bnet(2);

  bnet->copy(*src);

  lua.push_boolean(true);
  return 1;
}

// 名前をセットする．
int
bnet_set_name(
  lua_State* L
)
{
  LuaMagus lua{L};

  int n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: BnNetwork:set_name() expects one argment.");
  }

  auto bnet = lua.to_bnet(1);
  auto name = lua.to_string(2);

  bnet->set_name(name);

  lua.push_boolean(true);
  return 1;
}

// 名前を得る．
int
bnet_name(
  lua_State* L
)
{
  LuaMagus lua{L};

  int n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: BnNetwork:name() expects no arguments.");
  }

  auto bnet = lua.to_bnet(1);
  auto name = bnet->name();

  lua.push_string(name.c_str());
  return 1;
}

END_NONAMESPACE


void
LuaMagus::init_Bnet()
{
  static const struct luaL_Reg mt[] = {
    {"read_blif", bnet_read_blif},
    {"read_iscas89", bnet_read_iscas89},
    {"write_blif", bnet_write_blif},
    {"write_iscas89", bnet_write_iscas89},
    {"clear", bnet_clear},
    {"copy", bnet_copy},
    {"set_name", bnet_set_name},
    {"name", bnet_name},
    {nullptr, nullptr}
  };

  // BnNetwork に対応する Lua の metatable を作る．
  L_newmetatable(BNET_SIGNATURE);

  // metatable 自身を __index に登録する．
  push_value(-1);
  set_field(-2, "__index");

  // デストラクタを __gc い登録する．
  push_cfunction(bnet_gc);
  set_field(-2, "__gc");

  // メソッドテーブルを登録する．
  luaL_setfuncs(lua_state(), mt, 0);

  // 生成関数を登録する．
  push_cfunction(bnet_new);
  set_global("new_bnet");
}


// @brief 対象が BnNetwork の時 true を返す．
bool
LuaMagus::is_bnet(
  int idx
)
{
  auto bnet = to_bnet(idx);
  return bnet != nullptr;
}

// @brief 対象を BnNetwork として取り出す．
BnNetwork*
LuaMagus::to_bnet(
  int idx
)
{
  return reinterpret_cast<BnNetwork*>(luaL_checkudata(lua_state(), idx, BNET_SIGNATURE));
}

END_NAMESPACE_MAGUS
