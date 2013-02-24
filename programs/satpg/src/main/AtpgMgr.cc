
/// @file src/main/AtpgMgr.cc
/// @brief AtpgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgMgr.h"
#include "Op1.h"
#include "ym_networks/TgBlifReader.h"
#include "ym_networks/TgIscas89Reader.h"
#include "ym_networks/TgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FsimNetBinder
//////////////////////////////////////////////////////////////////////
class FsimNetBinder :
  public T2Binder<const TgNetwork&, const vector<SaFault*>&>
{
public:

  /// @brief コンストラクタ
  FsimNetBinder(Fsim* fsim);

  /// @brief イベント処理関数
  virtual
  void
  event_proc(const TgNetwork& network,
	     const vector<SaFault*>& fault_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Fsim
  Fsim* mFsim;

};


// @brief コンストラクタ
FsimNetBinder::FsimNetBinder(Fsim* fsim) :
  mFsim(fsim)
{
}

// @brief イベント処理関数
void
FsimNetBinder::event_proc(const TgNetwork& network,
			  const vector<SaFault*>& fault_list)
{
  mFsim->set_network(network, fault_list);
}


//////////////////////////////////////////////////////////////////////
// AtpgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AtpgMgr::AtpgMgr() :
  mTimer(TM_SIZE, TM_MISC)
{
  mFsim = new_Fsim2();
  mFsim3 = new_Fsim3();

  mDtpg = new_DtpgSat();
  mDtpgOld = new_DtpgSatOld();

  mDtpgDrop = false;

  mDtpgVerify = false;

  reg_network_handler(new FsimNetBinder(mFsim));
  reg_network_handler(new FsimNetBinder(mFsim3));

  set_dtpg_mode();
}

// @brief デストラクタ
AtpgMgr::~AtpgMgr()
{
  delete mFsim;
  delete mFsim3;
  delete mDtpg;
  delete mDtpgOld;
}

// @brief blif 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] cell_library セルライブラリ
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
AtpgMgr::read_blif(const string& filename,
		   const CellLibrary* cell_library)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_READ);

  TgBlifReader read;
  bool stat = read(filename, mNetwork, cell_library);
  if ( stat ) {
    after_set_network();
  }

  mTimer.change(old_id);

  return stat;
}

// @brief iscas89 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
AtpgMgr::read_iscas89(const string& filename)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_READ);

  TgIscas89Reader read;
  bool stat = read(filename, mNetwork);
  if ( stat ) {
    after_set_network();
  }

  mTimer.change(old_id);

  return stat;
}

// @brief 一つのテストベクタに対する故障シミュレーションを行なう．
// @param[in] tv テストベクタ
// @param[out] det_faults 検出された故障を格納するリスト
void
AtpgMgr::fsim(TestVector* tv,
	      vector<SaFault*>& det_faults)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_FSIM);

  mFsim->run(tv, det_faults);

  ymuint det_count = det_faults.size();
  if ( det_count > 0 ) {
    for (vector<SaFault*>::const_iterator p = det_faults.begin();
	 p != det_faults.end(); ++ p) {
      SaFault* f = *p;
      if ( f->status() == kFsUndetected ) {
	mFaultMgr.set_status(f, kFsDetected);
      }
    }
    after_update_faults();
  }

  mTimer.change(old_id);
}

// @brief 複数のテストベクタに対する故障シミュレーションを行なう．
// @param[in] tv_list テストベクタのリスト
// @param[out] det_faults 検出された故障を格納するリストのリスト
void
AtpgMgr::fsim(const vector<TestVector*>& tv_list,
	      vector<vector<SaFault*> >& det_faults_list)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_FSIM);

  mFsim->run(tv_list, det_faults_list);

  ymuint det_count = 0;
  for (ymuint i = 0; i < tv_list.size(); ++ i) {
    const vector<SaFault*>& det_faults = det_faults_list[i];
    ymuint det_count1 = det_faults.size();
    if ( det_count1 > 0 ) {
      det_count += det_count1;
      for (vector<SaFault*>::const_iterator p = det_faults.begin();
	   p != det_faults.end(); ++ p) {
	SaFault* f = *p;
	if ( f->status() == kFsUndetected ) {
	  mFaultMgr.set_status(f, kFsDetected);
	}
      }
    }
  }

  if ( det_count > 0 ) {
    after_update_faults();
  }

  mTimer.change(old_id);
}

