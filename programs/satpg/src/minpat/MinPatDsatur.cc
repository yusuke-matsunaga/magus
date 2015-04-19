
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
// @param[out] fault_list 検出された故障のリスト
void
MinPatDsatur::init(TpgNetwork& network,
		   TvMgr& tvmgr,
		   Fsim& fsim2,
		   vector<TpgFault*>& fault_list)
{
  mMaxNodeId = network.max_node_id();

  FaultAnalyzer& analyzer = mAnalyzer;

  analyzer.set_verbose(verbose());

  analyzer.init(network, tvmgr);

  fault_list = analyzer.fault_list();

  RandGen rg;
  analyzer.get_pat_list(fsim2, tvmgr, rg);

  analyzer.get_dom_faults(dom_method());

  const vector<TpgFault*>& src_list = analyzer.dom_fault_list();
  ymuint nf = src_list.size();

  ymuint max_fault_id = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* fault = src_list[i];
    if ( max_fault_id < fault->id() ) {
      max_fault_id = fault->id();
    }
  }
  ++ max_fault_id;

  mFaultStructList.clear();
  mFaultStructList.resize(nf);
  mFaultMap.clear();
  mFaultMap.resize(max_fault_id);

  for (ymuint i = 0; i < nf; ++ i) {
    FaultStruct& fs = mFaultStructList[i];
    TpgFault* fault = src_list[i];
    fs.mFault = fault;
    fs.mSelected = false;
    fs.mConflictNum = 0;
    fs.mConflictMap.resize(1, false);
    fs.mPendingNum = 0;
    fs.mPendingMap.resize(1, false);
    mFaultMap[fault->id()] = i;
  }
  mFaultNum = nf;
  mRemainNum = nf;

  mSimpleConfNum = 0;
  mSatConfNum = 0;
  mCompatNum = 0;
}

// @brief 対象の全故障数を返す．
ymuint
MinPatDsatur::fault_num()
{
  return mFaultNum;
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
// @param[in] group_list 現在のグループリスト
//
// 故障が残っていなければ NULL を返す．
TpgFault*
MinPatDsatur::get_next_fault(FgMgr& fgmgr,
			     const vector<ymuint>& group_list)
{
  if ( mRemainNum == 0 ) {
    if ( verbose() ) {
      cout << endl;
      cout << "SimpleConfNum: " << mSimpleConfNum << endl
	   << "SatConfNum:    " << mSatConfNum << endl
	   << "CompatNum:     " << mCompatNum << endl;
    }
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
      const NodeValList& ma_list = mAnalyzer.fault_info(fs.mFault->id()).mandatory_assignment();
      for (ymuint gid = 0; gid < ng; ++ gid) {
	if ( fs.mPendingMap[gid] ) {
	  fs.mPendingMap[gid] = false;
	  -- fs.mPendingNum;
	  const NodeValList& suf_list0 = fgmgr.sufficient_assignment(gid);
	  if ( check_conflict(suf_list0, ma_list) ) {
	    ++ fs.mConflictNum;
	    fs.mConflictMap[gid] = true;
	    ++ mSimpleConfNum;
	  }
	  else if ( !tpg_cnf.check_intersect(suf_list0) ) {
	    ++ fs.mConflictNum;
	    fs.mConflictMap[gid] = true;
	    ++ mSatConfNum;
	  }
	  else {
	    ++ mCompatNum;
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
// @param[in] group_list 現在のグループリスト
//
// グループが見つからなければ fgmgr.group_num() を返す．
ymuint
MinPatDsatur::find_group(FgMgr& fgmgr,
			 TpgFault* fault,
			 const vector<ymuint>& group_list)
{
  ymuint gid = MinPatBase::find_group(fgmgr, fault, group_list);
  mPrevGid = gid;
  ymuint ng = fgmgr.group_num();
  if ( gid == ng ) {
    for (ymuint i = 0; i < mFaultStructList.size(); ++ i) {
      FaultStruct& fs = mFaultStructList[i];
      if ( fs.mSelected ) {
	continue;
      }
      fs.mConflictMap.resize(ng + 1, false);
      fs.mPendingMap.resize(ng + 1, false);
    }
  }
  return ng;
}

END_NAMESPACE_YM_SATPG
