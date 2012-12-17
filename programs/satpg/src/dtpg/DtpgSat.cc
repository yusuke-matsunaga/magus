
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
#include "DtpgOperator.h"
#include "FaultMgr.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "ym_logic/SatSolver.h"
#include "ym_logic/LogExpr.h"
#include "ym_networks/TgNetwork.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// DtpgSat 用の DtpgOperator
//////////////////////////////////////////////////////////////////////
class Operator1 :
  public DtpgOperator
{
public:

  /// @brief コンストラクタ
  Operator1(FaultMgr& fmgr,
	    TvMgr& tvmgr);

  /// @brief デストラクタ
  ~Operator1();


public:

  /// @brief テストパタンが見つかった場合に呼ばれる関数
  virtual
  void
  set_detected(DtpgFault* f,
	       TestVector* tv);

  /// @brief 検出不能のときに呼ばれる関数
  virtual
  void
  set_untestable(DtpgFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  FaultMgr& mFaultMgr;

  TvMgr& mTvMgr;

};

// @brief コンストラクタ
Operator1::Operator1(FaultMgr& fmgr,
		     TvMgr& tvmgr) :
  mFaultMgr(fmgr),
  mTvMgr(tvmgr)
{
}

// @brief デストラクタ
Operator1::~Operator1()
{
}

// @brief テストパタンが見つかった場合に呼ばれる関数
void
Operator1::set_detected(DtpgFault* f,
			TestVector* tv)
{
  mFaultMgr.set_status(f->safault(), kFsDetected);
}

// @brief 検出不能のときに呼ばれる関数
void
Operator1::set_untestable(DtpgFault* f)
{
  mFaultMgr.set_status(f->safault(), kFsUntestable);
}

END_NONAMESPACE


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
    if ( f->stat() == kFsUndetected ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_single(solver, *mNetwork, f, op);
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
  vector<DtpgFault*> u_list;
  for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
    mNetwork->activate_po(po_pos);
    for (vector<DtpgFault*>::const_iterator p = flist.begin();
	 p != flist.end(); ++ p) {
      DtpgFault* f = *p;
      if ( f->stat() == kFsUndetected && f->node()->is_active() ) {
	SatSolver solver(mType, mOption, mOutP);
	dtpg_single(solver, *mNetwork, f, op);
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
  if ( f0 != NULL && f0->stat() == kFsDetected ) {
    f0 = NULL;
  }
  if ( f1 != NULL && f1->stat() == kFsDetected ) {
    f1 = NULL;
  }
  if ( f0 != NULL && f1 != NULL ) {
    SatSolver solver(mType, mOption, mOutP);
    dtpg_dual(solver, *mNetwork, f0, f1, op);
  }
  else if ( f0 != NULL ) {
    SatSolver solver(mType, mOption, mOutP);
    dtpg_single(solver, *mNetwork, f0, op);
  }
  else if ( f1 != NULL ) {
    SatSolver solver(mType, mOption, mOutP);
    dtpg_single(solver, *mNetwork, f1, op);
  }
  ymuint ni = node->fanin_num();
  for (ymuint j = 0; j < ni; ++ j) {
    DtpgFault* f0 = node->input_fault(0, j);
    DtpgFault* f1 = node->input_fault(1, j);
    if ( f0 != NULL && f0->stat() == kFsDetected ) {
      f0 = NULL;
    }
    if ( f1 != NULL && f1->stat() == kFsDetected ) {
      f1 = NULL;
    }
    if ( f0 != NULL && f1 != NULL ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_dual(solver, *mNetwork, f0, f1, op);
    }
    else if ( f0 != NULL ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_single(solver, *mNetwork, f0, op);
    }
    else if ( f1 != NULL ) {
      SatSolver solver(mType, mOption, mOutP);
      dtpg_single(solver, *mNetwork, f1, op);
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
      if ( f->stat() == kFsUndetected ) {
	flist.push_back(f);
      }
    }
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      for (int val = 0; val < 2; ++ val) {
	DtpgFault* f = node->input_fault(val, i);
	if ( f->stat() == kFsUndetected ) {
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
}

// @brief StatsList をクリアする．
void
DtpgSat::clear_stats_list()
{
  mStatsList.clear();
}

// @brief StatsList を得る．
const vector<SatStats>&
DtpgSat::stats_list() const
{
  return mStatsList;
}

END_NAMESPACE_YM_SATPG_DTPG
