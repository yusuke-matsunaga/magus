
/// @file DtpgSatS.cc
/// @brief DtpgSatS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatS.h"
#include "DtpgStats.h"
#include "GvalCnf.h"
#include "FvalCnf.h"
#include "NodeSet.h"
#include "SatEngine.h"
#include "TpgFault.h"
#include "TpgNetwork.h"


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
  DtpgSat(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatS::~DtpgSatS()
{
}

// @brief テスト生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] stats 結果を格納する構造体
void
DtpgSatS::run(TpgNetwork& network,
	      DtpgStats& stats)
{
  clear_stats();

  ymuint nn = network.active_node_num();
  ymuint max_id = network.max_node_id();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    NodeSet node_set;
    node_set.mark_region(max_id, node);

    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      TpgFault* fault = node->fault(i);
      if ( fault->status() == kFsDetected ) {
	continue;
      }

      cnf_begin();

      GvalCnf gval_cnf(max_id);
      FvalCnf fval_cnf(max_id, gval_cnf);
      SatEngine engine(sat_type(), sat_option(), sat_outp());

      engine.make_fval_cnf(fval_cnf, fault, node_set, kVal1);

      cnf_end();

      // 故障に対するテスト生成を行なう．
      solve(engine, vector<Literal>(), fault, node_set, fval_cnf.gvar_map(), fval_cnf.fvar_map());
    }
  }

  get_stats(stats);
}

END_NAMESPACE_YM_SATPG
