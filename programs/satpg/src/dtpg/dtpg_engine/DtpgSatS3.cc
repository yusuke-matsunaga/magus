
/// @file DtpgSatS3.cc
/// @brief DtpgSatS3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatS3.h"

#include "DtpgStats.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "SatEngine.h"
#include "AssignList.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief Single エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatS3(const string& sat_type,
	      const string& sat_option,
	      ostream* sat_outp,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop)
{
  return new DtpgSatS3(sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatS3::DtpgSatS3(const string& sat_type,
		     const string& sat_option,
		     ostream* sat_outp,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop) :
  DtpgSatBaseS(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatS3::~DtpgSatS3()
{
}

// @brief テストパタン生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] flist 故障リスト
void
DtpgSatS3::run_single(TpgNetwork& network,
		      TpgFault* fault)
{
  TpgNode* fnode = fault->node();

  cnf_begin();

  SatEngine engine(sat_type(), sat_option(), sat_outp());

  //////////////////////////////////////////////////////////////////////
  // 変数の割当
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    VarId gvar = engine.new_var();
    node->set_gvar(gvar);
  }
  for (ymuint i = 0; i < tfo_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    VarId fvar = engine.new_var();
    VarId dvar = engine.new_var();
    node->set_fvar(fvar, dvar);
  }

  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_tfi_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);
    make_gval_cnf(engine, node);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_size(); ++ i) {
    TpgNode* node = tfo_tfi_node(i);

    // 故障回路のゲートの入出力関係を表すCNFを作る．
    if ( node == fnode ) {
      make_fault_cnf(engine, fault);
    }
    else {
      make_fval_cnf(engine, node);
    }

    // D-Chain 制約を作る．
    make_dchain_cnf(engine, node);
  }

  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_list().size();
  engine.tmp_lits_begin(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    TpgNode* node = output_list()[i];
    Literal dlit(node->dvar(), false);
    engine.tmp_lits_add(dlit);
  }
  engine.tmp_lits_end();

  cnf_end();

  // 故障に対するテスト生成を行なう．
  engine.assumption_begin();

  // dominator ノードの dvar は1でなければならない．
  for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    Literal dlit(node->dvar(), false);
    engine.assumption_add(dlit);
  }

  cout << fault->str() << endl;

  vector<Bool3> sat_model;
  Bool3 sat_ans = engine.solve(sat_model);
  if ( sat_ans == kB3True ) {
#if 0
    AssignList as_list;
    backtrace(network, sat_model, as_list);

    // 必要割当を求める．
    vector<Bool3> tmp_model;
    ymuint n = as_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      Assign as = as_list.elem(i);
      TpgNode* node = network.node(as.node_id());

      engine.assumption_begin();

      // dominator ノードの dvar は1でなければならない．
      for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
	Literal dlit(node->dvar(), false);
	engine.assumption_add(dlit);
      }
      // node の割当の反対を試す．
      bool inv = as.val();
      Literal alit(node->gvar(), inv);
      engine.assumption_add(alit);

      Bool3 tmp_stat = engine.solve(tmp_model);
      if ( tmp_stat == kB3True ) {
	// 反対でも検出できたので必要割当ではない．
	;
      }
      else if ( tmp_stat == kB3False ) {
	cout << "   Node#" << node->id() << " is a mandatory assignment" << endl;
      }
      else {
	// アボート．とりあえず無視
	;
      }
    }
#endif
  }

  clear_node_mark();
}

END_NAMESPACE_YM_SATPG
