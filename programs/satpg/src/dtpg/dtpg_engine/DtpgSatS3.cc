
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
//#include "TestVector.h"
#include "SatEngine.h"
#include "AssignList.h"
#include "BtJust3.h"


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
  }

  clear_node_mark();
}

BEGIN_NONAMESPACE

// @brief ノードの正常値を読み出す．
// @param[in] node 対象のノード
// @param[in] model SAT の割り当て結果
inline
Bool3
node_gval(TpgNode* node,
	  const vector<Bool3>& model)
{
  return read_value(node->gvar(), model);
}

// @brief ノードの故障値を読み出す．
// @param[in] node 対象のノード
// @param[in] model SAT の割り当て結果
inline
Bool3
node_fval(TpgNode* node,
	  const vector<Bool3>& model)
{
  return read_value(node->fvar(), model);
}

// @brief ノードの故障差を読み出す．
// @param[in] node 対象のノード
// @param[in] model SAT の割り当て結果
inline
Bool3
node_dval(TpgNode* node,
	  const vector<Bool3>& model)
{
  return read_value(node->dvar(), model);
}

// @brief 変数番号に対応する値を読み出す．
// @param[in] vid 変数番号
// @param[in] model SAT の割り当て結果
inline
Bool3
read_value(VarId vid,
	   const vector<Bool3>& model)
{
  return model[vid.val()];
}

END_NONAMESPACE

// @brief バックトレースを行う．
void
DtpgSatS3::backtrace(TpgNetwork& network,
		     const vector<Bool3>& model,
		     AssignList& as_list)
{
  // 故障差の伝搬している外部出力を選ぶ．
  ymuint nmin = 0;
  AssignList best_list;
  for (vector<TpgNode*>::const_iterator p = output_list().begin();
       p != output_list().end(); ++ p) {
    TpgNode* node = *p;
    if ( node_dval(node, model) == kB3True ) {
      // 正当化を行う．
      AssignList as_list1;
      justify(node, model, as_list1);
      ymuint n = as_list1.size();
      if ( nmin == 0 || nmin > n ) {
	nmin = n;
	best_list = as_list1;
      }
    }
  }
  ASSERT_COND( nmin > 0 );
  as_list = best_list;
}

END_NAMESPACE_YM_SATPG
