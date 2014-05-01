#ifndef ATPGMGR_H
#define ATPGMGR_H

/// @file AtpgMgr.h
/// @brief AtpgMgr のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "DtpgMode.h"
#include "cell/cell_nsdef.h"
#include "utils/Binder.h"
#include "utils/MStopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

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
  _fsim3();


public:
  //////////////////////////////////////////////////////////////////////
  // ネットワークを読み込む関数
  //////////////////////////////////////////////////////////////////////

  /// @brief blif 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] force_to_cplx_logic 組み込み型を使わない時に true にするフラグ
  /// @param[in] cell_library セルライブラリ
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_blif(const string& filename,
	    bool force_to_cplx_logic,
	    const CellLibrary* cell_library = NULL);

  /// @brief iscas89 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] force_to_cplx_logic 組み込み型を使わない時に true にするフラグ
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_iscas89(const string& filename,
	       bool force_to_cplx_logic);


public:
  //////////////////////////////////////////////////////////////////////
  // 故障シミュレーションを行なう関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 乱数生成器を初期化する．
  void
  rtpg_init(ymuint32 seed);

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
  /// @param[in] fault_analysis 故障の支配関係を解析する．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  /// @param[in] stats 結果を格納する構造体
  void
  dtpg(DtpgMode mode,
       tDtpgPoMode po_mode,
       bool fault_analysis,
       BackTracer& bt,
       DetectOp& dop,
       UntestOp& uop,
       DtpgStats& stats);


public:
  //////////////////////////////////////////////////////////////////////
  // テストパタン圧縮を行なう関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタン圧縮を行なう．
  void
  minpat(MinPatStats& stats);


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
  FaultMgr* mFaultMgr;

  // テストベクタを管理するオブジェクト
  TvMgr* mTvMgr;

  // テストベクタのリスト
  vector<TestVector*> mTvList;

  // 故障シミュレータ
  Fsim* mFsim;

  // 3値の故障シミュレータ
  Fsim* mFsim3;

  // RTPG
  Rtpg* mRtpg;

  // テストパタン生成器
  Dtpg* mDtpg;

  // テストパタン生成器
  Dtpg* mDtpg2;

  // パタン圧縮器
  MinPat* mMinPat;

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
  return *mFaultMgr;
}

// @brief TvMgr を取り出す．
inline
TvMgr&
AtpgMgr::_tv_mgr()
{
  return *mTvMgr;
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
AtpgMgr::_fsim3()
{
  return *mFsim3;
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
