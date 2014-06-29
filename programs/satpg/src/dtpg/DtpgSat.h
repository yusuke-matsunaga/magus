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
#include "SatEngineMulti.h"
#include "SatEngineSingle.h"


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

  /// @brief activate された部分回路に大してテスト生成を行う．
  /// @param[in] mode メインモード
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  dtpg1(DtpgMode mode,
	BackTracer& bt,
	DetectOp& dop,
	UntestOp& uop);

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  single_mode(BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  ffr_mode(BackTracer& bt,
	   DetectOp& dop,
	   UntestOp& uop);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  mffc_mode(BackTracer& bt,
	    DetectOp& dop,
	    UntestOp& uop);

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

  /// @brief テストパタン生成を行なう．
  void
  do_dtpg(BackTracer& bt,
	  DetectOp& dop,
	  UntestOp& uop);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT エンジン
  SatEngineMulti mSatEngine;

  SatEngineSingle mSatEngineSingle;

  // 対象の回路
  TpgNetwork* mNetwork;

  // do_dtpg() で用いる対象の故障リスト
  vector<TpgFault*> mFaultList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 時間計測を制御する．
inline
void
DtpgSat::timer_enable(bool enable)
{
  mSatEngine.timer_enable(enable);
  mSatEngineSingle.timer_enable(enable);
}

// @brief 故障リストをクリアする．
inline
void
DtpgSat::clear_faults()
{
  mFaultList.clear();
}

END_NAMESPACE_YM_SATPG

#endif // DTPGSAT_H
