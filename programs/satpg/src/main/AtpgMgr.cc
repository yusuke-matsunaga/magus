
/// @file src/main/AtpgMgr.cc
/// @brief AtpgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgMgr.h"
#include "RtpgStats.h"
#include "TpgNetwork.h"
#include "TpgFault.h"
#include "BackTracer.h"
#include "DetectOp.h"
#include "UntestOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FsimNetBinder
//////////////////////////////////////////////////////////////////////
class FsimNetBinder :
  public T2Binder<const TpgNetwork&, FaultMgr&>
{
public:

  /// @brief コンストラクタ
  FsimNetBinder(Fsim* fsim);

  /// @brief イベント処理関数
  virtual
  void
  event_proc(const TpgNetwork& network,
	     FaultMgr& fault_mgr);


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
FsimNetBinder::event_proc(const TpgNetwork& network,
			  FaultMgr& fault_mgr)
{
  mFsim->set_network(network, fault_mgr);
}


//////////////////////////////////////////////////////////////////////
// AtpgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AtpgMgr::AtpgMgr() :
  mTimer(TM_SIZE, TM_MISC)
{
  mFsim = new_Fsim2();
  mFsim3 = new_FsimX2();

  mDtpg = new_DtpgSat();

  mNetwork = NULL;

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
  delete mNetwork;
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

  delete mNetwork;
  mNetwork = TpgNetwork::read_blif(filename, cell_library);
  if ( mNetwork != NULL ) {
    after_set_network();
  }

  mTimer.change(old_id);

  return mNetwork != NULL;
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

  delete mNetwork;
  mNetwork = TpgNetwork::read_iscas89(filename);
  if ( mNetwork != NULL ) {
    after_set_network();
  }

  mTimer.change(old_id);

  return mNetwork != NULL;
}

// @brief 乱数パタンを用いた故障シミュレーションを行なう．
// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
// @param[in] max_i 故障検出できないシミュレーション回数の上限
// @param[in] max_pat 最大のパタン数
// @param[in] stats 実行結果の情報を格納する変数
void
AtpgMgr::rtpg(ymuint min_f,
	      ymuint max_i,
	      ymuint max_pat,
	      RtpgStats& stats)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_FSIM);

  StopWatch local_timer;
  local_timer.start();

  ymuint fnum = mFaultMgr.remain_num();
  ymuint undet_i = 0;
  ymuint epat_num = 0;
  ymuint total_det_count = 0;

  TestVector* tv_array[kPvBitLen];
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    tv_array[i] = mTvMgr.new_vector();
  }

  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);
  vector<vector<TpgFault*> > det_faults(kPvBitLen);

  ymuint pat_num = 0;
  for ( ; ; ) {
    if ( pat_num < max_pat ) {
      TestVector* tv = tv_array[cur_array.size()];
      tv->set_from_random(mPatGen);
      cur_array.push_back(tv);
      ++ pat_num;
      if ( cur_array.size() < kPvBitLen ) {
	continue;
      }
    }
    else if ( cur_array.empty() ) {
      break;
    }

    mFsim->run(cur_array, det_faults);

    ymuint det_count = 0;
    for (ymuint i = 0; i < cur_array.size(); ++ i) {
      ymuint det_count1 = det_faults[i].size();
      if ( det_count1 ) {
	det_count += det_count1;
	TestVector* tv = cur_array[i];
	mTvList.push_back(tv);
	tv_array[i] = mTvMgr.new_vector();
	++ epat_num;
	for (vector<TpgFault*>::iterator p = det_faults[i].begin();
	     p != det_faults[i].end(); ++ p) {
	  TpgFault* f = *p;
	  if ( f->status() == kFsUndetected ) {
	    mFaultMgr.set_status(f, kFsDetected);
	  }
	}
      }
    }
    cur_array.clear();

    mFaultMgr.update();

    total_det_count += det_count;

    if ( total_det_count == fnum ) {
      // 全ての故障を検出した．
      break;
    }
    if ( det_count < min_f ) {
      // 一回の故障検出数が下限を下回った．
      break;
    }
    if ( det_count > 0 ) {
      undet_i = 0;
    }
    else {
      ++ undet_i;
      if ( undet_i > max_i ) {
	// 未検出の回数が上限を越えた．
	break;
      }
    }
  }

  // 後始末
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    mTvMgr.delete_vector(tv_array[i]);
  }

  stats.mDetectNum = total_det_count;
  stats.mPatNum = pat_num;
  stats.mEfPatNum = epat_num;
  stats.mTime = local_timer.time();

  mTimer.change(old_id);
}

