#ifndef DTPGSMT_H
#define DTPGSMT_H

/// @file DtpgSmt.h
/// @brief DtpgSmt のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dtpg.h"
#include "TpgFault.h"
#include "SatEngineMulti.h"
#include "SmtEngineSingle.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSmt DtpgSmt.h "DtpgSmt.h"
/// @brief SAT を用いたパタン生成器
//////////////////////////////////////////////////////////////////////
class DtpgSmt :
  public Dtpg
{
public:

  /// @brief コンストラクタ
  DtpgSmt();

  /// @brief デストラクタ
  virtual
  ~DtpgSmt();


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
  do_dtpg(BackTracer& bt,
	  DetectOp& dop,
	  UntestOp& uop);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  SmtEngineSingle mSatEngineSingle;

  // 対象の回路
  TpgNetwork* mNetwork;

  // mNetwork のノード数
  ymuint32 mMaxId;

  // do_dtpg() で用いる対象の故障リスト
  vector<TpgFault*> mFaultList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 時間計測を制御する．
inline
void
DtpgSmt::timer_enable(bool enable)
{
  mSatEngineSingle.timer_enable(enable);
}

// @brief 故障リストをクリアする．
inline
void
DtpgSmt::clear_faults()
{
  mFaultList.clear();
}

// @brief 故障を追加する．
inline
void
DtpgSmt::add_fault(TpgFault* fault)
{
  if ( fault != NULL &&
       fault->is_rep() &&
       fault->status() != kFsDetected &&
       !fault->is_skip() ) {
    mFaultList.push_back(fault);
  }
}

END_NAMESPACE_YM_SATPG

#endif // DTPGSMT_H
