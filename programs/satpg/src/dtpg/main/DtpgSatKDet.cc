
/// @file DtpgSatKDet.cc
/// @brief DtpgSatKDet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatKDet.h"

#include "DtpgStats.h"
#include "NodeSet.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "TestVector.h"
#include "SatEngine.h"
#include "GenVidMap.h"
#include "ModelValMap.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief SingleKDet エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
// @param[in] kdet 多重度
DtpgEngine*
new_DtpgSatKDet(const string& sat_type,
		const string& sat_option,
		ostream* sat_outp,
		BackTracer& bt,
		DetectOp& dop,
		UntestOp& uop,
		ymuint kdet)
{
  return new DtpgSatKDet(sat_type, sat_option, sat_outp, bt, dop, uop, kdet);
}

// @brief コンストラクタ
DtpgSatKDet::DtpgSatKDet(const string& sat_type,
			 const string& sat_option,
			 ostream* sat_outp,
			 BackTracer& bt,
			 DetectOp& dop,
			 UntestOp& uop,
			 ymuint kdet) :
  DtpgSatBaseS(sat_type, sat_option, sat_outp, bt, dop, uop),
  mCount(kdet)
{
}

// @brief デストラクタ
DtpgSatKDet::~DtpgSatKDet()
{
}

// @brief テストパタン生成を行なう．
// @param[in] node_set 対象のノード集合
// @param[in] flist 故障リスト
void
DtpgSatKDet::run_single(const NodeSet& node_set,
			TpgFault* fault)
{
  const TpgNode* fnode = fault->node();

  cnf_begin();

  SatEngine engine(sat_type(), sat_option(), sat_outp());

  ymuint max_id = node_set.max_id();

  GenVidMap gvar_map(max_id);
  GenVidMap fvar_map(max_id);
  GenVidMap dvar_map(max_id);

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      engine.make_fault_cnf(fault, gvar_map, fvar_map);
    }
    else {
      engine.make_node_cnf(node, fvar_map);
    }

    // D-Chain 制約を作る．
    engine.make_dchain_cnf(node, gvar_map, fvar_map, dvar_map);
  }


  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = node_set.output_list().size();
  engine.tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    const TpgNode* node = node_set.output_list()[i];
    Literal dlit(dvar_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  cnf_end();

  for (ymuint i = 0; i < mCount; ++ i) {
    // 故障に対するテスト生成を行なう．
    engine.assumption_begin();

    // dominator ノードの dvar は1でなければならない．
    for (const TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(dvar_map(node), false);
    engine.assumption_add(dlit);
    }

    vector<Bool3> sat_model;
    SatStats sat_stats;
    USTime time;
    Bool3 ans = engine.solve(sat_model, sat_stats, time);

    if ( ans == kB3True ) {
      // パタンが求まった．
      ModelValMap val_map(gvar_map, fvar_map, sat_model);
      detect_op(fault, node_set, val_map, sat_stats, time);
    }
    else {
      if ( i == 0 ) {
	if ( ans == kB3False ) {
	  // 検出不能と判定された．
	  untest_op(fault, sat_stats, time);
	}
	else { // ans == kB3X つまりアボート
	  abort_op(fault, sat_stats, time);
	}
      }
      else {
	// 二回目以降は無視
	break;
      }
    }

    ymuint n = last_assign().size();
    engine.tmp_lits_begin(n);
    for (ymuint i = 0; i < n; ++ i) {
      NodeVal nv = last_assign()[i];
      const TpgNode* node = nv.node();
      Literal lit(gvar_map(node));
      if ( nv.val() ) {
	engine.tmp_lits_add(~lit);
      }
      else {
	engine.tmp_lits_add(lit);
      }
    }
    engine.tmp_lits_end();
  }
}

END_NAMESPACE_YM_SATPG
