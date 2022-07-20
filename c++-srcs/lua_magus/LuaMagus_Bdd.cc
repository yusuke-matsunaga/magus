
/// @file LuaMagus_Bdd.cc
/// @brief LuaMagus の BDD 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

// Bdd 用のシグネチャ
const char* BDD_SIGNATURE{"Magus.Bdd"};

// Bdd を生成する共通関数
Bdd
bdd_new(
  Luapp& lua
)
{
  // メモリ領域は Lua で確保する．
  void* p = lua.new_userdata(sizeof(Bdd));
  auto bdd = new (p) Bdd{};

  // Bdd 用の metatable を取ってくる．
  lua.L_getmetatable(BDD_SIGNATURE);

  // それを今作った userdata にセットする．
  lua.set_metatable(-2);

  return bnet;
}

// Bdd 用のデストラクタ
int
bdd_gc(
  lua_State* L
)
{
  LuaMagus lua{L};

  auto bdd = lua.to_bdd(1);
  if ( bdd ) {
    // デストラクタを明示的に起動する．
    bdd->~Bdd();
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
    auto src = BnNetwork::read_iscas89(filename, clock_str);

    auto bnet = bnet_new(lua);

    // 今読み込んだネットワークの内容をムーブする．
    bnet->move(std::move(src));

    return 1;
  }
  catch ( BnetError& error ) {
    return lua.error_end("Error: read_iscas89() failed.");
  }
}

// aig ファイルを読み込む．
int
bnet_read_aig(
  lua_State* L
)
{
  LuaMagus lua{L};

  string filename{};
  string clock_str{};
  string reset_str{};

  int n = lua.get_top();
  if ( n == 1 ) {
    // ファイル名を引数にとる．
    if ( !lua.is_string(1) ) {
      return lua.error_end("Error in read_aig(): 1st argument should be a string.");
    }
    filename = lua.to_string(1);
  }
  else if ( n == 2 ) {
    // ファイル名とパラメータのテーブル
    if ( !lua.is_string(1) ) {
      return lua.error_end("Error in read_aig(): 1st argument should be a string.");
    }
    filename = lua.to_string(1);

    const int TABLE_INDEX = 2;
    if ( !lua.is_table(TABLE_INDEX) ) {
      return lua.error_end("Error in read_aig(): 2nd argument should be a table.");
    }

    if ( lua.get_string_field(TABLE_INDEX, "clock", clock_str) == Luapp::ERROR ) {
      return lua.error_end("Error in read_aig(): Illegal value for 'clock' field in 2nd argument.");
    }

    if ( lua.get_string_field(TABLE_INDEX, "reset", reset_str) == Luapp::ERROR ) {
      return lua.error_end("Error in read_aig(): Illegal value for 'reset' field in 2nd argument.");
    }

  }
  else {
    return lua.error_end("Error: read_aig() expects one or two arguments.");
  }

  try {
    auto src = BnNetwork::read_aig(filename, clock_str, reset_str);

    auto bnet = bnet_new(lua);

    // 今読み込んだネットワークの内容をムーブする．
    bnet->move(std::move(src));

    return 1;
  }
  catch ( BnetError& error ) {
    return lua.error_end("Error: read_aig() failed.");
  }
}

