
/// @file LuaMagus_Bnet.cc
/// @brief LuaMagus の Bnet 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "ym/ClibCellLibrary.h"

#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

// Bnet 用のシグネチャ
const char* BNET_SIGNATURE{"Magus.Bnet"};

// BnNetwork を生成する共通関数
BnNetwork*
bnet_new(
  Luapp& lua
)
{
  // メモリ領域は Lua で確保する．
  void* p = lua.new_userdata(sizeof(BnNetwork));
  auto bnet = new (p) BnNetwork{};

  // BnNetwork 用の metatable を取ってくる．
  luaL_getmetatable(lua.lua_state(), BNET_SIGNATURE);

  // それを今作った userdata にセットする．
  lua.set_metatable(-2);

  return bnet;
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

// blif 形式でファイルに書き出す．
int
bnet_write_blif(
  lua_State* L
)
{
  LuaMagus lua{L};

  int n = lua.get_top();
  if ( n != 1 && n != 2 ) {
    return lua.error_end("Error: BnNetwork:write_blif() expects at most one argument.");
  }

  auto bnet = lua.to_bnet(1);

  if ( n == 1 ) {
    bnet->write_blif(cout);
  }
  else {
    auto filename = lua.to_string(2);

    bnet->write_blif(filename);
  }

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
  if ( n != 1 && n != 2 ) {
    return lua.error_end("Error: BnNetwork:write_iscas89() expects at most one argument.");
  }

  auto bnet = lua.to_bnet(1);

  if ( n == 1 ) {
    bnet->write_iscas89(cout);
  }
  else {
    auto filename = lua.to_string(2);

    bnet->write_iscas89(filename);
  }

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
  if ( n != 1 ) {
    return lua.error_end("Error: BnNetwork:copy() expects one argument.");
  }

  auto src = lua.to_bnet(1);

  auto bnet = bnet_new(lua);

  bnet->copy(*src);

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

// 諸元を格納したターブルを作る．
int
bnet_stats(
  lua_State* L
)
{
  LuaMagus lua{L};

  int n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: BnNetwork:stats() expects no arguments.");
  }

  // 対象のネットワーク
  auto bnet = lua.to_bnet(1);

  // 結果のテーブルを作る．
  lua.create_table();
  int tbl_idx = lua.absindex(-1);

  { // cell_library
    auto lib = bnet->library();
    if ( lib.cell_num() > 0 ) {
      ;
    }
  }

  { // port_num
    SizeType n = bnet->port_num();
    lua.push_integer(n);
    lua.set_field(tbl_idx, "port_num");
  }
  { // dff_num
    SizeType n = bnet->dff_num();
    lua.push_integer(n);
    lua.set_field(tbl_idx, "dff_num");
  }
  { // dff_num
    SizeType n = bnet->dff_num();
    lua.push_integer(n);
    lua.set_field(tbl_idx, "dff_num");
  }
  { // latch_num
    SizeType n = bnet->latch_num();
    lua.push_integer(n);
    lua.set_field(tbl_idx, "latch_num");
  }
  { // input_num
    SizeType n = bnet->input_num();
    lua.push_integer(n);
    lua.set_field(tbl_idx, "input_num");
  }
  { // output_num
    SizeType n = bnet->output_num();
    lua.push_integer(n);
    lua.set_field(tbl_idx, "output_num");
  }
  { // logic_num
    SizeType n = bnet->logic_num();
    lua.push_integer(n);
    lua.set_field(tbl_idx, "logic_num");
  }
  if ( 0 ) { // literal_sum
    SizeType n = 0;
    for ( SizeType id: bnet->logic_id_list() ) {
      const auto& node = bnet->node(id);
      SizeType expr_id = node.expr_id();
      auto expr = bnet->expr(expr_id);
      n += expr.literal_num();
    }
    lua.push_integer(n);
    lua.set_field(tbl_idx, "literal_sum");
  }

  return 1;
}

END_NONAMESPACE

// blif ファイルを読み込む．
int
bnet_read_blif(
  lua_State* L
)
{
  LuaMagus lua{L};

  string clock_str{};
  string reset_str{};
  ClibCellLibrary library{};
  string filename{};

  int n = lua.get_top();
  if ( n == 1 ) {
    // ファイル名のみ
    if ( !lua.is_string(1) ) {
      return lua.error_end("Error in read_blif(): 1st argument should be a string.");
    }
    filename = lua.to_string(1);
  }
  else if ( n == 2 ) {
    // ファイル名とパラメータのテーブル
    if ( !lua.is_string(1) ) {
      return lua.error_end("Error in read_blif(): 1st argument should be a string.");
    }
    filename = lua.to_string(1);

    const int TABLE_INDEX = 2;
    if ( !lua.is_table(TABLE_INDEX) ) {
      return lua.error_end("Error in read_blif(): 2nd argument should be a table.");
    }
    auto ret = lua.get_field(TABLE_INDEX, "cell_library");
    if ( ret == LUA_TNIL ) {
      // 存在しなかった．
      // エラーではない．
      ;
    }
    else {
      auto lib = lua.to_clib(-1);
      if ( lib == nullptr ) {
	return lua.error_end("Error in read_blif(): ClibCellLibrary required for 'cell_library' field.");
      }
      library = *lib;
    }
    // get_filed(TABLE_INDEX, "cell_library") で積まれた要素を棄てる．
    lua.pop(1);

    if ( lua.get_string_field(TABLE_INDEX, "clock", clock_str) == Luapp::ERROR ) {
      return lua.error_end("Error in read_blif(): Illegal value for 'clock' field in 2nd argument.");
    }
    if ( lua.get_string_field(TABLE_INDEX, "reset", reset_str) == Luapp::ERROR ) {
      return lua.error_end("Error in read_blif(): Illegal value for 'reset' field in 2nd argument.");
    }
  }
  else {
    return lua.error_end("Error: read_blif() expects one or two arguments.");
  }

  try {
    auto src = BnNetwork::read_blif(filename, library, clock_str, reset_str);

    auto bnet = bnet_new(lua);

    // 今読み込んだネットワークの内容をムーブする．
    bnet->move(std::move(src));

    return 1;
  }
  catch ( BnetError& error ) {
    return lua.error_end("Error: read_blif() failed.");
  }
}

// iscas89 ファイルを読み込む．
int
bnet_read_iscas89(
  lua_State* L
)
{
  LuaMagus lua{L};

  string filename{};
  string clock_str{};

  int n = lua.get_top();
  if ( n == 1 ) {
    // ファイル名を引数にとる．
    if ( !lua.is_string(1) ) {
      return lua.error_end("Error in read_iscas89(): 1st argument should be a string.");
    }
    filename = lua.to_string(1);
  }
  else if ( n == 2 ) {
    // ファイル名とパラメータのテーブル
    if ( !lua.is_string(1) ) {
      return lua.error_end("Error in read_iscas89(): 1st argument should be a string.");
    }
    filename = lua.to_string(1);

    const int TABLE_INDEX = 2;
    if ( !lua.is_table(TABLE_INDEX) ) {
      return lua.error_end("Error in read_iscas89(): 2nd argument should be a table.");
    }

    if ( lua.get_string_field(TABLE_INDEX, "clock", clock_str) == Luapp::ERROR ) {
      return lua.error_end("Error in read_iscas89(): Illegal value for 'clock' field in 2nd argument.");
    }

  }
  else {
    return lua.error_end("Error: read_iscas89() expects one or two arguments.");
  }

  try {
    auto src = BnNetwork::read_iscas89(filename);

    auto bnet = bnet_new(lua);

    // 今読み込んだネットワークの内容をムーブする．
    bnet->move(std::move(src));

    return 1;
  }
  catch ( BnetError& error ) {
    return lua.error_end("Error: read_iscas89() failed.");
  }
}


void
LuaMagus::init_Bnet(
  vector<struct luaL_Reg>& mylib
)
{
  // メンバ関数(メソッド)テーブル
  static const struct luaL_Reg mt[] = {
    {"write_blif", bnet_write_blif},
    {"write_iscas89", bnet_write_iscas89},
    {"clear", bnet_clear},
    {"copy", bnet_copy},
    {"set_name", bnet_set_name},
    {"name", bnet_name},
    {"stats", bnet_stats},
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
  mylib.push_back({"read_blif", bnet_read_blif});
  mylib.push_back({"read_iscas89", bnet_read_iscas89});
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
