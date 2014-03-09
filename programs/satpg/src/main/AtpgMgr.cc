
/// @file AtpgMgr.cc
/// @brief AtpgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgMgr.h"
#include "RtpgStats.h"
#include "TpgNetwork.h"
#include "TpgFault.h"
#include "FaultMgr.h"
#include "TvMgr.h"
#include "Fsim.h"
#include "Dtpg.h"
#include "Rtpg.h"
#include "MinPat.h"
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
  mFaultMgr = new FaultMgr();
  mTvMgr = new TvMgr();

  mFsim = new_Fsim2();
  mFsim3 = new_Fsim3();

  mRtpg = new_Rtpg(*this);
  mDtpg = new_DtpgSat();
  mMinPat = new_MinPat(*this);

  mNetwork = NULL;

  reg_network_handler(new FsimNetBinder(mFsim));
  reg_network_handler(new FsimNetBinder(mFsim3));

  set_dtpg_mode();
}

// @brief デストラクタ
AtpgMgr::~AtpgMgr()
{
  delete mFaultMgr;
  delete mTvMgr;
  delete mFsim;
  delete mFsim3;
  delete mRtpg;
  delete mDtpg;
  delete mMinPat;
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

// @brief 乱数生成器を初期化する．
void
AtpgMgr::rtpg_init(ymuint32 seed)
{
  mRtpg->init(seed);
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

  mRtpg->run(min_f, max_i, max_pat, stats);

  mTimer.change(old_id);
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
AtpgMgr::dtpg(DtpgMode mode,
	      tDtpgPoMode po_mode,
	      bool fault_analysis,
	      BackTracer& bt,
	      const vector<DetectOp*>& dop_list,
	      const vector<UntestOp*>& uop_list,
	      DtpgStats& stats)
{
  ymuint old_id = mTimer.cur_id();
  mTimer.change(TM_DTPG);

  mDtpg->run(mode, po_mode, fault_analysis, bt, dop_list, uop_list, stats);

  mTimer.change(old_id);
}

// @brief テストパタン圧縮を行なう．
void
AtpgMgr::minpat(MinPatStats& stats)
{
  mMinPat->run(_tv_list(), stats);
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
  mFaultMgr->set_ssa_fault(*mNetwork);

  mTvMgr->clear();
  mTvMgr->init(mNetwork->input_num2());

  mDtpg->set_network(*mNetwork, *mFaultMgr);

  mNtwkBindMgr.prop_event(*mNetwork, *mFaultMgr);
}

END_NAMESPACE_YM_SATPG
