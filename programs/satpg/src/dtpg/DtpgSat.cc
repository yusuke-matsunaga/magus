
/// @file atpg/src/dtpg/DtpgSat.cc
/// @brief DtpgSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"
#include "DtpgNetwork.h"
#include "DtpgNode.h"
#include "DtpgFFR.h"
#include "DtpgFault.h"
#include "ym_logic/SatSolver.h"
#include "Op1.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

// @brief コンストラクタ
DtpgSat::DtpgSat()
{
  mNetwork = NULL;
}

// @brief デストラクタ
DtpgSat::~DtpgSat()
{
  delete mNetwork;
}

// @brief 回路と故障リストを設定する．
// @param[in] tgnetwork 対象のネットワーク
// @param[in] fault_list 故障リスト
void
DtpgSat::set_network(const TgNetwork& tgnetwork,
		     const vector<SaFault*>& fault_list)
{
  delete mNetwork;
  mNetwork = new DtpgNetwork(tgnetwork, fault_list);
}

// @brief single モードでテスト生成を行なう．
void
DtpgSat::single(FaultMgr& fmgr,
		TvMgr& tvmgr)
{
  Op1 op(fmgr, tvmgr, true);
  single(op);
}

// @brief dual モードでテスト生成を行なう．
void
DtpgSat::dual(FaultMgr& fmgr,
	      TvMgr& tvmgr)
{
  Op1 op(fmgr, tvmgr, true);
  dual(op);
}

// @brief ffr モードでテスト生成を行なう．
void
DtpgSat::ffr(FaultMgr& fmgr,
	     TvMgr& tvmgr)
{
  Op1 op(fmgr, tvmgr, true);
  ffr(op);
}

// @brief mffc モードでテスト生成を行なう．
void
DtpgSat::mffc(FaultMgr& fmgr,
	      TvMgr& tvmgr)
{
  Op1 op(fmgr, tvmgr, true);
  mffc(op);
}

// @brief all モードでテスト生成を行なう．
void
DtpgSat::all(FaultMgr& fmgr,
	     TvMgr& tvmgr)
{
  Op1 op(fmgr, tvmgr, true);
  all(op);
}

// @brief single モードでテスト生成を行なう．
void
DtpgSat::single_posplit(FaultMgr& fmgr,
			TvMgr& tvmgr,
			bool skip)
{
  Op1 op(fmgr, tvmgr, skip);
  single_posplit(op);

  if ( skip ) {
    clear_skip();
  }
}

// @brief dual モードでテスト生成を行なう．
void
DtpgSat::dual_posplit(FaultMgr& fmgr,
		      TvMgr& tvmgr,
		      bool skip)
{
  Op1 op(fmgr, tvmgr, skip);
  dual_posplit(op);

  if ( skip ) {
    clear_skip();
  }
}

// @brief ffr モードでテスト生成を行なう．
void
DtpgSat::ffr_posplit(FaultMgr& fmgr,
		     TvMgr& tvmgr,
		     bool skip)
{
  Op1 op(fmgr, tvmgr, skip);
  ffr_posplit(op);

  if ( skip ) {
    clear_skip();
  }
}

// @brief mffc モードでテスト生成を行なう．
void
DtpgSat::mffc_posplit(FaultMgr& fmgr,
		      TvMgr& tvmgr,
		      bool skip)
{
  Op1 op(fmgr, tvmgr, skip);
  mffc_posplit(op);

  if ( skip ) {
    clear_skip();
  }
}

// @brief all モードでテスト生成を行なう．
void
DtpgSat::all_posplit(FaultMgr& fmgr,
		     TvMgr& tvmgr,
		     bool skip)
{
  Op1 op(fmgr, tvmgr, skip);
  all_posplit(op);

  if ( skip ) {
    clear_skip();
  }
}

// @brief 一つの故障に対してテストパタン生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::single(DtpgOperator& op)
{
  mNetwork->activate_all();
  const vector<DtpgFault*>& flist = mNetwork->fault_list();
  for (vector<DtpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    DtpgFault* f = *p;
    if ( !f->is_skip() ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_single(solver, *mNetwork, f, op);
      update_stats(solver);
    }
  }
}

// @brief dual モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::dual(DtpgOperator& op)
{
  mNetwork->activate_all();
  ymuint nn = mNetwork->node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    DtpgNode* node = mNetwork->node(i);
    dual_mode_node(node, op);
  }
}

// @brief ffr モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::ffr(DtpgOperator& op)
{
  mNetwork->activate_all();
  vector<DtpgFFR*> ffr_list;
  mNetwork->get_ffr_list(ffr_list);
  cout << "#FFR = " << ffr_list.size() << endl;
  for (vector<DtpgFFR*>::const_iterator p = ffr_list.begin();
       p != ffr_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, op);
  }
}

