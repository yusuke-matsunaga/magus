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
#include "SatEngineSingle2.h"
#include "SatEngineMulti2.h"


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

  /// @brief テスト生成を行なう．
  /// @param[in] network 対象のネットワーク
  /// @param[in] mode メインモード
  /// @param[in] po_mode PO分割モード
  /// @param[in] option_str オプション文字列
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& network,
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
	      ymuint th_val,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop,
	      DtpgStats& stats);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  mffc_mode(TpgNetwork& tpgnetwork,
	    ymuint th_val,
	    BackTracer& bt,
	    DetectOp& dop,
	    UntestOp& uop,
	    DtpgStats& stats);

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
  do_dtpg(TpgNetwork& network,
	  ymuint th_val,
	  BackTracer& bt,
	  DetectOp& dop,
	  UntestOp& uop);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT エンジン
  SatEngineSingle2 mSatEngineSingle;

  SatEngineMulti2 mSatEngineMulti;

  // do_dtpg() で用いる対象の故障リスト
  vector<TpgFault*> mFaultList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 時間計測を制御する．
inline
void
DtpgSat2::timer_enable(bool enable)
{
  mSatEngineSingle.timer_enable(enable);
  mSatEngineMulti.timer_enable(enable);
}

// @brief 故障リストをクリアする．
inline
void
DtpgSat2::clear_faults()
{
  mFaultList.clear();
}

END_NAMESPACE_YM_SATPG

#endif // DTPGSAT2_H
