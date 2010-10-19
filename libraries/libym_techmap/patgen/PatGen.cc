
/// @file libym_techmap/patgen/PatGen.cc
/// @brief PatGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatGen.h"
#include "ym_lexp/LogExpr.h"
#include "ym_utils/Generator.h"


BEGIN_NONAMESPACE

const ymuint n_pat3 = 2;
int pat3[n_pat3][5] = {
	{ -1, 0, -1, 1, 2 },
	{ -1, -1, 0, 1, 2 }
};

const ymuint n_pat4 = 5;
int pat4[n_pat4][7] = {
	{ -1, 0, -1, 1, -1, 2, 3 },
	{ -1, 0, -1, -1, 1, 2, 3 },
	{ -1, -1, 0, 1, -1, 2, 3 },
	{ -1, -1, 0, -1, 1, 2, 3 },
	{ -1, -1, -1, 0, 1, 2, 3 }
};

const ymuint n_pat5 = 14;
int pat5[n_pat5][9] = {
	{ -1, 0, -1, 1, -1, 2, -1, 3, 4 },
	{ -1, 0, -1, 1, -1, -1, 2, 3, 4 },
	{ -1, 0, -1, -1, 1, 2, -1, 3, 4 },
	{ -1, 0, -1, -1, 1, -1, 2, 3, 4 },
	{ -1, 0, -1, -1, -1, 1, 2, 3, 4 },
	{ -1, -1, 0, 1, -1, 2, -1, 3, 4 },
	{ -1, -1, 0, 1, -1, -1, 2, 3, 4 },
	{ -1, -1, 0, -1, 1, 2, -1, 3, 4 },
	{ -1, -1, -1, 0, 1, 2, -1, 3, 4 },
	{ -1, -1, 0, -1, 1, -1, 2, 3, 4 },
	{ -1, -1, 0, -1, -1, 1, 2, 3, 4 },
	{ -1, -1, -1, 0, 1, -1, 2, 3, 4 },
	{ -1, -1, -1, 0, -1, 1, 2, 3, 4 },
	{ -1, -1, -1, -1, 0, 1, 2, 3, 4 }
};

const ymuint n_pat6 = 42;
int pat6[n_pat6][11] = {
	{ -1, 0, -1, 1, -1, 2, -1, 3, -1, 4, 5 },
	{ -1, 0, -1, 1, -1, 2, -1, -1, 3, 4, 5 },
	{ -1, 0, -1, 1, -1, -1, 2, 3, -1, 4, 5 },
	{ -1, 0, -1, 1, -1, -1, 2, -1, 3, 4, 5 },
	{ -1, 0, -1, 1, -1, -1, -1, 2, 3, 4, 5 },
	{ -1, 0, -1, -1, 1, 2, -1, 3, -1, 4, 5 },
	{ -1, 0, -1, -1, 1, 2, -1, -1, 3, 4, 5 },
	{ -1, 0, -1, -1, 1, -1, 2, 3, -1, 4, 5 },
	{ -1, 0, -1, -1, -1, 1, 2, 3, -1, 4, 5 },
	{ -1, 0, -1, -1, 1, -1, 2, -1, 3, 4, 5 },
	{ -1, 0, -1, -1, 1, -1, -1, 2, 3, 4, 5 },
	{ -1, 0, -1, -1, -1, 1, 2, -1, 3, 4, 5 },
	{ -1, 0, -1, -1, -1, 1, -1, 2, 3, 4, 5 },
	{ -1, 0, -1, -1, -1, -1, 1, 2, 3, 4, 5 },
	{ -1, -1, 0, 1, -1, 2, -1, 3, -1, 4, 5 },
	{ -1, -1, 0, 1, -1, 2, -1, -1, 3, 4, 5 },
	{ -1, -1, 0, 1, -1, -1, 2, 3, -1, 4, 5 },
	{ -1, -1, 0, 1, -1, -1, 2, -1, 3, 4, 5 },
	{ -1, -1, 0, 1, -1, -1, -1, 2, 3, 4, 5 },
	{ -1, -1, 0, -1, 1, 2, -1, 3, -1, 4, 5 },
	{ -1, -1, 0, -1, 1, 2, -1, -1, 3, 4, 5 },
	{ -1, -1, -1, 0, 1, 2, -1, 3, -1, 4, 5 },
	{ -1, -1, -1, 0, 1, 2, -1, -1, 3, 4, 5 },
	{ -1, -1, 0, -1, 1, -1, 2, 3, -1, 4, 5 },
	{ -1, -1, 0, -1, -1, 1, 2, 3, -1, 4, 5 },
	{ -1, -1, -1, 0, 1, -1, 2, 3, -1, 4, 5 },
	{ -1, -1, -1, 0, -1, 1, 2, 3, -1, 4, 5 },
	{ -1, -1, -1, -1, 0, 1, 2, 3, -1, 4, 5 },
	{ -1, -1, 0, -1, 1, -1, 2, -1, 3, 4, 5 },
	{ -1, -1, 0, -1, 1, -1, -1, 2, 3, 4, 5 },
	{ -1, -1, 0, -1, -1, 1, 2, -1, 3, 4, 5 },
	{ -1, -1, 0, -1, -1, 1, -1, 2, 3, 4, 5 },
	{ -1, -1, 0, -1, -1, -1, 1, 2, 3, 4, 5 },
	{ -1, -1, -1, 0, 1, -1, 2, -1, 3, 4, 5 },
	{ -1, -1, -1, 0, 1, -1, -1, 2, 3, 4, 5 },
	{ -1, -1, -1, 0, -1, 1, 2, -1, 3, 4, 5 },
	{ -1, -1, -1, -1, 0, 1, 2, -1, 3, 4, 5 },
	{ -1, -1, -1, 0, -1, 1, -1, 2, 3, 4, 5 },
	{ -1, -1, -1, 0, -1, -1, 1, 2, 3, 4, 5 },
	{ -1, -1, -1, -1, 0, 1, -1, 2, 3, 4, 5 },
	{ -1, -1, -1, -1, 0, -1, 1, 2, 3, 4, 5 },
	{ -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5 }
};

