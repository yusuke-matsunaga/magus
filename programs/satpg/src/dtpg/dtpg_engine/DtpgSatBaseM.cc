
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
// @param[in] stats 結果を格納する構造体
void
DtpgSatBaseM::run(TpgNetwork& network,
		  DtpgStats& stats)
{
  clear_stats();

  mMark.clear();
  mMark.resize(network.max_node_id(), false);
  ymuint n = network.active_node_num();
  ymuint max_id = network.max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* node = network.active_node(i);
    if ( node->imm_dom() == NULL ) {
      mFaultNodeList.clear();
      mFaultList.clear();

      dfs_mffc(node);

      if ( !mFaultList.empty() ) {
	mark_region(max_id, mFaultNodeList);
	run_multi(network, mFaultNodeList, mFaultList);
      }
    }
  }

  get_stats(stats);
}

// @brief DFS で MFFC を求める．
// @param[in] node 対象のノード
void
DtpgSatBaseM::dfs_mffc(TpgNode* node)
{
  mMark[node->id()] = true;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( mMark[inode->id()] == false && inode->imm_dom() != NULL ) {
      dfs_mffc(inode);
    }
  }

  if ( !node->is_output() ) {
    mFaultNodeList.push_back(node);
    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      TpgFault* f = node->fault(i);
      if ( f->status() != kFsDetected &&
	   !f->is_skip() ) {
	mFaultList.push_back(f);
      }
    }
  }
}

END_NAMESPACE_YM_SATPG
