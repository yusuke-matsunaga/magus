
/// @file GbmNaiveEnum.cc
/// @brief GbmNaiveEnum の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNaiveEnum.h"
#include "GbmEngine.h"
#include "ym_logic/SatStats.h"
#include "ym_logic/SatMsgHandler.h"
#include "ym_utils/PermGen.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GbmNaiveEnum
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmNaiveEnum::GbmNaiveEnum()
{
}

// @brief デストラクタ
GbmNaiveEnum::~GbmNaiveEnum()
{
}

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
// @note iorder[0] に func の0番めの入力に対応した RcfNetwork の入力番号が入る．
bool
GbmNaiveEnum::_solve(const RcfNetwork& network,
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
GbmNaiveEnum::_solve_with_order(const RcfNetwork& network,
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
  bool conflict = false;
  for (ymuint b = 0U; b < ni_exp && !conflict; ++ b) {
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
    // 内部のノードに変数番号を割り当てる．
    for (vector<const RcfNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const RcfNode* node = *p;
      if ( node->id() == oid ) {
	// 外部出力の時は関数の値に応じた定数となる．
	if ( static_cast<bool>(func.value(b)) ^ oinv ) {
	  engine.set_node_var(oid, GbmLit::make_one());
	}
	else {
	  engine.set_node_var(oid, GbmLit::make_zero());
	}
      }
      else {
	VarId vid = solver.new_var();
	engine.set_node_var(node->id(), GbmLit(vid));
      }
    }
    // 内部のノードに対する CNF 式を作る．
    for (vector<const RcfNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const RcfNode* node = *p;
      bool stat = engine.make_node_cnf(node);
      if ( !stat ) {
	// 矛盾が起こった．
	conflict = true;
	break;
      }
    }
    if ( conflict ) {
      break;
    }
  }

  stat = solver.solve(model);

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