END_NONAMESPACE

BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

// @brief コンストラクタ
PatGen::PatGen()
{
}

// @brief デストラクタ
PatGen::~PatGen()
{
}

// @brief 論理式から対応するパタングラフを生成する．
void
PatGen::operator()(const LogExpr& expr,
		   PgNodeMgr& mgr,
		   vector<PgHandle>& pg_list)
{
  assert_cond( !expr.is_constant(), __FILE__, __LINE__);
}

// @brief パタングラフを生成する再帰関数
void
PatGen::gen_pat(const LogExpr& expr,
		PgNodeMgr& mgr,
		vector<PgHandle>& pg_list)
{
  if ( expr.is_literal() ) {
    PgNode* node = mgr.new_input(expr.varid());
    bool inv = expr.is_negaliteral();
    pg_list.push_back(PgHandle(node, inv));
  }
  else {
    ymuint n = expr.child_num();
    vector<vector<PgHandle> > input_pg_list(n);
    vector<pair<size_t, size_t> > nk_array(n);
    for (ymuint i = 0; i < n; ++ i) {
      gen_pat(expr.child(i), mgr, input_pg_list[i]);
      nk_array[i] = make_pair(input_pg_list[i].size(), 1);
    }
    MultiCombiGen mcg(nk_array);
    for (MultiCombiGen::iterator p = mcg.begin(); !p.is_end(); ++ p) {
      vector<PgHandle> input(n);
      for (ymuint i = 0; i < n; ++ i) {
	input[i] = input_pg_list[i][p(i, 0)];
      }
      switch ( n ) {
      case 3:
	for (ymuint i = 0; i < n_pat3; ++ i) {
	  ymuint pos = 0;
	  PgHandle handle = make_graph(mgr, input, pat3[i], pos);
	  pg_list.push_back(handle);
	}
	break;

      case 4:
	for (ymuint i = 0; i < n_pat4; ++ i) {
	  ymuint pos = 0;
	  PgHandle handle = make_graph(mgr, input, pat4[i], pos);
	  pg_list.push_back(handle);
	}
	break;

      case 5:
	for (ymuint i = 0; i < n_pat5; ++ i) {
	  ymuint pos = 0;
	  PgHandle handle = make_graph(mgr, input, pat5[i], pos);
	  pg_list.push_back(handle);
	}
	break;

      case 6:
	for (ymuint i = 0; i < n_pat6; ++ i) {
	  ymuint pos = 0;
	  PgHandle handle = make_graph(mgr, input, pat6[i], pos);
	  pg_list.push_back(handle);
	}
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
    }
  }
}

// @brief テンプレートにしたがって2分木を作る．
PgHandle
PatGen::make_graph(PgNodeMgr& mgr,
		   const vector<PgHandle>& input,
		   int pat[],
		   ymuint& pos)
{
  int p = pat[pos];
  ++ pos;
  if ( p == -1 ) {
    // 演算ノード
    PgHandle l_handle = make_graph(mgr, input, pat, pos);
    PgHandle r_handle = make_graph(mgr, input, pat, pos);
    PgNode* l_node = l_handle.node();
    PgNode* r_node = r_handle.node();
    bool l_inv = l_handle.inv();
    bool r_inv = r_handle.inv();
    PgNode* node = mgr.new_and(l_node, r_node, l_inv, r_inv);
    return PgHandle(node, false);
  }
  else {
    // 終端ノード
    ymuint id = static_cast<ymuint>(p);
    return input[id];
  }
}

END_NAMESPACE_YM_TECHMAP_PATGEN
