#ifndef ATPGMGR_H
#define ATPGMGR_H

/// @file include/AtpgMgr.h
/// @brief AtpgMgr のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "FaultMgr.h"
#include "TvMgr.h"
#include "Fsim.h"
#include "Dtpg.h"
#include "ym_networks/TgNetwork.h"
#include "ym_cell/cell_nsdef.h"
#include "ym_utils/Binder.h"
#include "ym_utils/MStopWatch.h"


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
  const TgNetwork&
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

  /// @brief 統計情報をクリアする．
  void
  clear_stats();

  /// @brief 統計情報を得る．
  void
  get_stats();


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

  /// @brief 一つのテストベクタに対する故障シミュレーションを行なう．
  /// @param[in] tv テストベクタ
  /// @param[out] det_faults 検出された故障を格納するリスト
  void
  fsim(TestVector* tv,
       vector<SaFault*>& det_faults);

  /// @brief 複数のテストベクタに対する故障シミュレーションを行なう．
  /// @param[in] tv_list テストベクタのリスト
  /// @param[out] det_faults 検出された故障を格納するリストのリスト
  void
  fsim(const vector<TestVector*>& tv_list,
       vector<vector<SaFault*> >& det_faults_list);

  /// @brief 一つのパタンで一つの故障に対するシミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  /// @retval true tv で f の検出ができた．
  /// @retval false tv では f の検出ができない．
  bool
  fsim(TestVector* tv,
       SaFault* f);


public:
  //////////////////////////////////////////////////////////////////////
  // DTPG を行なう関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  void
  set_dtpg_mode(const string& type = string(),
		const string& option = string(),
		ostream* outp = NULL);

  /// @brief テストパタン生成を行なう．
  void
  dtpg(const string& option);


public:
  //////////////////////////////////////////////////////////////////////
  // イベントハンドラの登録
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークの変更に関するハンドラを登録する．
  void
  reg_network_handler(T2Binder<const TgNetwork&, const vector<SaFault*>&>* handler);

  /// @brief 故障リストの変更に関するハンドラを登録する．
  void
  reg_fault_handler(T1Binder<const vector<SaFault*>& >* handler);


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

  /// @brief 故障リストが変更された時に呼ばれる関数
  void
  after_update_faults();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  TgNetwork mNetwork;

  // 故障リスト
  FaultMgr mFaultMgr;

  // テストベクタを管理するオブジェクト
  TvMgr mTvMgr;

  // テストベクタのリスト
  vector<TestVector*> mTvList;

  // 故障シミュレータ
  Fsim* mFsim;

  // テストパタン生成器
  Dtpg* mDtpg;

  // ネットワークが変更された時に呼ばれるイベントハンドラ
  T2BindMgr<const TgNetwork&, const vector<SaFault*>&> mNtwkBindMgr;

  // 故障リストが変更された時に呼ばれるイベントハンドラ
  T1BindMgr<const vector<SaFault*>& > mFaultBindMgr;

  // タイマー
  MStopWatch mTimer;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief TgNetwork を取り出す．
inline
const TgNetwork&
AtpgMgr::_network()
{
  return mNetwork;
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

// @brief ネットワークの変更に関するハンドラを登録する．
inline
void
AtpgMgr::reg_network_handler(T2Binder<const TgNetwork&, const vector<SaFault*>&>* handler)
{
  mNtwkBindMgr.reg_binder(handler);
}

END_NAMESPACE_YM_SATPG

#endif // ATPGMGR_H
