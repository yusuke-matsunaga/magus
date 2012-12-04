
/// @file src/main/AtpgMgr.cc
/// @brief AtpgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AtpgMgr.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgMgr.h"
#include "fsim/Fsim.h"
#include "dtpg/DtpgSat.h"
#include "ym_networks/TgBlifReader.h"
#include "ym_networks/TgIscas89Reader.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FsimNetBinder
//////////////////////////////////////////////////////////////////////
class FsimNetBinder :
  public T1Binder<const TgNetwork&>
{
public:

  /// @brief コンストラクタ
  FsimNetBinder(Fsim* fsim);

  /// @brief イベント処理関数
  virtual
  void
  event_proc(const TgNetwork& network);


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
FsimNetBinder::event_proc(const TgNetwork& network)
{
  mFsim->set_network(network);
}


//////////////////////////////////////////////////////////////////////
// AtpgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AtpgMgr::AtpgMgr() :
  mTimer(TM_SIZE, TM_MISC)
{
  mFsim = new_Fsim();

  mDtpg = new DtpgSat();

  reg_network_handler(new FsimNetBinder(mFsim));

  set_dtpg_mode();
}

// @brief デストラクタ
AtpgMgr::~AtpgMgr()
{
  delete mFsim;
  delete mDtpg;
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
    mFaultMgr.update();
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
    mFaultMgr.update();
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
      mFaultMgr.update();
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
}

// @brief 一つの故障に対してテストパタン生成を行なう．
// @param[in] fnode 故障ノード
// @param[in] fval 故障値
// @param[in] tv 生成したパタンを入れるベクタ
// @retval kDetect パタン生成が成功した．
// @retval kUntest テスト不能故障だった．
// @retval kAbort アボートした．
tStat
AtpgMgr::dtpg_single(const TgNode* fnode,
		     int fval,
		     TestVector* tv)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_SAT);

  tStat stat = mDtpg->dtpg_single(_network(), fnode, false, 0, fnode, fval, tv);

  mTimer.change(old_id);

  return stat;
}

// @brief 一つの故障に対してテストパタン生成を行なう．
// @param[in] fnode 故障ノード
// @param[in] ipos 故障の入力位置
// @param[in] fsrc 故障ノードの入力
// @param[in] fval 故障値
// @param[in] tv 生成したパタンを入れるベクタ
// @retval kDetect パタン生成が成功した．
// @retval kUntest テスト不能故障だった．
// @retval kAbort アボートした．
tStat
AtpgMgr::dtpg_single(const TgNode* fnode,
		     ymuint ipos,
		     const TgNode* fsrc,
		     int fval,
		     TestVector* tv)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_SAT);

  tStat stat = mDtpg->dtpg_single(_network(), fnode, true, ipos, fsrc, fval, tv);

  mTimer.change(old_id);

  return stat;
}

// @brief 同じ位置の2つの故障に対してテストパタン生成を行なう．
// @param[in] fnode 故障ノード
// @param[in] tv 生成したパタンを入れるベクタ
// @retval kDetect パタン生成が成功した．
// @retval kUntest テスト不能故障だった．
// @retval kAbort アボートした．
pair<tStat, tStat>
AtpgMgr::dtpg_dual(const TgNode* fnode,
		   TestVector* tv[])
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_SAT);

  pair<tStat, tStat> stats = mDtpg->dtpg_dual(_network(), fnode, false, 0, fnode, tv);

  mTimer.change(old_id);

  return stats;
}

// @brief 同じ位置の2つの故障に対してテストパタン生成を行なう．
// @param[in] fnode 故障ノード
// @param[in] ipos 故障の入力位置
// @param[in] fsrc 故障ノードの入力
// @param[in] tv 生成したパタンを入れるベクタ
// @retval kDetect パタン生成が成功した．
// @retval kUntest テスト不能故障だった．
// @retval kAbort アボートした．
pair<tStat, tStat>
AtpgMgr::dtpg_dual(const TgNode* fnode,
		   ymuint ipos,
		   const TgNode* fsrc,
		   TestVector* tv[])
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_SAT);

  pair<tStat, tStat> stats = mDtpg->dtpg_dual(_network(), fnode, true, ipos, fsrc, tv);

  mTimer.change(old_id);

  return stats;
}

// @brief FFR 内の故障に対してテストパタン生成を行なう．
// @param[in] root FFR の根のノード
// @param[in] flist 故障リスト
// @param[in] tv_list 生成したパタンを入れるベクタ
// @param[in] stat_list 結果を入れるベクタ
// @note flist の故障は必ず root が dominator となっていなければならない．
void
AtpgMgr::dtpg_ffr(const TgNode* root,
		  const vector<SaFault*>& flist,
		  vector<TestVector*>& tv_list,
		  vector<tStat>& stat_list)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_SAT);

  mDtpg->dtpg_ffr(_network(), root, flist, tv_list, stat_list);

  mTimer.change(old_id);
}

// @brief 直前の実行結果を得る．
const SatStats&
AtpgMgr::dtpg_stats() const
{
  return mDtpg->stats();
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

// @brief ネットワークをセットした後に呼ぶ関数
void
AtpgMgr::after_set_network()
{
  mFaultMgr.clear();
  mTvMgr.clear();

  ymuint ni = mNetwork.input_num2();
  mTvMgr.init(ni);

  mNtwkBindMgr.prop_event(mNetwork);
}

// @brief 故障リストが変更された時に呼ばれる関数
void
AtpgMgr::after_update_faults()
{
  mFaultBindMgr.prop_event(mFaultMgr.remain_list());
}

END_NAMESPACE_YM_SATPG
