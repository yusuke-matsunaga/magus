
/// @file DtpgSatS.cc
/// @brief DtpgSatS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatS.h"

#include "DtpgStats.h"
#include "NodeSet.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "TestVector.h"
#include "SatEngine.h"
#include "GenVidMap.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief Single エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatS(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop)
{
  return new DtpgSatS(sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatS::DtpgSatS(const string& sat_type,
		   const string& sat_option,
		   ostream* sat_outp,
		   BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop) :
  DtpgSatBaseS(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatS::~DtpgSatS()
{
}

// @brief テストパタン生成を行なう．
// @param[in] node_set 対象のノード集合
// @param[in] flist 故障リスト
void
DtpgSatS::run_single(const NodeSet& node_set,
		     TpgFault* fault)
{
  TpgNode* fnode = fault->node();

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
    TpgNode* node = node_set.tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    gvar_map.set_vid(node, gvar);
    fvar_map.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);
    engine.make_node_cnf(node, gvar_map);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < node_set.tfo_size(); ++ i) {
    TpgNode* node = node_set.tfo_tfi_node(i);

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
    TpgNode* node = node_set.output_list()[i];
    Literal dlit(dvar_map(node), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  cnf_end();

  // 故障に対するテスト生成を行なう．
  engine.assumption_begin();

  // dominator ノードの dvar は1でなければならない．
  for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    Literal dlit(dvar_map(node), false);
    engine.assumption_add(dlit);
  }

  solve(engine, fault, node_set, gvar_map, fvar_map);
}

END_NAMESPACE_YM_SATPG
