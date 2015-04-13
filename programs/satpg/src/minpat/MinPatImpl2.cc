
/// @file MinPatImpl2.cc
/// @brief MinPatImpl2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatImpl2.h"
#include "MinPatStats.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "FaultMgr.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "FaultAnalyzer.h"
#include "Verifier.h"
#include "GcSolver2.h"

#include "TpgCnf0.h"

#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
MinPat*
new_MinPat2()
{
  return new MinPatImpl2();
}


//////////////////////////////////////////////////////////////////////
// クラス MinPatImpl2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatImpl2::MinPatImpl2()
{
}

// @brief デストラクタ
MinPatImpl2::~MinPatImpl2()
{
}

// @brief テストベクタの最小化を行なう．
// @param[in] network 対象のネットワーク
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
// @param[out] tv_list テストベクタのリスト
// @param[out] stats 実行結果の情報を格納する変数
void
MinPatImpl2::run(TpgNetwork& network,
		 TvMgr& tvmgr,
		 FaultMgr& fmgr,
		 Fsim& fsim2,
		 vector<TestVector*>& tv_list,
		 MinPatStats& stats)
{
  StopWatch total_timer;
  StopWatch local_timer;

  total_timer.start();
  local_timer.start();

  ymuint orig_num = tv_list.size();

  Verifier ver(fmgr, fsim2);

#if 0
  const vector<TpgFault>& fault_list = fmgr.det_list();
#else
  vector<TpgFault*> f_list2;
  for (ymuint i = 0; i < network.active_node_num(); ++ i) {
    const TpgNode* node = network.active_node(i);
    ymuint ni = node->fanin_num();
    bool has_ncfault = false;
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* f0 = node->input_fault(0, j);
      if ( f0 != NULL ) {
	if ( f0->is_rep() && f0->status() == kFsDetected ) {
	  f_list2.push_back(f0);
	}
	if ( node->nval() == kVal0 && f0->rep_fault()->status() == kFsDetected ) {
	  has_ncfault = true;
	}
      }
      TpgFault* f1 = node->input_fault(1, j);
      if ( f1 != NULL ) {
	if ( f1->is_rep() && f1->status() == kFsDetected ) {
	  f_list2.push_back(f1);
	}
	if ( node->nval() == kVal1 && f1->rep_fault()->status() == kFsDetected ) {
	  has_ncfault = true;
	}
      }
    }
    TpgFault* f0 = node->output_fault(0);
    if ( f0 != NULL && f0->is_rep() && f0->status() == kFsDetected ) {
      if ( node->noval() != kVal0 || !has_ncfault ) {
	f_list2.push_back(f0);
      }
    }
    TpgFault* f1 = node->output_fault(1);
    if ( f1 != NULL && f1->is_rep() && f1->status() == kFsDetected ) {
      if ( node->noval() != kVal1 || !has_ncfault ) {
	f_list2.push_back(f1);
      }
    }
  }
  const vector<TpgFault*>& fault_list = f_list2;
#endif

  bool verbose = true;
  FaultAnalyzer analyzer(verbose);

  analyzer.init(network.max_node_id(), fault_list);

  ymuint npat0 = 10000;
  RandGen rg;
  analyzer.get_pat_list(fsim2, tvmgr, tv_list, rg, npat0);

  analyzer.get_dom_faults();

  analyzer.analyze_faults();

  analyzer.analyze_conflict();

  local_timer.reset();
  local_timer.start();
  cout << "coloring start" << endl;
  GcSolver2 gcsolver;
  const vector<TpgFault*>& dom_fault_list = analyzer.dom_fault_list();
  const vector<FaultInfo>& fault_info_array = analyzer.fault_info_array();
  const vector<vector<ymuint> >& input_list_array = analyzer.input_list_array();
  ymuint nc = gcsolver.coloring(dom_fault_list, fault_info_array, input_list_array, network.max_node_id());
  cout << " # of fault groups = " << nc << endl;
  local_timer.stop();
  cout << "CPU time (coloring)          " << local_timer.time() << endl;

  // テストパタンを作る．
  TpgCnf0 tpg_cnf0(string(), string(), NULL);
  vector<TestVector*> new_tv_list;
  new_tv_list.reserve(nc);
  for (ymuint col = 1; col <= nc; ++ col) {
    const NodeValList& suf_list = gcsolver.suf_list(col);
    TestVector* tv = tvmgr.new_vector();
    bool stat = tpg_cnf0.get_testvector(network, suf_list, tv);
    ASSERT_COND( stat );
    new_tv_list.push_back(tv);
    if ( false ) {
      cout << "#" << col << endl
	   << " faults: ";
      const vector<TpgFault*>& fault_list = gcsolver.fault_list(col);
      for (ymuint i = 0; i < fault_list.size(); ++ i) {
	TpgFault* f = fault_list[i];
	cout << " " << f->str();
      }
      cout << endl;
      cout << tv << endl;
    }
  }
  tv_list = new_tv_list;
  {
    // 検証しておく．
    if ( ver.check(tv_list) ) {
      cout << "No Errors" << endl;
    }
  }

  total_timer.stop();
  USTime time = total_timer.time();

  stats.set(orig_num, tv_list.size(), time);
}

END_NAMESPACE_YM_SATPG