// @brief mffc モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::mffc(DtpgOperator& op)
{
  mNetwork->activate_all();
  vector<DtpgFFR*> mffc_list;
  mNetwork->get_mffc_list(mffc_list);
  cout << "#MFFC = " << mffc_list.size() << endl;
  for (vector<DtpgFFR*>::const_iterator p = mffc_list.begin();
       p != mffc_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, op);
  }
}

// @brief all モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::all(DtpgOperator& op)
{
#if 0
  mNetwork->activate_all();
  vector<DtpgFFR*> mffc_list;
  mNetwork->get_mffc_list(mffc_list);
  for (vector<DtpgFFR*>::const_iterator p = mffc_list.begin();
       p != mffc_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, tvmgr, d_list, u_list);
  }
#endif
}

// @brief 一つの故障に対してテストパタン生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::single_posplit(DtpgOperator& op)
{
  const vector<DtpgFault*>& flist = mNetwork->fault_list();

  ymuint no = mNetwork->output_num2();
  for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
    mNetwork->activate_po(po_pos);
    for (vector<DtpgFault*>::const_iterator p = flist.begin();
	 p != flist.end(); ++ p) {
      DtpgFault* f = *p;
      if ( !f->is_skip() && f->node()->is_active() ) {
	SatSolver solver(mType, mOption, mOutP);
	dtpg_single(solver, *mNetwork, f, op);
	update_stats(solver);
      }
    }
  }
}

// @brief dual モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::dual_posplit(DtpgOperator& op)
{
  ymuint no = mNetwork->output_num2();
  for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
    mNetwork->activate_po(po_pos);
    ymuint nn = mNetwork->active_node_num();
    for (ymuint i = 0; i < nn; ++ i) {
      DtpgNode* node = mNetwork->active_node(i);
      dual_mode_node(node, op);
    }
  }
}

// @brief ffr モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::ffr_posplit(DtpgOperator& op)
{
  ymuint no = mNetwork->output_num2();
  for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
    mNetwork->activate_po(po_pos);
    vector<DtpgFFR*> ffr_list;
    mNetwork->get_ffr_list(ffr_list);
    for (vector<DtpgFFR*>::const_iterator p = ffr_list.begin();
	 p != ffr_list.end(); ++ p) {
      DtpgFFR* ffr = *p;
      ffr_mode(ffr, op);
    }
  }
}

// @brief mffc モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::mffc_posplit(DtpgOperator& op)
{
  ymuint no = mNetwork->output_num2();
  for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
    mNetwork->activate_po(po_pos);
    vector<DtpgFFR*> mffc_list;
    mNetwork->get_mffc_list(mffc_list);
    for (vector<DtpgFFR*>::const_iterator p = mffc_list.begin();
	 p != mffc_list.end(); ++ p) {
      DtpgFFR* ffr = *p;
      ffr_mode(ffr, op);
    }
  }
}

// @brief all モードでテスト生成を行なう．
// @param[in] op テスト生成後に呼ばれるファンクター
void
DtpgSat::all_posplit(DtpgOperator& op)
{
#if 0
  mNetwork->activate_po(po_pos);
  vector<DtpgFFR*> ffr_list;
  mNetwork->get_ffr_list(ffr_list);
  for (vector<DtpgFFR*>::const_iterator p = ffr_list.begin();
       p != ffr_list.end(); ++ p) {
    DtpgFFR* ffr = *p;
    ffr_mode(ffr, tvmgr, d_list, u_list);
  }
#endif
}

// @brief 使用する SAT エンジンを指定する．
void
DtpgSat::set_mode(const string& type,
		  const string& option,
		  ostream* outp)
{
  mType = type;
  mOption = option;
  mOutP = outp;
}

