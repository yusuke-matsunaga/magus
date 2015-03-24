
/// @file DtpgSatKDet.cc
/// @brief DtpgSatKDet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatKDet.h"

#include "DtpgStats.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "TestVector.h"
#include "SatEngine.h"


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
// @param[in] network 対象のネットワーク
// @param[in] flist 故障リスト
void
DtpgSatKDet::run_single(TpgNetwork& network,
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

  for (ymuint i = 0; i < mCount; ++ i) {
    // 故障に対するテスト生成を行なう．
    engine.assumption_begin();

    // dominator ノードの dvar は1でなければならない．
    for (TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(node->dvar(), false);
    engine.assumption_add(dlit);
    }

    SatStats sat_stats;
    USTime time;
    Bool3 ans = solve(engine, sat_stats, time);

    if ( ans == kB3True ) {
      // パタンが求まった．
      detect_op(fault, sat_stats, time);
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

    TestVector* pat = last_pat();
    ymuint ni = pat->input_num();
    engine.tmp_lits_begin(ni - pat->x_num());
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* node = network.input(i);
      Literal lit(node->gvar());
      switch ( pat->val3(i) ) {
      case kVal1:
	engine.tmp_lits_add(~lit);
	break;

      case kVal0:
	engine.tmp_lits_add(lit);
	break;

      default:
	break;
      }
    }
    engine.tmp_lits_end();
  }

  clear_node_mark();
}

END_NAMESPACE_YM_SATPG