// @brief 一つのパタンで一つの故障に対するシミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] f 対象の故障
// @retval true tv で f の検出ができた．
// @retval false tv では f の検出ができない．
bool
AtpgMgr::fsim(TestVector* tv,
	      SaFault* f)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_FSIM);

  bool stat = mFsim->run(tv, f);
  if ( stat ) {
    if ( f->status() == kFsUndetected ) {
      mFaultMgr.set_status(f, kFsDetected);

      after_update_faults();
    }
  }

  mTimer.change(old_id);

  return stat;
}

// @brief 使用する SAT エンジンを指定する．
void
AtpgMgr::set_dtpg_mode(const string& type,
		       const string& option,
		       ostream* outp)
{
  mDtpg->set_mode(type, option, outp);
  mDtpgOld->set_mode(type, option, outp);
}

// @brief X抽出のモードを指定する．
void
AtpgMgr::set_dtpg_xmode(ymuint val)
{
  mDtpg->set_get_pat(val);
  mDtpgOld->set_get_pat(val);
}

// @brief dry-run フラグを設定する．
void
AtpgMgr::set_dtpg_dry_run(bool flag)
{
  mDtpg->set_dry_run(flag);
  mDtpgOld->set_dry_run(flag);
}

// @brief テストパタン生成時に故障ドロップを行なうかを指定する．
void
AtpgMgr::set_dtpg_drop_mode(bool drop)
{
  mDtpgDrop = drop;
}

// @brief テストパタン生成時に故障シミュレーションを用いて検証するかを指定する．
void
AtpgMgr::set_dtpg_verify_mode(bool verify)
{
  mDtpgVerify = verify;
}

// @brief テストパタン生成時に時間計測を行なうかどうかを指定する．
void
AtpgMgr::set_dtpg_timer(bool enable)
{
  mDtpg->timer_enable(enable);
  mDtpgOld->timer_enable(enable);
}

// @brief テストパタン生成を行なう．
void
AtpgMgr::dtpg(const string& option)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_DTPG);

  Op1 op(mFaultMgr, mTvMgr, mTvList, *mFsim3, mDtpgDrop, mDtpgVerify);

  mDtpg->run(op, option);

  after_update_faults();

  mTimer.change(old_id);
}

// @brief テストパタン生成を行なう．
void
AtpgMgr::dtpg_old(const string& option)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_DTPG);

  Op1 op(mFaultMgr, mTvMgr, mTvList, *mFsim3, mDtpgDrop, mDtpgVerify);

  mDtpgOld->run(op, option);

  after_update_faults();

  mTimer.change(old_id);
}


#if 0

