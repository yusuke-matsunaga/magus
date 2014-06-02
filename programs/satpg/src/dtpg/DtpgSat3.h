#ifndef DTPGSAT3_H
#define DTPGSAT3_H

/// @file DtpgSat3.h
/// @brief DtpgSat3 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dtpg.h"
#include "TpgFault.h"
#include "SatEngineSingle.h"
#include "SatEngineMulti.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSat3 DtpgSat3.h "DtpgSat3.h"
/// @brief SAT を用いたパタン生成器
//////////////////////////////////////////////////////////////////////
class DtpgSat3 :
  public Dtpg
{
public:

  /// @brief コンストラクタ
  DtpgSat3();

  /// @brief デストラクタ
  virtual
  ~DtpgSat3();


public:
  //////////////////////////////////////////////////////////////////////
  // Dtpg の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  virtual
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL);

  /// @brief テスト生成を行なう．
  /// @param[in] tpgnetwork 対象のネットワーク
  /// @param[in] mode メインモード
  /// @param[in] po_mode PO分割モード
  /// @param[in] option_str オプション文字列
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& tgnetwork,
      DtpgMode mode,
      tDtpgPoMode po_mode,
      const string& option_str,
      BackTracer& bt,
      DetectOp& dop,
      UntestOp& uop,
      DtpgStats& stats);

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable);


public:
  //////////////////////////////////////////////////////////////////////
  // 一つの故障に対する処理が終わったときに呼ばれる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった場合に呼ばれる関数
  /// @param[in] f 故障
  /// @param[in] tv テストパタン
  void
  set_detected(TpgFault* f,
	       TestVector* tv);

  /// @brief 検出不能のときに呼ばれる関数
  /// @param[in] f 故障
  void
  set_untestable(TpgFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  single_mode(TpgNetwork& network,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  ffr_mode(TpgNetwork& network,
	   BackTracer& bt,
	   DetectOp& dop,
	   UntestOp& uop);

  /// @brief 一つの故障に対してテストパタン生成を行う．
  /// @param[in] f 故障
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  dtpg_single1(TpgNetwork& network,
	       TpgFault* f,
	       BackTracer& bt,
	       DetectOp& dop,
	       UntestOp& uop);

  /// @brief 一つの故障に対してテストパタン生成を行う．
  /// @param[in] f 故障
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  dtpg_single2(TpgNetwork& network,
	       TpgFault* f,
	       BackTracer& bt,
	       DetectOp& dop,
	       UntestOp& uop);

  /// @brief DFS で FFR を求める．
  void
  dfs_ffr(TpgNode* node);

  /// @brief 故障リストをクリアする．
  void
  clear_faults();

  /// @brief ノードの故障を追加する．
  void
  add_node_faults(TpgNode* node);

  /// @brief 故障を追加する．
  void
  add_fault(TpgFault* fault);

  /// @brief テストパタン生成を行なう．
  void
  do_dtpg(TpgNetwork& network,
	  BackTracer& bt,
	  DetectOp& dop,
	  UntestOp& uop);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT エンジン
  SatEngineSingle mSatEngineS;

  SatEngineMulti mSatEngineM;

  // do_dtpg() で用いる対象の故障リスト
  vector<TpgFault*> mFaultList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 時間計測を制御する．
inline
void
DtpgSat3::timer_enable(bool enable)
{
  mSatEngineS.timer_enable(enable);
  mSatEngineM.timer_enable(enable);
}

// @brief 故障リストをクリアする．
inline
void
DtpgSat3::clear_faults()
{
  mFaultList.clear();
}

// @brief 故障を追加する．
inline
void
DtpgSat3::add_fault(TpgFault* fault)
{
  if ( fault != NULL &&
       fault->is_rep() &&
       fault->status() != kFsDetected &&
       !fault->is_skip() ) {
    mFaultList.push_back(fault);
  }
}

END_NAMESPACE_YM_SATPG

#endif // DTPGSAT3_H
