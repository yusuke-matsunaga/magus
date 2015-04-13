
/// @file MinPatDsatur.cc
/// @brief MinPatDsatur の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatDsatur.h"
#include "TpgNetwork.h"
#include "TpgFault.h"
#include "FaultMgr.h"
#include "FaultAnalyzer.h"
#include "FgMgr.h"
#include "TpgCnf1.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
MinPat*
new_MinPatDsatur()
{
  return new MinPatDsatur();
}

//////////////////////////////////////////////////////////////////////
// クラス MinPatDsatur
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatDsatur::MinPatDsatur()
{
}

// @brief デストラクタ
MinPatDsatur::~MinPatDsatur()
{
}

// @brief 初期化を行う．
// @param[in] network 対象のネットワーク
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
// @param[in] tv_list テストベクタのリスト
// @return 支配故障数を返す．
ymuint
MinPatDsatur::init(TpgNetwork& network,
		   TvMgr& tvmgr,
		   FaultMgr& fmgr,
		   Fsim& fsim2,
		   vector<TestVector*>& tv_list)
{
  mMaxNodeId = network.max_node_id();

  FaultAnalyzer& analyzer = mAnalyzer;

  bool verbose = false;
  analyzer.set_verbose(verbose);

#if 0
  const vector<TpgFault*>& fault_list = fmgr.det_list();
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

  analyzer.init(mMaxNodeId, fault_list);

  ymuint npat0 = 10000;
  RandGen rg;
  analyzer.get_pat_list(fsim2, tvmgr, tv_list, rg, npat0);

  bool dom_fast = false;
  analyzer.get_dom_faults(dom_fast);

  const vector<TpgFault*>& src_list = analyzer.dom_fault_list();
  ymuint nf = src_list.size();
  mFaultStructList.clear();
  mFaultStructList.resize(nf);
  mMaxFaultId = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* fault = src_list[i];
    if ( mMaxFaultId < fault->id() ) {
      mMaxFaultId = fault->id();
    }
    FaultStruct& fs = mFaultStructList[i];
    fs.mFault = fault;
    fs.mSelected = false;
    fs.mConflictNum = 0;
    fs.mConflictMap.resize(1, false);
    fs.mPendingNum = 0;
    fs.mPendingMap.resize(1, false);
  }
  ++ mMaxFaultId;
  mRemainNum = nf;

  return nf;
}

// @brief 最初の故障を選ぶ．
TpgFault*
MinPatDsatur::get_first_fault()
{
  ASSERT_COND( mRemainNum > 0 );

  // 最初は同時検出故障数の少ない故障を選ぶ．
  TpgFault* min_fault = NULL;
  ymuint min_count = 0;
  ymuint min_pos = 0;
  ymuint fault_num = mFaultStructList.size();
  for (ymuint i = 0; i < fault_num; ++ i) {
    TpgFault* fault = mFaultStructList[i].mFault;
    ymuint fnum = mAnalyzer.fault_info(fault->id()).fnum();
    if ( min_fault == NULL || min_count > fnum ) {
      min_count = fnum;
      min_fault = fault;
      min_pos = i;
    }
  }
  mPrevGid = 0;
  mFaultStructList[min_pos].mSelected = true;
  -- mRemainNum;
  return min_fault;
}

// @brief 次に処理すべき故障を選ぶ．
// @param[in] fgmgr 故障グループを管理するオブジェクト
//
// 故障が残っていなければ NULL を返す．
TpgFault*
MinPatDsatur::get_next_fault(FgMgr& fgmgr)
{
  if ( mRemainNum == 0 ) {
    return NULL;
  }

  // 飽和度最大の故障を選ぶ．
  ymuint ng = fgmgr.group_num();
  ymuint fault_num = mFaultStructList.size();

  ymuint max_pos = 0;

  bool first = true;
  ymuint max_satur = 0;
  for (ymuint i = 0; i < fault_num; ++ i) {
    FaultStruct& fs = mFaultStructList[i];
    if ( fs.mSelected ) {
      continue;
    }
    if ( !fs.mConflictMap[mPrevGid] && !fs.mPendingMap[mPrevGid] ) {
      fs.mPendingMap[mPrevGid] = true;
      ++ fs.mPendingNum;
    }
    if ( first || max_satur < fs.mConflictNum ) {
      first = false;
      max_satur = fs.mConflictNum;
      max_pos = i;
    }
  }

  for ( ; ; ) {
    ymuint max2 = max_satur;
    ymuint max2_pos = 0;
    for (ymuint i = 0; i < fault_num; ++ i) {
      FaultStruct& fs = mFaultStructList[i];
      if ( fs.mSelected ) {
	continue;
      }
      if ( max2 < fs.mConflictNum + fs.mPendingNum ) {
	max2 = fs.mConflictNum + fs.mPendingNum;
	max2_pos = i;
      }
    }
    if ( max2 > max_satur ) {
      ymuint slack = max2 - max_satur;
      FaultStruct& fs = mFaultStructList[max2_pos];
      TpgCnf1 tpg_cnf(string(), string(), NULL);
      tpg_cnf.make_fval_cnf(fs.mFault, mMaxNodeId);
      for (ymuint gid = 0; gid < ng; ++ gid) {
	if ( fs.mPendingMap[gid] ) {
	  fs.mPendingMap[gid] = false;
	  -- fs.mPendingNum;
	  const NodeValList& suf_list0 = fgmgr.suf_list(gid);
	  if ( !tpg_cnf.check_intersect(suf_list0) ) {
	    ++ fs.mConflictNum;
	    fs.mConflictMap[gid] = true;
	  }
	  else {
	    -- slack;
	    if ( slack == 0 ) {
	      break;
	    }
	  }
	}
      }
      if ( slack > 0 ) {
	max_satur = fs.mConflictNum;
	max_pos = max2_pos;
      }
    }
    else {
      break;
    }
  }

  mFaultStructList[max_pos].mSelected = true;
  -- mRemainNum;
  return mFaultStructList[max_pos].mFault;
}

// @brief 故障を追加するグループを選ぶ．
// @param[in] fgmgr 故障グループを管理するオブジェクト
// @param[in] fault 故障
//
// グループが見つからなければ fgmgr.group_num() を返す．
ymuint
MinPatDsatur::find_group(FgMgr& fgmgr,
			 TpgFault* fault)
{
  TpgCnf1 tpg_cnf(string(), string(), NULL);
  tpg_cnf.make_fval_cnf(fault, mMaxNodeId);

  ymuint ng = fgmgr.group_num();
  for (ymuint gid = 0; gid < ng; ++ gid) {
    const NodeValList& suf_list0 = fgmgr.suf_list(gid);
    if ( tpg_cnf.check_intersect(suf_list0) ) {
      mPrevGid = gid;
      return gid;
    }
  }
  for (ymuint i = 0; i < mFaultStructList.size(); ++ i) {
    FaultStruct& fs = mFaultStructList[i];
    if ( fs.mSelected ) {
      continue;
    }
    fs.mConflictMap.resize(ng + 1, false);
    fs.mPendingMap.resize(ng + 1, false);
  }
  mPrevGid = ng;
  return ng;
}

END_NAMESPACE_YM_SATPG
