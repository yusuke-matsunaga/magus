
/// @file LuaMagus_equiv.cc
/// @brief LuaMagus_equiv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"
#include "EquivMgr.h"
#include "ym/LuaBnet.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

string
b3tostr(
  SatBool3 val
)
{
  switch ( val ) {
  case SatBool3::True: return "true";
  case SatBool3::False: return "false";
  case SatBool3::X: return "unknown";
  }
  ASSERT_NOT_REACHED;
  return string{};
}

// 組み合わせ回路の等価比較を行う．
int
equiv_cmd(
  lua_State* L
)
{
  LuaMagus lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 && n != 3 ) {
    return lua.error_end("Error in equiv(): expects two or three arguments.");
  }

  EquivMgr mgr;

  // 2つの BnNetwork とオプションテーブルを引数に取る．
  if ( !lua.is_bnet(1) ) {
    return lua.error_end("Error in equiv(): 1st argument should be a BnNetwork.");
  }
  auto net1 = lua.to_bnet(1);

  if ( !lua.is_bnet(2) ) {
    return lua.error_end("Error in equiv(): 2nd argument should be a BnNetwork.");
  }
  auto net2 = lua.to_bnet(2);

  bool match_by_name{false};
  if ( n == 3 ) {
    if ( !lua.is_table(3) ) {
      return lua.error_end("Error in equiv(): 3rd argument should be a table.");
    }

    lua_Integer sig_size;
    auto ret = lua.get_int_field(3, "signature_size", sig_size);
    if ( ret == Luapp::ERROR ) {
      return lua.error_end("Error in equiv(): Illegal value for 'signature_size' in 3rd argument.");
    }
    if ( ret == Luapp::OK ) {
      mgr.set_signature_size(sig_size);
    }

    lua_Integer loglevel;
    ret = lua.get_int_field(3, "loglevel", loglevel);
    if ( ret == Luapp::ERROR ) {
      return lua.error_end("Error in equiv(): Illegal value for 'loglevel' in 3rd argument.");
    }
    if ( ret == Luapp::OK ) {
      mgr.set_loglevel(loglevel);
    }

    bool val{false};
    ret = lua.get_boolean_field(3, "match_by_name", val);
    if ( ret == Luapp::ERROR ) {
      return lua.error_end("Error in equiv(): Illegal value for 'match_by_name' in 3rd argument.");
    }
    if ( ret == Luapp::OK ) {
      match_by_name = val;
    }
  }

  // 等価検証を行う．
  auto result = mgr.check(*net1, *net2, match_by_name);

  // 結果をスタックに積む．
  lua.push_string(b3tostr(result.result()).c_str());
  const auto& oresults = result.output_results();
  SizeType no = oresults.size();
  lua.create_table(no, 0);
  for ( SizeType i = 0; i < no; ++ i ) {
    lua.push_string(b3tostr(oresults[i]).c_str());
    lua.set_table(-2, i);
  }
  return 2;
}

END_NONAMESPACE

void
LuaMagus::init_equiv(
  vector<struct luaL_Reg>& mylib
)
{
  // 生成関数を登録する．
  mylib.push_back({"check_equiv", equiv_cmd});
}

END_NAMESPACE_MAGUS
