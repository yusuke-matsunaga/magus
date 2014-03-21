#ifndef DTPGSAT2_H
#define DTPGSAT2_H

/// @file DtpgSat2.h
/// @brief DtpgSat2 のヘッダファイル
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
class DtpgSat2 :
  public Dtpg
{
public:

  /// @brief コンストラクタ
  DtpgSat2();

  /// @brief デストラクタ
  virtual
  ~DtpgSat2();


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
  /// @param[in] fault_mgr 故障マネージャ
  virtual
  void
  set_network(TpgNetwork& tgnetwork,
	      FaultMgr& fault_mgr);

  /// @brief テスト生成を行なう．
  /// @param[in] mode メインモード
  /// @param[in] po_mode PO分割モード
  /// @param[in] fault_analysis 故障の支配関係を解析する．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(DtpgMode mode,
      tDtpgPoMode po_mode,
      bool fault_analysis,
      BackTracer& bt,
      DetectOp& dop,
      UntestOp& uop,
      DtpgStats& stats);

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief activate された部分回路に大してテスト生成を行う．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  dtpg1(BackTracer& bt,
	DetectOp& dop,
	UntestOp& uop);

  /// @brief 一つの故障に対してテストパタン生成を行う．
  /// @param[in] f 故障
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  dtpg_single(TpgFault* f,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT エンジン
  SatEngine* mSatEngine;

  // 対象の回路
  TpgNetwork* mNetwork;

  // 故障マネージャ
  FaultMgr* mFaultMgr;

  // mNetwork のノード数
  ymuint32 mMaxId;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 時間計測を制御する．
inline
void
DtpgSat2::timer_enable(bool enable)
{
  mSatEngine->timer_enable(enable);
}

END_NAMESPACE_YM_SATPG

#endif // DTPGSAT2_H