// @brief 一つのテストベクタに対する故障シミュレーションを行なう．
// @param[in] tv テストベクタ
// @param[out] det_faults 検出された故障を格納するリスト
void
AtpgMgr::fsim(TestVector* tv,
	      vector<TpgFault*>& det_faults)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_FSIM);

  mFsim->run(tv, det_faults);

  ymuint det_count = det_faults.size();
  if ( det_count > 0 ) {
    for (vector<TpgFault*>::const_iterator p = det_faults.begin();
	 p != det_faults.end(); ++ p) {
      TpgFault* f = *p;
      if ( f->status() == kFsUndetected ) {
	mFaultMgr.set_status(f, kFsDetected);
      }
    }
  }

  mTimer.change(old_id);
}

// @brief 複数のテストベクタに対する故障シミュレーションを行なう．
// @param[in] tv_list テストベクタのリスト
// @param[out] det_faults 検出された故障を格納するリストのリスト
void
AtpgMgr::fsim(const vector<TestVector*>& tv_list,
	      vector<vector<TpgFault*> >& det_faults_list)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_FSIM);

  mFsim->run(tv_list, det_faults_list);

  ymuint det_count = 0;
  for (ymuint i = 0; i < tv_list.size(); ++ i) {
    const vector<TpgFault*>& det_faults = det_faults_list[i];
    ymuint det_count1 = det_faults.size();
    if ( det_count1 > 0 ) {
      det_count += det_count1;
      for (vector<TpgFault*>::const_iterator p = det_faults.begin();
	   p != det_faults.end(); ++ p) {
	TpgFault* f = *p;
	if ( f->status() == kFsUndetected ) {
	  mFaultMgr.set_status(f, kFsDetected);
	}
      }
    }
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
	      TpgFault* f)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_FSIM);

  bool stat = mFsim->run(tv, f);
  if ( stat ) {
    if ( f->status() == kFsUndetected ) {
      mFaultMgr.set_status(f, kFsDetected);
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
}

// @brief テストパタン生成時に時間計測を行なうかどうかを指定する．
void
AtpgMgr::set_dtpg_timer(bool enable)
{
  mDtpg->timer_enable(enable);
}

// @brief テストパタン生成を行なう．
void
AtpgMgr::dtpg(tDtpgMode mode,
	      tDtpgPoMode po_mode,
	      BackTracer& bt,
	      const vector<DetectOp*>& dop_list,
	      const vector<UntestOp*>& uop_list)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_DTPG);

  mDtpg->run(mode, po_mode, bt, dop_list, uop_list);

  mTimer.change(old_id);
}

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
}

// @brief 統計情報を得る．
void
AtpgMgr::get_stats(DtpgStats& stats)
{
  mDtpg->get_stats(stats);
}

// @brief ネットワークをセットした後に呼ぶ関数
void
AtpgMgr::after_set_network()
{
  mFaultMgr.set_ssa_fault(*mNetwork);

  mTvMgr.clear();
  mTvMgr.init(mNetwork->input_num2());

  mDtpg->set_network(*mNetwork);

  mNtwkBindMgr.prop_event(*mNetwork, mFaultMgr);
}

END_NAMESPACE_YM_SATPG