// @brief テストパタン生成を行なう．
void
AtpgMgr::dtpg2(const string& option)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_DTPG);

  Op1 op(mFaultMgr, mTvMgr, mTvList, *mFsim3, mDtpgDrop, mDtpgVerify);
  bool single = false;
  bool dual = false;
  bool node = false;
  bool ffr = false;
  bool mffc = false;
  bool all = false;
  bool po = false;
  bool skip = false;

  if ( option == string() || option == "single" ) {
    single = true;
  }
  else if ( option == "dual" ) {
    dual = true;
  }
  else if ( option == "node" ) {
    node = true;
  }
  else if ( option == "ffr" ) {
    ffr = true;
  }
  else if ( option == "mffc" ) {
    mffc = true;
  }
  else if ( option == "all" ) {
    all = true;
  }
  else if ( option == "single_po" ) {
    single = true;
    po = true;
  }
  else if ( option == "single_po_skip" ) {
    single = true;
    po = true;
    skip = true;
  }
  else if ( option == "dual_po" ) {
    dual = true;
    po = true;
  }
  else if ( option == "dual_po_skip" ) {
    dual = true;
    po = true;
    skip = true;

  }
  else if ( option == "ffr_po" ) {
    ffr = true;
    po = true;
  }
  else if ( option == "ffr_po_skip" ) {
    ffr = true;
    po = true;
    skip = true;
  }
  else if ( option == "mffc_po" ) {
    mffc = true;
    po = true;
  }
  else if ( option == "mffc_po_skip" ) {
    mffc = true;
    po = true;
    skip = true;
  }
  else if ( option == "all_po" ) {
    all = true;
    po = true;
  }
  else if ( option == "all_po_skip" ) {
    all = true;
    po = true;
    skip = true;
  }
  else {
    cout << "illegal option " << option << ". ignored" << endl;
    return;
  }

  if ( !po ) {
    mDtpg->set_network(mNetwork, mFaultMgr.remain_list());

    ymuint nn = mNetwork.node_num();
    if ( single ) {
      vector<SaFault*> tmp(1);
      for (ymuint i = 0; i < nn; ++ i) {
	FaultInfo& fi = mFaultArray[i];
	ymuint n = fi.mAllFaults.size();
	for (ymuint j = 0; j < n; ++ j) {
	  tmp[0] = fi.mAllFaults[j];
	  mDtpg->run(tmp, op);
	}
      }
    }
    else if ( dual ) {
      vector<SaFault*> tmp(2);
      for (ymuint i = 0; i < nn; ++ i) {
	FaultInfo& fi = mFaultArray[i];
	tmp[0] = fi.mOutputFaults[0];
	tmp[1] = fi.mOutputFaults[1];
	mDtpg->run(tmp, op);
	ymuint n = fi.mInputFaults.size() / 2;
	for (ymuint j = 0; j < n; ++ j) {
	  tmp[0] = fi.mInputFaults[j * 2 + 0];
	  tmp[1] = fi.mInputFaults[j * 2 + 1];
	  mDtpg->run(tmp, op);
	}
      }
    }
    else if ( node ) {
      vector<SaFault*> tmp_list;
      for (ymuint i = 0; i < nn; ++ i) {
	tmp_list.clear();
	FaultInfo& fi = mFaultArray[i];
	for (vector<SaFault*>::iterator q = fi.mAllFaults.begin();
	     q != fi.mAllFaults.end(); ++ q) {
	  SaFault* f = *q;
	  tmp_list.push_back(f);
	}
	mDtpg->run(tmp_list, op);
      }
    }
    else if ( ffr ) {
      vector<SaFault*> tmp_list;
      for (ymuint i = 0; i < nn; ++ i) {
	const TgNode* node = mNetwork.node(i);
	if ( node->is_output() || node->fanout_num() > 1 ) {
	  tmp_list.clear();
	  dfs_ffr(node, tmp_list);
	  mDtpg->run(tmp_list, op);
	}
      }
    }
    else if ( mffc ) {
      vector<ymuint32> order(nn);
      vector<const TgNode*> node_list(nn);
      ymuint npi = mNetwork.input_num2();
      ymuint nl = mNetwork.logic_num();
      ymuint npo = mNetwork.output_num2();
      ymuint pos = 0;
      for (ymuint i = 0; i < npi; ++ i) {
	const TgNode* node = mNetwork.input(i);
	node_list[pos] = node;
	order[node->gid()] = pos;
	++ pos;
      }
      for (ymuint i = 0; i < nl; ++ i) {
	const TgNode* node = mNetwork.sorted_logic(i);
	node_list[pos] = node;
	order[node->gid()] = pos;
	++ pos;
      }
      for (ymuint i = 0; i < npo; ++ i) {
	const TgNode* node = mNetwork.output(i);
	node_list[pos] = node;
	order[node->gid()] = pos;
	++ pos;
      }

      mImmDomArray.clear();
      mImmDomArray.resize(nn, NULL);
      for (ymuint i = 0; i < nn; ++ i) {
	const TgNode* node = node_list[nn - i - 1];
	ymuint nfo = node->fanout_num();
	if ( nfo > 0 ) {
	  const TgNode* node0 = node->fanout(0);
	  for (ymuint j = 1; j < nfo; ++ j) {
	    node0 = merge(node0, node->fanout(j), order);
	    if ( node0 == NULL ) {
	      break;
	    }
	  }
	  mImmDomArray[node->gid()] = node0;
	}
      }

      vector<SaFault*> tmp_list;
      vector<bool> mark(nn, false);
      for (ymuint i = 0; i < nn; ++ i) {
	const TgNode* node = mNetwork.node(i);
	if ( mImmDomArray[node->gid()] == NULL ) {
	  tmp_list.clear();
	  dfs_mffc(node, mark, tmp_list);
	  mDtpg->run(tmp_list, op);
	}
      }
    }
    else if ( all ) {
      vector<SaFault*> tmp_list;
      for (ymuint i = 0; i < nn; ++ i) {
	FaultInfo& fi = mFaultArray[i];
	for (vector<SaFault*>::iterator q = fi.mAllFaults.begin();
	     q != fi.mAllFaults.end(); ++ q) {
	  SaFault* f = *q;
	  tmp_list.push_back(f);
	}
      }
      mDtpg->run(tmp_list, op);
    }
  }
