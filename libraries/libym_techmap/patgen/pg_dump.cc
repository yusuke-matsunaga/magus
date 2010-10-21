
/// @file libym_techmap/patgen/pg_dump.cc
/// @brief pg_dump() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatGen.h"
#include "PgNode.h"
#include "../PatMgr.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

BEGIN_NONAMESPACE

void
display_edge(ostream& s,
	     PgNode* node,
	     ymuint fanin_pos)
{
  s << "Edge#" << node->id() * 2 + fanin_pos<< ": ";
  if ( node->is_input() ) {
    s << "---";
  }
  else {
    s << "Node#" << node->fanin(fanin_pos)->id()
      << " -> Node#" << node->id()
      << "(" << fanin_pos << ")";
    if ( node->fanin_inv(fanin_pos) ) {
      s << "[inv]";
    }
  }
  s << endl;
}

void
display_dfs(ostream& s,
	    PgNode* node,
	    vector<bool>& vmark)
{
  if ( node->is_input() || vmark[node->id()] ) {
    return;
  }
  vmark[node->id()] = true;
  s << " Edge#" << node->id() * 2;
  display_dfs(s, node->fanin(0), vmark);
  s << " Edge#" << node->id() * 2 + 1;
  display_dfs(s, node->fanin(1), vmark);
}

END_NONAMESPACE

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
// @note ダンプされた情報はそのまま PatGraph で読み込むことができる．
void
pg_display(ostream& s,
	   const PatGen& pat_gen)
{
  ymuint n = pat_gen.node_num();

  s << "*** NODE SECTION ***" << endl;
  for (ymuint i = 0; i < n; ++ i) {
    PgNode* node = pat_gen.node(i);
    s << "Node#" << node->id() << ": ";
    if ( node->is_input() ) {
      s << "Input#" << node->input_id();
    }
    else if ( node->is_and() ) {
      s << "And";
    }
    else if ( node->is_xor() ) {
      s << "Xor";
    }
    s << endl;
  }

  s << " *** EDGE SECTION ***" << endl;
  for (ymuint i = 0; i < n; ++ i) {
    PgNode* node = pat_gen.node(i);
    display_edge(s, node, 0);
    display_edge(s, node, 1);
  }

  s << " *** PATTERN SECTION ***" << endl;
  vector<bool> vmark;
  ymuint np = pat_gen.pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    vmark.clear();
    vmark.resize(n, false);
    s << "Pat#" <<i << ": ";
    PgHandle root = pat_gen.pat_root(i);
    if ( root.inv() ) {
      s << "[inv]";
    }
    display_dfs(s, root.node(), vmark);
    s << endl;
  }
}


BEGIN_NONAMESPACE

void
dump_word(ostream& s,
	  ymuint val)
{
  static char buf[4];
  buf[0] = val & 255U; val >>= 8;
  buf[1] = val & 255U; val >>= 8;
  buf[2] = val & 255U; val >>= 8;
  buf[3] = val & 255U;

  s.write(buf, 4);
}

void
dump_edge(ostream& s,
	  PgNode* node,
	  ymuint fanin_pos)
{
  ymuint v = 0U;
  if ( !node->is_input() ) {
    v = node->fanin(fanin_pos)->id() * 2;
    if ( node->fanin_inv(fanin_pos) ) {
      v |= 1U;
    }
  }
  dump_word(s, v);
}

void
dump_dfs(ostream& s,
	 PgNode* node,
	 vector<bool>& vmark,
	 vector<ymuint>& val_list,
	 ymuint& max_input)
{
  if ( node->is_input() ) {
    if ( max_input < node->input_id() ) {
      max_input = node->input_id();
    }
    return;
  }
  if ( vmark[node->id()] ) {
    return;
  }
  vmark[node->id()] = true;
  val_list.push_back(node->id() * 2);
  dump_dfs(s, node->fanin(0), vmark, val_list, max_input);
  val_list.push_back(node->id() * 2 + 1);
  dump_dfs(s, node->fanin(1), vmark, val_list, max_input);
}

END_NONAMESPACE

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
// @note ダンプされた情報はそのまま PatGraph で読み込むことができる．
void
pg_dump(ostream& s,
	const PatGen& pat_gen)
{
  ymuint nn = pat_gen.node_num();
  dump_word(s, nn);
  for (ymuint i = 0; i < nn; ++ i) {
    PgNode* node = pat_gen.node(i);
    ymuint v = 0U;
    if ( node->is_input() ) {
      v = PatMgr::kInput | (node->input_id() << 2);
    }
    else if ( node->is_and() ) {
      v = PatMgr::kAnd;
    }
    else if ( node->is_xor() ) {
      v = PatMgr::kXor;
    }
    dump_word(s, v);
    dump_edge(s, node, 0);
    dump_edge(s, node, 1);
  }

  vector<bool> vmark(nn);
  vector<ymuint> val_list;
  val_list.reserve(nn * 2);

  ymuint np = pat_gen.pat_num();
  dump_word(s, np);
  for (ymuint i = 0; i < np; ++ i) {
    PgHandle root = pat_gen.pat_root(i);
    vmark.clear();
    vmark.resize(nn, false);
    val_list.clear();
    ymuint max_input = 0;
    dump_dfs(s, root.node(), vmark, val_list, max_input);
    ymuint v = (max_input + 1) << 1;
    if ( root.inv() ) {
      v |= 1U;
    }
    dump_word(s, v);
    ymuint ne = val_list.size();
    dump_word(s, ne);
    for (ymuint i = 0; i < ne; ++ i) {
      dump_word(s, val_list[i]);
    }
  }
}

END_NAMESPACE_YM_TECHMAP_PATGEN
