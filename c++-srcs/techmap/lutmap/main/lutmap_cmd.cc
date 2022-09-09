
/// @file lutmap_cmd.cc
/// @brief LuaMagus_lutmap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"
#include "Bn2Sbj.h"
#include "SbjGraph.h"
#include "AreaCover.h"
#include "DelayCover.h"
#include "CutHolder.h"
#include "CutResub.h"
#include "MapGen.h"
#include "MapRecord.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

// 面積最小化 DAG covering のヒューリスティック関数
int
area_map(
  lua_State* L
)
{
  using namespace nsLutmap;

  LuaMagus lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 && n != 3 ) {
    return lua.error_end("Error in area_map(): expects two or trhee arguments.");
  }

  // 1つめは BnNetwork
  if ( !lua.is_bnet(1) ) {
    return lua.error_end("Error in area_map(): 1st argument should be a BnNetwork.");
  }
  auto src_net = lua.to_bnet(1);

  // 2つめはカットサイズ
  bool ok;
  lua_Integer lut_size;
  tie(ok, lut_size) = lua.to_integer(2);
  if ( !ok ) {
    return lua.error_end("Error in area_map(): 2nd argument should be an integer.");
  }

  // 3つめはオプションテーブル
  bool fanout_mode{false};
  bool do_cut_resub{true};
  if ( n == 3 ) {
    if ( !lua.is_table(3) ) {
      return lua.error_end("Error in area_map(): 3rd argument should be a table.");
    }
  }

  SbjGraph sbjgraph;
  Bn2Sbj bn2sbj;
  bn2sbj.convert(*src_net, sbjgraph);

  // カットを列挙する．
  CutHolder cut_holder;
  cut_holder.enum_cut(sbjgraph, lut_size);

  int slack = -1;

  // 最良カットを記録する．
  MapRecord maprec;

  // 本当は mAlgorithm に応じた処理を行う．
  AreaCover area_cover(fanout_mode);
  area_cover.record_cuts(sbjgraph, cut_holder, maprec);

  if ( do_cut_resub ) {
    // cut resubstituion
    CutResub cut_resub;
    cut_resub(sbjgraph, cut_holder, maprec, slack);
  }

  // 最終的なネットワークを生成する．
  auto dst_net = lua.new_bnet();
  MapGen gen;
  SizeType lut_num;
  SizeType depth;
  *dst_net = gen.generate(sbjgraph, maprec, lut_num, depth);
  lua.push_integer(lut_num);
  lua.push_integer(depth);

  return 3;
}

// 遅延最小化 DAG covering のヒューリスティック関数
int
delay_map(
  lua_State* L
)
{
  return 0;
}

END_NONAMESPACE


void
init_lutmap(
  lua_State* L
)
{
  static const struct luaL_Reg table[] = {
    {"area_map", area_map},
    {"delay_map", delay_map},
    {nullptr, nullptr}
  };
  luaL_newlib(L, table);

  Luapp lua{L};
  lua.reg_module("magus", "lutmap");
}

END_NAMESPACE_MAGUS
