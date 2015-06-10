
/// @file DtpgSatBaseM.cc
/// @brief DtpgSatBaseM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatBaseM.h"

#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "NodeSet.h"
#include "FaultMgr.h"
#include "Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
DtpgSatBaseM::DtpgSatBaseM(const string& sat_type,
			   const string& sat_option,
			   ostream* sat_outp,
			   BackTracer& bt,
			   DetectOp& dop,
			   UntestOp& uop) :
  DtpgSat(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatBaseM::~DtpgSatBaseM()
{
}

// @brief テスト生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
// @param[in] fault_list 対象の故障リスト
// @param[in] stats 結果を格納する構造体
void
DtpgSatBaseM::run(TpgNetwork& network,
		  FaultMgr& fmgr,
		  Fsim& fsim,
		  const vector<const TpgFault*>& fault_list,
		  DtpgStats& stats)
{
  clear_stats();

  // 故障シミュレータに故障リストをセットする．
  fsim.set_faults(fault_list);

  NodeSet node_set;

  ymuint max_id = network.max_node_id();

  // 故障番号の最大値を求める．
  ymuint max_fault_id = 0;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    const TpgFault* fault = fault_list[i];
    ymuint fid = fault->id();
    if ( max_fault_id < fid ) {
      max_fault_id = fid;
    }
  }
  ++ max_fault_id;

  // fault_list に含まれる故障に印をつける．
  mFaultMark.clear();
  mFaultMark.resize(max_fault_id, false);
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    const TpgFault* fault = fault_list[i];
    ymuint fid = fault->id();
    mFaultMark[fid] = true;
  }

  mMark.clear();
  mMark.resize(max_id, false);
  ymuint n = network.active_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = network.active_node(i);
    if ( node->imm_dom() == NULL ) {
      mFaultNodeList.clear();
      mFaultList.clear();

      dfs_mffc(node, fmgr);

      if ( !mFaultList.empty() ) {
	node_set.mark_region(max_id, mFaultNodeList);
	run_multi(node_set, mFaultNodeList, mFaultList);
      }
    }
  }

  get_stats(stats);
}

// @brief DFS で MFFC を求める．
// @param[in] node 対象のノード
void
DtpgSatBaseM::dfs_mffc(const TpgNode* node,
		       FaultMgr& fmgr)
{
  mMark[node->id()] = true;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    if ( mMark[inode->id()] == false && inode->imm_dom() != NULL ) {
      dfs_mffc(inode, fmgr);
    }
  }

  if ( !node->is_output() ) {
    mFaultNodeList.push_back(node);
    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      const TpgFault* f = node->fault(i);
      if ( mFaultMark[f->id()] && fmgr.status(f) == kFsUndetected ) {
	mFaultList.push_back(f);
      }
    }
  }
}

END_NAMESPACE_YM_SATPG
