#ifndef DTPGSAT_H
#define DTPGSAT_H

/// @file DtpgSat.h
/// @brief DtpgSat のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dtpg.h"
#include "TpgFault.h"
#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSat DtpgSat.h "DtpgSat.h"
/// @brief SAT を用いたパタン生成器
//////////////////////////////////////////////////////////////////////
class DtpgSat :
  public Dtpg
{
public:

  /// @brief コンストラクタ
  DtpgSat();

  /// @brief デストラクタ
  virtual
  ~DtpgSat();


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

  /// @brief 回路と故障リストを設定する．
  /// @param[in] tpgnetwork 対象のネットワーク
  virtual
  void
  set_network(TpgNetwork& tgnetwork);

  /// @brief テスト生成を行なう．
  /// @param[in] mode メインモード
  /// @param[in] po_mode PO分割モード
  /// @param[in] fault_analysis 故障の支配関係を解析する．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop_list DetectOp のリスト
  /// @param[in] uop_list UntestOp のリスト
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(DtpgMode mode,
      tDtpgPoMode po_mode,
      bool fault_analysis,
      BackTracer& bt,
      const vector<DetectOp*>& dop_list,
      const vector<UntestOp*>& uop_list,
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

  /// @brief MFFC 単位で故障差関数を作り，支配関係をもとにした順序付けを行う．
  void
  fault_analyze();

  /// @brief activate された部分回路に大してテスト生成を行う．
  /// @param[in] mode メインモード
  /// @param[in] bt バックトレーサー
  void
  dtpg1(DtpgMode mode,
	BackTracer& bt);

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  void
  single_mode(BackTracer& bt);

  /// @brief dual モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  void
  dual_mode(BackTracer& bt);

  /// @brief node モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  void
  node_mode(BackTracer& bt);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  void
  ffr_mode(BackTracer& bt);

  /// @brief ffr2 モードでテスト生成を行なう．
  /// @param[in] size_limit サイズの上限
  /// @param[in] bt バックトレーサー
  void
  ffr2_mode(ymuint size_limit,
	    BackTracer& bt);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  void
  mffc_mode(BackTracer& bt);

  /// @brief all モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  void
  all_mode(BackTracer& bt);

  /// @brief single モードの共通処理
  void
  single_sub(BackTracer& bt);

  /// @brief dual モードの共通処理
  void
  dual_sub(BackTracer& bt);

  /// @brief ffr モードの共通処理
  void
  ffr_sub(BackTracer& bt);

  /// @brief mffc モードの共通処理
  void
  mffc_sub(BackTracer& bt);

  /// @brief all モードの共通処理
  void
  all_sub(BackTracer& bt);

  /// @brief 一つの故障に対してテストパタン生成を行う．
  /// @param[in] f 故障
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  dtpg_single(TpgFault* f,
	      BackTracer& bt);

  /// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
  /// @param[in] f0 0縮退故障
  /// @param[in] f1 1縮退故障
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  dtpg_dual(TpgFault* f0,
	    TpgFault* f1,
	    BackTracer& bt);

  /// @brief DFS で FFR を求める．
  void
  dfs_ffr(TpgNode* node);

  /// @brief DFS で MFFC を求める．
  void
  dfs_mffc(TpgNode* node,
	   vector<bool>& mark);

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
  do_dtpg(BackTracer& bt);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT エンジン
  SatEngine* mSatEngine;

  // 対象の回路
  TpgNetwork* mNetwork;

  // mNetwork のノード数
  ymuint32 mMaxId;

  // do_dtpg() で用いる対象の故障リスト
  vector<TpgFault*> mFaultList;

  // テストパタンが求められたときに実行するファンクタのリスト
  vector<DetectOp*> mDetectOpList;

  // 検出不能と判定されたときに実行するファンクタのリスト
  vector<UntestOp*> mUntestOpList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 時間計測を制御する．
inline
void
DtpgSat::timer_enable(bool enable)
{
  mSatEngine->timer_enable(enable);
}

// @brief 故障リストをクリアする．
inline
void
DtpgSat::clear_faults()
{
  mFaultList.clear();
}

// @brief 故障を追加する．
inline
void
DtpgSat::add_fault(TpgFault* fault)
{
  if ( fault != NULL &&
       fault->is_rep() &&
       fault->status() != kFsDetected &&
       !fault->is_skip() ) {
    mFaultList.push_back(fault);
  }
}

// @brief テストパタン生成を行なう．
inline
void
DtpgSat::do_dtpg(BackTracer& bt)
{
  if ( !mFaultList.empty() ) {
    mSatEngine->run(mFaultList, mMaxId, bt);
  }
}

END_NAMESPACE_YM_SATPG

#endif // DTPGSAT_H