#if 0
  else {
    ymuint no = mNetwork->output_num2();
    for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
      DtpgNode* onode = mNetwork->output2(po_pos);
      mNetwork->activate_po(onode);
      if ( single ) {
	single_sub(op);
      }
      else if ( dual ) {
	dual_sub(op);
      }
      else if ( ffr ) {
	ffr_sub(op);
      }
      else if ( mffc ) {
	mffc_sub(op);
      }
      else if ( all ) {
	all_sub(op);
      }
    }
    if ( skip ) {
      clear_skip();
    }

    skip = false;
  }
#endif

  after_update_faults();

  mTimer.change(old_id);
}

void
AtpgMgr::dfs_ffr(const TgNode* node,
		 vector<SaFault*>& flist)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TgNode* inode = node->fanin(i);
    if ( inode->fanout_num() == 1 ) {
      dfs_ffr(inode, flist);
    }
  }

  FaultInfo& fi = mFaultArray[node->gid()];
  for (vector<SaFault*>::iterator q = fi.mAllFaults.begin();
       q != fi.mAllFaults.end(); ++ q) {
    SaFault* f = *q;
    flist.push_back(f);
  }
}

void
AtpgMgr::dfs_mffc(const TgNode* node,
		  vector<bool>& mark,
		  vector<SaFault*>& flist)
{
  mark[node->gid()] = true;

  FaultInfo& fi = mFaultArray[node->gid()];
  for (vector<SaFault*>::iterator q = fi.mAllFaults.begin();
       q != fi.mAllFaults.end(); ++ q) {
    SaFault* f = *q;
    flist.push_back(f);
  }

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TgNode* inode = node->fanin(i);
    if ( mImmDomArray[inode->gid()] != NULL && !mark[inode->gid()] ) {
      dfs_mffc(inode, mark, flist);
    }
  }
}

const TgNode*
AtpgMgr::merge(const TgNode* node1,
	       const TgNode* node2,
	       const vector<ymuint32>& order)
{
  for ( ; ; ) {
    if ( node1 == node2 ) {
      return node1;
    }
    if ( node1 == NULL || node2 == NULL ) {
      return NULL;
    }
    ymuint id1 = order[node1->gid()];
    ymuint id2 = order[node2->gid()];
    if ( id1 < id2 ) {
      node1 = mImmDomArray[node1->gid()];
    }
    else if ( id1 > id2 ) {
      node2 = mImmDomArray[node2->gid()];
    }
  }
}

#endif

// @brief ファイル読み込みに関わる時間を得る．
USTime
AtpgMgr::read_time() const
{
  return mTimer.time(TM_READ);
}

// @brief DTPG に関わる時間を得る．
USTime
AtpgMgr::dtpg_time() const
{
  return mTimer.time(TM_DTPG);
}