// aig ファイルを読み込む．
int
bnet_read_aag(
  lua_State* L
)
{
  LuaMagus lua{L};

  string filename{};
  string clock_str{};
  string reset_str{};

  int n = lua.get_top();
  if ( n == 1 ) {
    // ファイル名を引数にとる．
    if ( !lua.is_string(1) ) {
      return lua.error_end("Error in read_aag(): 1st argument should be a string.");
    }
    filename = lua.to_string(1);
  }
  else if ( n == 2 ) {
    // ファイル名とパラメータのテーブル
    if ( !lua.is_string(1) ) {
      return lua.error_end("Error in read_aag(): 1st argument should be a string.");
    }
    filename = lua.to_string(1);

    const int TABLE_INDEX = 2;
    if ( !lua.is_table(TABLE_INDEX) ) {
      return lua.error_end("Error in read_aag(): 2nd argument should be a table.");
    }

    if ( lua.get_string_field(TABLE_INDEX, "clock", clock_str) == Luapp::ERROR ) {
      return lua.error_end("Error in read_aag(): Illegal value for 'clock' field in 2nd argument.");
    }

    if ( lua.get_string_field(TABLE_INDEX, "reset", reset_str) == Luapp::ERROR ) {
      return lua.error_end("Error in read_aag(): Illegal value for 'reset' field in 2nd argument.");
    }

  }
  else {
    return lua.error_end("Error: read_aag() expects one or two arguments.");
  }

  try {
    auto src = BnNetwork::read_aag(filename, clock_str, reset_str);

    auto bnet = bnet_new(lua);

    // 今読み込んだネットワークの内容をムーブする．
    bnet->move(std::move(src));

    return 1;
  }
  catch ( BnetError& error ) {
    return lua.error_end("Error: read_aag() failed.");
  }
}

// aig ファイルを読み込む．
int
bnet_read_truth(
  lua_State* L
)
{
  LuaMagus lua{L};

  string filename{};

  int n = lua.get_top();
  if ( n == 1 ) {
    // ファイル名を引数にとる．
    if ( !lua.is_string(1) ) {
      return lua.error_end("Error in read_truth(): 1st argument should be a string.");
    }
    filename = lua.to_string(1);
  }
  else {
    return lua.error_end("Error: read_aig() expects one argument.");
  }

  try {
    auto src = BnNetwork::read_truth(filename);

    auto bnet = bnet_new(lua);

    // 今読み込んだネットワークの内容をムーブする．
    bnet->move(std::move(src));

    return 1;
  }
  catch ( BnetError& error ) {
    return lua.error_end("Error: read_truth() failed.");
  }
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

// aig 形式でファイルに書き出す．
int
bnet_write_aig(
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
    bnet->write_aig(cout);
  }
  else {
    auto filename = lua.to_string(2);

    bnet->write_aig(filename);
  }

  lua.push_boolean(true);
  return 1;
}

// aag 形式でファイルに書き出す．
int
bnet_write_aag(
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
    bnet->write_aag(cout);
  }
  else {
    auto filename = lua.to_string(2);

    bnet->write_aag(filename);
  }

  lua.push_boolean(true);
  return 1;
}

END_NONAMESPACE

void
LuaMagus::init_Bdd(
  vector<struct luaL_Reg>& mylib
)
{
  // メンバ関数(メソッド)テーブル
  static const struct luaL_Reg mt[] = {
    {nullptr,         nullptr}
  };

  // Bdd に対応する Lua の metatable を作る．
  L_newmetatable(BDD_SIGNATURE);

  // metatable 自身を __index に登録する．
  push_value(-1);
  set_field(-2, "__index");

  // デストラクタを __gc い登録する．
  push_cfunction(bdd_gc);
  set_field(-2, "__gc");

  // メソッドテーブルを登録する．
  L_setfuncs(mt, 0);

  // 生成関数を登録する．
  mylib.push_back({"read_blif",    bnet_read_blif});
  mylib.push_back({"read_iscas89", bnet_read_iscas89});
  mylib.push_back({"read_aig",     bnet_read_aig});
  mylib.push_back({"read_aag",     bnet_read_aag});
  mylib.push_back({"read_truth",   bnet_read_truth});
}


// @brief 対象が BnNetwork の時 true を返す．
bool
LuaMagus::is_bdd(
  int idx
)
{
  auto bdd = to_bdd(idx);
  return bnet != nullptr;
}

// @brief 対象を Bdd として取り出す．
Bdd*
LuaMagus::to_bnet(
  int idx
)
{
  return reinterpret_cast<Bdd*>(L_checkudata(idx, BDD_SIGNATURE));
}

END_NAMESPACE_MAGUS
