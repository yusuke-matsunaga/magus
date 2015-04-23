
/// @file FaultAnalyzer.cc
/// @brief FaultAnalyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FaultAnalyzer.h"

#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "NodeSet.h"
#include "NodeValList.h"

#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"

#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

void
mark_tfi(const TpgNode* node,
	 HashSet<ymuint>& tfi_mark,
	 vector<ymuint>& input_list)
{
  if ( tfi_mark.check(node->id()) ) {
    return;
  }
  tfi_mark.add(node->id());

  if ( node->is_input() ) {
    input_list.push_back(node->input_id());
  }
  else {
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = node->fanin(i);
      mark_tfi(inode, tfi_mark, input_list);
    }
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス FaultAnalyzer
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FaultAnalyzer::FaultAnalyzer()
{
  mVerbose = 0;
}

// @brief デストラクタ
FaultAnalyzer::~FaultAnalyzer()
{
}

// @brief verbose フラグを設定する．
// @param[in] verbose 表示を制御するフラグ
void
FaultAnalyzer::set_verbose(int verbose)
{
  mVerbose = verbose;
}

// @brief verbose フラグを得る．
int
FaultAnalyzer::verbose() const
{
  return mVerbose;
}

// @brief 初期化する．
// @param[in] network ネットワーク
// @param[in] tvmgr テストベクタのマネージャ
// @param[out] fault_list 検出された故障のリスト
void
FaultAnalyzer::init(const TpgNetwork& network,
		    TvMgr& tvmgr,
		    vector<const TpgFault*>& fault_list)
{
  StopWatch local_timer;
  local_timer.start();

  ymuint nn = network.active_node_num();
  mMaxNodeId = network.max_node_id();
  mMaxFaultId = 0;
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }
    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      const TpgFault* fault = node->fault(i);
      ymuint f_id = fault->id();
      if ( mMaxFaultId < f_id ) {
	mMaxFaultId = f_id;
      }
    }
  }
  ++ mMaxFaultId;

  mInputListArray.clear();
  mInputListArray.resize(mMaxNodeId);

  mInputList2Array.clear();
  mInputList2Array.resize(mMaxNodeId);

  mNodeSetArray.clear();
  mNodeSetArray.resize(mMaxNodeId);

  mFaultInfoArray.clear();
  mFaultInfoArray.resize(mMaxFaultId);

  ymuint f_all = 0;
  ymuint f_det = 0;
  ymuint f_red = 0;
  ymuint f_abt = 0;

  mTestVectorList.clear();
  vector<bool> det_flag(mMaxFaultId, false);
  for (ymuint i = 0; i < nn; ++ i) {
    if ( verbose() > 1 ) {
      cout << "\r" << setw(6) << i << " / " << setw(6) << nn;
      cout.flush();
    }

    const TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    NodeSet& node_set = mNodeSetArray[node->id()];
    node_set.mark_region(mMaxNodeId, node);

    vector<ymuint>& input_list = mInputListArray[node->id()];
    for (ymuint j = 0; j < node_set.tfo_tfi_size(); ++ j) {
      const TpgNode* node1 = node_set.tfo_tfi_node(j);
      if ( node1->is_input() ) {
	input_list.push_back(node1->input_id());
      }
    }
    // ソートしておく．
    sort(input_list.begin(), input_list.end());

    // 故障箇所の TFI に含まれる入力番号を mInputList2Array に入れる．
    HashSet<ymuint> tfi_mark;
    vector<ymuint>& input_list2 = mInputList2Array[node->id()];
    mark_tfi(node, tfi_mark, input_list2);
    // ソートしておく．
    sort(input_list2.begin(), input_list2.end());

    ymuint nf = node->fault_num();
    for (ymuint j = 0; j < nf; ++ j) {
      const TpgFault* fault = node->fault(j);
      Bool3 stat = analyze_fault(fault, tvmgr);
      ++ f_all;
      switch ( stat ) {
      case kB3True:
	++ f_det;
	det_flag[fault->id()] = true;
	break;

      case kB3False:
	++ f_red;
	break;

      case kB3X:
	++ f_abt;
      }
    }
  }

  mOrigFaultList.clear();
  mOrigFaultList.reserve(f_det);
  for (ymuint i = 0; i < network.active_node_num(); ++ i) {
    const TpgNode* node = network.active_node(i);
    ymuint ni = node->fanin_num();
    bool has_ncfault = false;
    for (ymuint j = 0; j < ni; ++ j) {
      const TpgFault* f0 = node->input_fault(0, j);
      if ( f0 != NULL ) {
	if ( f0->is_rep() && det_flag[f0->id()] ) {
	  mOrigFaultList.push_back(f0);
	}
	if ( node->nval() == kVal0 && det_flag[f0->rep_fault()->id()] ) {
	  has_ncfault = true;
	}
      }
      const TpgFault* f1 = node->input_fault(1, j);
      if ( f1 != NULL ) {
	if ( f1->is_rep() && det_flag[f1->id()] ) {
	  mOrigFaultList.push_back(f1);
	}
	if ( node->nval() == kVal1 && det_flag[f1->rep_fault()->id()] ) {
	  has_ncfault = true;
	}
      }
    }
    const TpgFault* f0 = node->output_fault(0);
    if ( f0 != NULL && f0->is_rep() && det_flag[f0->id()] ) {
      if ( node->noval() != kVal0 || !has_ncfault ) {
	mOrigFaultList.push_back(f0);
      }
    }
    const TpgFault* f1 = node->output_fault(1);
    if ( f1 != NULL && f1->is_rep() && det_flag[f1->id()] ) {
      if ( node->noval() != kVal1 || !has_ncfault ) {
	mOrigFaultList.push_back(f1);
      }
    }
  }

  fault_list = mOrigFaultList;

  local_timer.stop();

  if ( verbose() > 0 ) {
    if ( verbose() > 1 ) {
      cout << endl;
    }
    cout << "Total " << setw(6) << f_all << " faults" << endl
	 << "Total " << setw(6) << f_det << " detected faults" << endl
	 << "Total " << setw(6) << f_red << " redundant faults" << endl
	 << "Total " << setw(6) << f_abt << " aborted faults" << endl
	 << "CPU time " << local_timer.time() << endl;
  }
}

