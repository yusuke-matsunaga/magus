#ifndef ATPGMGR_H
#define ATPGMGR_H

/// @file include/AtpgMgr.h
/// @brief AtpgMgr のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"

#include "FaultMgr.h"
#include "TvMgr.h"
#include "Fsim.h"
#include "FsimOld.h"
#include "Dtpg.h"
#include "ym_cell/cell_nsdef.h"
#include "ym_utils/Binder.h"
#include "ym_utils/MStopWatch.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

class RtpgStats;

//////////////////////////////////////////////////////////////////////
/// @class AtpgMgr AtpgMgr.h "AtpgMgr.h"
/// @brief ATPG に共通のデータを保持しておくためのクラス
//////////////////////////////////////////////////////////////////////
class AtpgMgr
{
public:

  /// @brief コンストラクタ
  AtpgMgr();

  /// @brief デストラクタ
  ~AtpgMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief TgNetwork を取り出す．
  TpgNetwork&
  _network();

  /// @brief FaultMgr を取り出す．
  FaultMgr&
  _fault_mgr();

  /// @brief TvMgr を取り出す．
  TvMgr&
  _tv_mgr();

  /// @brief テストベクタのリストを取り出す．
  vector<TestVector*>&
  _tv_list();

  /// @brief 2値の故障シミュレータを取り出す．
  Fsim&
  _fsim();

  /// @brief 3値の故障シミュレータを返す．
  Fsim&
  _fsimx();

  /// @brief 2値の故障シミュレータを取り出す．
  FsimOld&
  _fsimold();

  /// @brief 3値の故障シミュレータを返す．
  FsimOld&
  _fsimoldx();

  /// @brief 統計情報をクリアする．
  void
  clear_stats();

  /// @brief 統計情報を得る．
  /// @param[in] stats 結果を格納する構造体
  void
  get_stats(DtpgStats& stats);


public:
  //////////////////////////////////////////////////////////////////////
  // ネットワークを読み込む関数
  //////////////////////////////////////////////////////////////////////

  /// @brief blif 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] cell_library セルライブラリ
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_blif(const string& filename,
	    const CellLibrary* cell_library = NULL);

  /// @brief iscas89 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_iscas89(const string& filename);


public:
  //////////////////////////////////////////////////////////////////////
  // 故障シミュレーションを行なう関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 乱数パタンを用いた故障シミュレーションを行なう．
  /// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
  /// @param[in] max_i 故障検出できないシミュレーション回数の上限
  /// @param[in] max_pat 最大のパタン数
  /// @param[in] stats 実行結果の情報を格納する変数
  void
  rtpg(ymuint min_f,
       ymuint max_i,
       ymuint max_pat,
       RtpgStats& stats);


public:
  //////////////////////////////////////////////////////////////////////
  // DTPG を行なう関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  void
  set_dtpg_mode(const string& type = string(),
		const string& option = string(),
		ostream* outp = NULL);

  /// @brief テストパタン生成時に時間計測を行なうかどうかを指定する．
  void
  set_dtpg_timer(bool enable);

  /// @brief テストパタン生成を行なう．
  /// @param[in] mode メインモード
  /// @param[in] po_mode PO分割モード
  /// @param[in] bt バックトレーサー
  /// @param[in] dop_list DetectOp のリスト
  /// @param[in] uop_list UntestOp のリスト
  void
  dtpg(tDtpgMode mode,
       tDtpgPoMode po_mode,
       BackTracer& bt,
       const vector<DetectOp*>& dop_list,
       const vector<UntestOp*>& uop_list);


public:
  //////////////////////////////////////////////////////////////////////
  // イベントハンドラの登録
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークの変更に関するハンドラを登録する．
  void
  reg_network_handler(T2Binder<const TpgNetwork&, FaultMgr&>* handler);


public:
  //////////////////////////////////////////////////////////////////////
  // タイマー関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル読み込みに関わる時間を得る．
  USTime
  read_time() const;

  /// @brief DTPG に関わる時間を得る．
  USTime
  dtpg_time() const;

  /// @brief 故障シミュレーションに関わる時間を得る．
  USTime
  fsim_time() const;

  /// @brief SAT に関わる時間を得る．
  USTime
  sat_time() const;

  /// @brief その他の時間を得る．
  USTime
  misc_time() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークが変更された時に呼ばれる関数
  void
  after_set_network();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  TpgNetwork* mNetwork;

  // 故障マネージャ
  FaultMgr mFaultMgr;

  // テストベクタを管理するオブジェクト
  TvMgr mTvMgr;

  // テストベクタのリスト
  vector<TestVector*> mTvList;

  // RTPG 用の乱数発生器
  RandGen mPatGen;

  // 故障シミュレータ
  Fsim* mFsim;

  // 3値の故障シミュレータ
  Fsim* mFsim3;

  // 故障シミュレータ
  FsimOld* mFsimOld;

  // 3値の故障シミュレータ
  FsimOld* mFsimOld3;

  // テストパタン生成器
  Dtpg* mDtpg;

  // ネットワークが変更された時に呼ばれるイベントハンドラ
  T2BindMgr<const TpgNetwork&, FaultMgr&> mNtwkBindMgr;

  // タイマー
  MStopWatch mTimer;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief TgNetwork を取り出す．
inline
TpgNetwork&
AtpgMgr::_network()
{
  return *mNetwork;
}

// @brief FaultMgr を取り出す．
inline
FaultMgr&
AtpgMgr::_fault_mgr()
{
  return mFaultMgr;
}

// @brief TvMgr を取り出す．
inline
TvMgr&
AtpgMgr::_tv_mgr()
{
  return mTvMgr;
}

// @brief テストベクタのリストを取り出す．
inline
vector<TestVector*>&
AtpgMgr::_tv_list()
{
  return mTvList;
}

// @brief 2値の故障シミュレータを取り出す．
inline
Fsim&
AtpgMgr::_fsim()
{
  return *mFsim;
}

// @brief 3値の故障シミュレータを返す．
inline
Fsim&
AtpgMgr::_fsimx()
{
  return *mFsim3;
}

// @brief 2値の故障シミュレータを取り出す．
inline
FsimOld&
AtpgMgr::_fsimold()
{
  return *mFsimOld;
}

// @brief 3値の故障シミュレータを返す．
inline
FsimOld&
AtpgMgr::_fsimoldx()
{
  return *mFsimOld3;
}

// @brief ネットワークの変更に関するハンドラを登録する．
inline
void
AtpgMgr::reg_network_handler(T2Binder<const TpgNetwork&, FaultMgr&>* handler)
{
  mNtwkBindMgr.reg_binder(handler);
}

END_NAMESPACE_YM_SATPG

#endif // ATPGMGR_H