// @brief 一つのノードに対して dual_mode でテストパタン生成を行なう．
// @param[in] node 対象のノード
// @param[in] fmgr 故障の管理用オブジェクト
// @param[in] tvmgr テストベクタの管理用オブジェクト
// @param[out] u_list 検査不能故障を格納するリスト
void
DtpgSat::dual_mode_node(DtpgNode* node,
			DtpgOperator& op)
{
  DtpgFault* f0 = node->output_fault(0);
  DtpgFault* f1 = node->output_fault(1);
  if ( f0 != NULL && f0->is_skip() ) {
    f0 = NULL;
  }
  if ( f1 != NULL && f1->is_skip() ) {
    f1 = NULL;
  }
  if ( f0 != NULL && f1 != NULL ) {
    SatSolver solver(mType, mOption, mOutP);
    dtpg_dual(solver, *mNetwork, f0, f1, op);
    update_stats(solver);
  }
  else if ( f0 != NULL ) {
    SatSolver solver(mType, mOption, mOutP);
    dtpg_single(solver, *mNetwork, f0, op);
    update_stats(solver);
  }
  else if ( f1 != NULL ) {
    SatSolver solver(mType, mOption, mOutP);
    dtpg_single(solver, *mNetwork, f1, op);
    update_stats(solver);
  }
  ymuint ni = node->fanin_num();
  for (ymuint j = 0; j < ni; ++ j) {
    DtpgFault* f0 = node->input_fault(0, j);
    DtpgFault* f1 = node->input_fault(1, j);
    if ( f0 != NULL && f0->is_skip() ) {
      f0 = NULL;
    }
    if ( f1 != NULL && f1->is_skip() ) {
      f1 = NULL;
    }
    if ( f0 != NULL && f1 != NULL ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_dual(solver, *mNetwork, f0, f1, op);
      update_stats(solver);
    }
    else if ( f0 != NULL ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_single(solver, *mNetwork, f0, op);
      update_stats(solver);
    }
    else if ( f1 != NULL ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_single(solver, *mNetwork, f1, op);
      update_stats(solver);
    }
  }
}

// @brief 一つの FFR に対してテストパタン生成を行う．
// @param[in] ffr 対象の FFR
void
DtpgSat::ffr_mode(DtpgFFR* ffr,
		  DtpgOperator& op)
{
  vector<DtpgFault*> flist;
  const vector<DtpgNode*>& node_list = ffr->node_list();
  for (vector<DtpgNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    DtpgNode* node = *p;
    for (int val = 0; val < 2; ++ val) {
      DtpgFault* f = node->output_fault(val);
      if ( f != NULL && !f->is_skip() ) {
	flist.push_back(f);
      }
    }
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      for (int val = 0; val < 2; ++ val) {
	DtpgFault* f = node->input_fault(val, i);
	if ( f != NULL && !f->is_skip() ) {
	  flist.push_back(f);
	}
      }
    }
  }

  if ( flist.empty() ) {
    return;
  }

  SatSolver solver(mType, mOption, mOutP);
  dtpg_ffr(solver, *mNetwork, flist, ffr->root(), node_list, op);
  update_stats(solver);
}

// @brief スキップフラグを解除する．
void
DtpgSat::clear_skip()
{
  const vector<DtpgFault*>& flist = mNetwork->fault_list();
  for (vector<DtpgFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    DtpgFault* f = *p;
    if ( f->is_skip() && f->safault()->status() == kFsUntestable ) {
      f->clear_skip();
    }
  }
}

// @brief 統計情報をクリアする．
void
DtpgSat::clear_stats()
{
  mRunCount = 0;
  mRestart = 0;
  mVarNum = 0;
  mConstrClauseNum = 0;
  mConstrLitNum = 0;
  mLearntClauseNum = 0;
  mLearntLitNum = 0;
  mConflictNum = 0;
  mDecisionNum = 0;
  mPropagationNum = 0;
}

// @brief 統計情報を得る．
void
DtpgSat::get_stats() const
{
  if ( mRunCount > 0 ) {
    cout << "# of runs:                     " << mRunCount << endl
	 << "Ave. # of restarts:            " << (double) mRestart / mRunCount << endl
	 << "Ave. # of varriables:          " << (double) mVarNum / mRunCount << endl
	 << "Ave. # of constraint clauses:  " << (double) mConstrClauseNum / mRunCount << endl
	 << "Ave. # of constraint literals: " << (double) mConstrLitNum / mRunCount << endl
	 << "Ave. # of learnt clauses:      " << (double) mLearntClauseNum / mRunCount << endl
	 << "Ave. # of learnt literals:     " << (double) mLearntLitNum / mRunCount << endl
	 << "Ave. # of conflicts:           " << (double) mConflictNum / mRunCount << endl
	 << "Ave. # of decisions:           " << (double) mDecisionNum / mRunCount << endl
	 << "Av.e # of implications:        " << (double) mPropagationNum / mRunCount << endl
	 << endl;
  }
}

// @brief 統計情報を得る．
void
DtpgSat::update_stats(SatSolver& solver)
{
  SatStats sat_stat;
  solver.get_stats(sat_stat);

  ++ mRunCount;
  mRestart += sat_stat.mRestart;
  mVarNum += sat_stat.mVarNum;
  mConstrClauseNum += sat_stat.mConstrClauseNum;
  mConstrLitNum += sat_stat.mConstrLitNum;
  mLearntClauseNum += sat_stat.mLearntClauseNum;
  mLearntLitNum += sat_stat.mLearntLitNum;
  mConflictNum += sat_stat.mConflictNum;
  mDecisionNum += sat_stat.mDecisionNum;
  mPropagationNum += sat_stat.mPropagationNum;
}

END_NAMESPACE_YM_SATPG_DTPG