// @brief 故障の解析を行う．
// @param[in] fault 故障
// @param[in] tvmgr テストベクタのマネージャ
Bool3
FaultAnalyzer::analyze_fault(const TpgFault* fault,
			     TvMgr& tvmgr)
{
  ymuint f_id = fault->id();
  FaultInfo& fi = mFaultInfoArray[f_id];

  fi.mFault = fault;

  GvalCnf gval_cnf(mMaxNodeId);
  FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
  SatEngine engine(string(), string(), NULL);

  engine.make_fval_cnf(fval_cnf, fault, node_set(f_id), kVal1);

  vector<Bool3> sat_model;
  Bool3 sat_stat = engine.check_sat(sat_model);
  if ( sat_stat == kB3True ) {
    NodeValList& suf_list = fi.mSufficientAssignment;
    NodeValList& pi_suf_list = fi.mPiSufficientAssignment;

    fval_cnf.get_pi_suf_list(sat_model, fault, node_set(f_id), suf_list, pi_suf_list);

    // テストベクタを作る．
    TestVector* tv = tvmgr.new_vector();
    ymuint npi = pi_suf_list.size();
    for (ymuint i = 0; i < npi; ++ i) {
      NodeVal nv = pi_suf_list[i];
      const TpgNode* node = nv.node();
      ASSERT_COND ( node->is_input() );
      ymuint id = node->input_id();
      if ( nv.val() ) {
	tv->set_val(id, kVal1);
      }
      else {
	tv->set_val(id, kVal0);
      }
    }
    // X の部分をランダムに設定しておく
    tv->fix_x_from_random(mRandGen);

    fi.mTestVector = tv;

    // 必要割当を求める．
    NodeValList& ma_list = fi.mMandatoryAssignment;
    ymuint n = suf_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      NodeVal nv = suf_list[i];

      NodeValList list1;
      const TpgNode* node = nv.node();
      bool val = nv.val();
      list1.add(node, !val);
      if ( engine.check_sat(gval_cnf, list1) == kB3False ) {
	// node の値を反転したら検出できなかった．
	// -> この割当は必須割当
	ma_list.add(node, val);
      }
    }
    if ( suf_list.size() == ma_list.size() ) {
      fi.mSingleCube = true;
    }
    else {
      NodeValList diff_list = suf_list;
      diff_list.diff(ma_list);
      ASSERT_COND( diff_list.size() > 0 );
      engine.add_negation(gval_cnf, diff_list);
      vector<Bool3> sat_model;
      Bool3 sat_ans = engine.check_sat(sat_model);
      ASSERT_COND( sat_ans == kB3True );
      for (ymuint i = 0; i < 2; ++ i) {
	NodeValList suf_list;
	fval_cnf.get_suf_list(sat_model, fault, node_set(f_id), suf_list);
	suf_list.diff(ma_list);

	fi.mOtherSufListArray.push_back(suf_list);
	engine.add_negation(gval_cnf, suf_list);
	sat_ans = engine.check_sat(sat_model);
	if ( sat_ans == kB3False ) {
	  break;
	}
      }
    }
  }
  return sat_stat;
}

// @brief ノード番号の最大値を得る．
ymuint
FaultAnalyzer::max_node_id() const
{
  return mMaxNodeId;
}

// @brief 故障番号の最大値を得る．
ymuint
FaultAnalyzer::max_fault_id() const
{
  return mMaxFaultId;
}

// @brief 検出可能な故障のリストを得る．
const vector<const TpgFault*>&
FaultAnalyzer::fault_list() const
{
  return mOrigFaultList;
}

// @brief 故障を得る．
// @param[in] fid 故障番号
const TpgFault*
FaultAnalyzer::fault(ymuint fid) const
{
  ASSERT_COND( fid < mMaxFaultId );
  return mFaultInfoArray[fid].fault();
}

// @brief 個別の故障の情報を得る．
// @param[in] fid 故障番号
const FaultInfo&
FaultAnalyzer::fault_info(ymuint fid) const
{
  ASSERT_COND( fid < mMaxFaultId );
  return mFaultInfoArray[fid];
}

// @brief 故障のTFOのTFIに含まれる入力番号のリスト返す．
// @param[in] fid 故障番号
const vector<ymuint>&
FaultAnalyzer::input_list(ymuint fid) const
{
  ASSERT_COND( fid < mMaxFaultId );
  const TpgFault* fault = mFaultInfoArray[fid].fault();
  return mInputListArray[fault->node()->id()];
}

// @brief 故障のTFIに含まれる入力番号のリスト返す．
// @param[in] fid 故障番号
const vector<ymuint>&
FaultAnalyzer::input_list2(ymuint fid) const
{
  ASSERT_COND( fid < mMaxFaultId );
  const TpgFault* fault = mFaultInfoArray[fid].fault();
  return mInputList2Array[fault->node()->id()];
}

// @brief 故障に関連するノード集合を返す．
const NodeSet&
FaultAnalyzer::node_set(ymuint fid) const
{
  ASSERT_COND( fid < mMaxFaultId );
  const TpgFault* fault = mFaultInfoArray[fid].fault();
  return mNodeSetArray[fault->node()->id()];
}

END_NAMESPACE_YM_SATPG
