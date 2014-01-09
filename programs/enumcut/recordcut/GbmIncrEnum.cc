
/// @file GbmIncrEnum.cc
/// @brief GbmIncrEnum の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmIncrEnum.h"
#include "GbmEngine.h"
#include "ym_logic/SatStats.h"
#include "ym_logic/SatMsgHandler.h"
#include "ym_utils/PermGen.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GbmIncrEnum
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmIncrEnum::GbmIncrEnum()
{
}

// @brief デストラクタ
GbmIncrEnum::~GbmIncrEnum()
{
}

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
// @note iorder[0] に func の0番めの入力に対応した RcfNetwork の入力番号が入る．
bool
GbmIncrEnum::_solve(const RcfNetwork& network,
		    const TvFunc& func,
		    vector<bool>& conf_bits,
		    vector<ymuint>& iorder)
{
  ymuint ni = network.input_num();
  for (PermGen pg(ni, ni); !pg.is_end(); ++ pg) {
    vector<ymuint> tmp_order(ni);
    bool skip = false;
    for (ymuint i = 0; i < ni; ++ i) {
      tmp_order[i] = pg(i);
      ymuint cur = tmp_order[i];
      ymuint pred;
      if ( network.get_pred(i, pred) ) {
	if ( tmp_order[pred] > cur ) {
	  skip = true;
	  break;
	}
      }
    }
    if ( skip ) {
      continue;
    }
    bool stat = _solve_with_order(network, func, tmp_order, conf_bits);
    if ( stat ) {
      iorder.resize(ni, 0);
      for (ymuint i = 0; i < ni; ++ i) {
	iorder[tmp_order[i]] = i;
      }
      return true;
    }
  }
  return false;
}

// @brief 問題を解く
// @param[in] network RcfNetwork
// @param[in] output Reconfigurable Network の出力
// @param[in] func マッチング対象の関数
// @param[in] iorder 入力順序
// @param[out] conf_bits configuration ビットの値を収める配列
bool
GbmIncrEnum::_solve_with_order(const RcfNetwork& network,
			       const TvFunc& func,
			       const vector<ymuint>& iorder,
			       vector<bool>& conf_bits)
{
  SatSolver solver("minisat");
  ymuint nc = network.conf_var_num();
  ymuint nn = network.node_num();

  GbmEngine engine(solver, nn, nc);

  // configuration 変数を作る．
  vector<VarId> conf_vid_array(nc);
  vector<GbmLit> conf_var_array(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = solver.new_var();
    conf_vid_array[i] = vid;
    engine.set_conf_var(i, GbmLit(vid));
  }
  conf_bits.resize(nc, false);

  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  // 外部入力変数に値を割り当てたときの CNF 式を作る．
  ymuint fn = network.fnode_num();
  vector<const RcfNode*> node_list(fn);
  for (ymuint i = 0; i < fn; ++ i) {
    const RcfNode* node = network.fnode(i);
    node_list[i] = node;
  }

  ymuint ni = network.input_num();
  ymuint ni_exp = 1U << ni;
  Bool3 stat = kB3X;
  vector<Bool3> model;
  for (ymuint b = 0U; b < ni_exp; ++ b) {
    // 入力に定数を割り当てる．
    for (ymuint i = 0; i < ni; ++ i) {
      const RcfNode* node = network.input_node(i);
      ymuint id = node->id();
      ymuint src_pos = iorder[i];
      if ( b & (1U << src_pos) ) {
	engine.set_node_var(id, GbmLit::make_one());
      }
      else {
	engine.set_node_var(id, GbmLit::make_zero());
      }
    }
    ymuint oval = static_cast<bool>(func.value(b)) ^ oinv;
    bool ok = engine.make_nodes_cnf(node_list, oid, oval);
    if ( !ok ) {
      break;
    }
    stat = solver.solve(model);
    if ( stat == kB3False ) {
      break;
    }
  }

  if ( stat == kB3True ) {
    for (ymuint i = 0; i < nc; ++ i) {
      VarId vid = conf_vid_array[i];
      if ( model[vid.val()] == kB3True ) {
	conf_bits[i] = true;
      }
      else {
	conf_bits[i] = false;
      }
    }
    return true;
  }

  return false;
}

END_NAMESPACE_YM