// @brief 故障シミュレーションに関わる時間を得る．
USTime
AtpgMgr::fsim_time() const
{
  return mTimer.time(TM_FSIM);
}

// @brief SAT に関わる時間を得る．
USTime
AtpgMgr::sat_time() const
{
  return mTimer.time(TM_SAT);
}

// @brief その他の時間を得る．
USTime
AtpgMgr::misc_time() const
{
  return mTimer.time(TM_MISC);
}

// @brief 統計情報をクリアする．
void
AtpgMgr::clear_stats()
{
  mDtpg->clear_stats();
  mDtpgOld->clear_stats();
}

// @brief 統計情報を得る．
void
AtpgMgr::get_stats(DtpgStats& stats)
{
  mDtpg->get_stats(stats);
}

// @brief 統計情報を得る．
void
AtpgMgr::get_stats_old(DtpgStats& stats)
{
  mDtpgOld->get_stats(stats);
}

// @brief ネットワークをセットした後に呼ぶ関数
void
AtpgMgr::after_set_network()
{
  mFaultMgr.clear();
  mFaultMgr.set_ssa_fault(mNetwork);

  mTvMgr.clear();
  mTvMgr.init(mNetwork.input_num2());

  ymuint nn = mNetwork.node_num();
  mFaultArray.clear();
  mFaultArray.resize(nn);

  for (ymuint i = 0; i < nn; ++ i) {
    const TgNode* node = mNetwork.node(i);
    FaultInfo& fi = mFaultArray[i];
    fi.mOutputFaults[0] = NULL;
    fi.mOutputFaults[1] = NULL;
    ymuint ni = node->fanin_num();
    fi.mInputFaults.resize(ni * 2);
    for (ymuint j = 0; j < ni; ++ j) {
      fi.mInputFaults[j * 2 + 0] = NULL;
      fi.mInputFaults[j * 2 + 1] = NULL;
    }
  }

  const vector<SaFault*>& flist = mFaultMgr.remain_list();
  for (vector<SaFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    SaFault* f = *p;
    const TgNode* node = f->node();
    FaultInfo& fi = mFaultArray[node->gid()];
    if ( f->is_output_fault() ) {
      fi.mOutputFaults[f->val()] = f;
    }
    else { // f->is_input_fault()
      fi.mInputFaults[f->pos() * 2 + f->val()] = f;
    }
  }

  for (ymuint i = 0; i < nn; ++ i) {
    FaultInfo& fi = mFaultArray[i];
    ymuint nf = 0;
    if ( fi.mOutputFaults[0] != NULL ) {
      ++ nf;
    }
    if ( fi.mOutputFaults[1] != NULL ) {
      ++ nf;
    }
    ymuint n = fi.mInputFaults.size();
    for (ymuint j = 0; j < n; ++ j) {
      if ( fi.mInputFaults[j] != NULL ) {
	++ nf;
      }
    }
    if ( nf > 0 ) {
      fi.mAllFaults.reserve(nf);
    }
    for (ymuint j = 0; j < n; ++ j) {
      if ( fi.mInputFaults[j] != NULL ) {
	fi.mAllFaults.push_back(fi.mInputFaults[j]);
      }
    }
    if ( fi.mOutputFaults[0] != NULL ) {
      fi.mAllFaults.push_back(fi.mOutputFaults[0]);
    }
    if ( fi.mOutputFaults[1] != NULL ) {
      fi.mAllFaults.push_back(fi.mOutputFaults[1]);
    }
  }

  mDtpg->set_network(mNetwork, mFaultMgr.remain_list());
  mDtpgOld->set_network(mNetwork, mFaultMgr.remain_list());

  mNtwkBindMgr.prop_event(mNetwork, mFaultMgr.remain_list());
}

// @brief 故障リストが変更された時に呼ばれる関数
void
AtpgMgr::after_update_faults()
{
  mFaultMgr.update();
  mFaultBindMgr.prop_event(mFaultMgr.remain_list());
}

END_NAMESPACE_YM_SATPG
