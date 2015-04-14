
/// @file MinPatDsatur2.cc
/// @brief MinPatDsatur2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatDsatur2.h"
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
new_MinPatDsatur2()
{
  return new MinPatDsatur2();
}

//////////////////////////////////////////////////////////////////////
// クラス MinPatDsatur2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatDsatur2::MinPatDsatur2()
{
}

// @brief デストラクタ
MinPatDsatur2::~MinPatDsatur2()
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
MinPatDsatur2::init(TpgNetwork& network,
		    TvMgr& tvmgr,
		    FaultMgr& fmgr,
		    Fsim& fsim2,
		    vector<TestVector*>& tv_list)
{
  mMaxNodeId = network.max_node_id();

  FaultAnalyzer& analyzer = mAnalyzer;

  analyzer.set_verbose(verbose());

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

  analyzer.analyze_faults();

  analyzer.analyze_conflict();

  const vector<TpgFault*>& src_list = analyzer.dom_fault_list();
  ymuint nf = src_list.size();

  mMaxFaultId = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* fault = src_list[i];
    if ( mMaxFaultId < fault->id() ) {
      mMaxFaultId = fault->id();
    }
  }
  ++ mMaxFaultId;

  mFaultStructList.clear();
  mFaultStructList.resize(nf);
  mFaultMap.clear();
  mFaultMap.resize(mMaxFaultId);

  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* fault = src_list[i];
    FaultStruct& fs = mFaultStructList[i];
    fs.mFault = fault;
    fs.mSelected = false;
    fs.mConflictList = analyzer.fault_info(fault->id()).mConflictList;
    fs.mConflictNum = 0;
    fs.mConflictMap.resize(1, false);
    mFaultMap[fault->id()] = i;
  }

  mRemainNum = nf;

  return nf;
}

// @brief 最初の故障を選ぶ．
TpgFault*
MinPatDsatur2::get_first_fault()
{
  ASSERT_COND( mRemainNum > 0 );

  // 最初は衝突数の多い故障を選ぶ．
  ymuint max_count = 0;
  ymuint max_pos = 0;
  ymuint fault_num = mFaultStructList.size();
  for (ymuint i = 0; i < fault_num; ++ i) {
    FaultStruct& fs = mFaultStructList[i];
    if ( max_count < fs.mConflictList.size() ) {
      max_count = fs.mConflictList.size();
      max_pos = i;
    }
  }
  mFaultStructList[max_pos].mSelected = true;
  -- mRemainNum;
  mPrevFpos = max_pos;
  mPrevGid = 0;
  return mFaultStructList[max_pos].mFault;
}

// @brief 次に処理すべき故障を選ぶ．
// @param[in] fgmgr 故障グループを管理するオブジェクト
//
// 故障が残っていなければ NULL を返す．
TpgFault*
MinPatDsatur2::get_next_fault(FgMgr& fgmgr)
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
  ymuint max_conf = 0;
  for (ymuint i = 0; i < fault_num; ++ i) {
    FaultStruct& fs = mFaultStructList[i];
    if ( fs.mSelected ) {
      continue;
    }
    if ( !fs.mConflictMap[mPrevGid] ) {
      FaultStruct& fs2 = mFaultStructList[mPrevFpos];
      for (ymuint j = 0; j < fs2.mConflictList.size(); ++ j) {
	ymuint fid = fs2.mConflictList[j];
	if ( fid == fs.mFault->id() ) {
	  fs.mConflictMap[mPrevGid] = true;
	  ++ fs.mConflictNum;
	  break;
	}
      }
    }
    if ( first || max_satur < fs.mConflictNum ) {
      first = false;
      max_satur = fs.mConflictNum;
      max_pos = i;
      ymuint conf_num = 0;
      for (ymuint j = 0; j < fs.mConflictList.size(); ++ j) {
	ymuint fid = fs.mConflictList[j];
	if ( !mFaultStructList[mFaultMap[fid]].mSelected ) {
	  ++ conf_num;
	}
      }
      max_conf = conf_num;
    }
    else if ( max_satur == fs.mConflictNum ) {
      // 同点の場合には未選択の故障と多く衝突するものを選ぶ．
      ymuint conf_num = 0;
      for (ymuint j = 0; j < fs.mConflictList.size(); ++ j) {
	ymuint fid = fs.mConflictList[j];
	if ( !mFaultStructList[mFaultMap[fid]].mSelected ) {
	  ++ conf_num;
	}
      }
      if ( max_conf < conf_num ) {
	max_conf = conf_num;
	max_pos = i;
      }
    }
  }
  mFaultStructList[max_pos].mSelected = true;
  -- mRemainNum;
  mPrevFpos = max_pos;
  return mFaultStructList[max_pos].mFault;
}

// @brief 故障を追加するグループを選ぶ．
// @param[in] fgmgr 故障グループを管理するオブジェクト
// @param[in] fault 故障
//
// グループが見つからなければ fgmgr.group_num() を返す．
ymuint
MinPatDsatur2::find_group(FgMgr& fgmgr,
			  TpgFault* fault)
{
  ymuint ng = fgmgr.group_num();
  ymuint gid = fgmgr.find_group(fault);
  mPrevGid = gid;
  if ( gid == ng ) {
    // 新しいグループを作ることになる．
    for (ymuint i = 0; i < mFaultStructList.size(); ++ i) {
      FaultStruct& fs = mFaultStructList[i];
      if ( fs.mSelected ) {
	continue;
      }
      fs.mConflictMap.resize(ng + 1, false);
    }
  }
  return gid;
}

END_NAMESPACE_YM_SATPG
