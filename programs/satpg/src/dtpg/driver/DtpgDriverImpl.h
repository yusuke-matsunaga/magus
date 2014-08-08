#ifndef DTPGDRIVERIMPL_H
#define DTPGDRIVERIMPL_H

/// @file DtpgDriverImpl.h
/// @brief DtpgDriverImpl のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgDriver.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgDriverImpl DtpgDriverImpl.h "DtpgDriverImpl.h"
/// @brief DtpgDriver の実装クラス
///
/// クラスの形をしているが，実際には状態をもたないただの関数
/// メンバ変数はサブルーティンを呼ぶ時の引数を省略するための
/// ものなので，一回の run() が終わったら消しても構わない．
//////////////////////////////////////////////////////////////////////
class DtpgDriverImpl :
  public DtpgDriver
{
public:

  /// @brief コンストラクタ
  DtpgDriverImpl();

  /// @brief デストラクタ
  virtual
  ~DtpgDriverImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // DtpgDriver の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] tpgnetwork 対象のネットワーク
  /// @param[in] mode メインモード
  /// @param[in] po_mode PO分割モード
  /// @param[in] engine DTPG エンジン
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& tgnetwork,
      tDtpgMode mode,
      tDtpgPoMode po_mode,
      DtpgEngine& engine,
      DtpgStats& stats);


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
  dtpg1(tDtpgMode mode,
	DtpgEngine& sat_engine);

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  single_mode(DtpgEngine& sat_engine);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  ffr_mode(DtpgEngine& sat_engine);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  mffc_mode(DtpgEngine& sat_engine);

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
  do_dtpg(DtpgEngine& sat_engine);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の回路
  TpgNetwork* mNetwork;

  // do_dtpg() で用いる対象の故障リスト
  vector<TpgFault*> mFaultList;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGDRIVERIMPL_H
