#ifndef DTPGSAT_H
#define DTPGSAT_H

/// @file DtpgSat.h
/// @brief DtpgSat のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
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
  /// @param[in] op テスト生成後に呼ばれるファンクター
  /// @param[in] option オプション文字列
  virtual
  void
  run(tDtpgMode mode,
      tDtpgPoMode po_mode,
      BackTracer& bt,
      TpgOperator& op);

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  single_mode(BackTracer& bt,
	      TpgOperator& op);

  /// @brief dual モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  dual_mode(BackTracer& bt,
	    TpgOperator& op);

  /// @brief node モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  node_mode(BackTracer& bt,
	    TpgOperator& op);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  ffr_mode(BackTracer& bt,
	   TpgOperator& op);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  mffc_mode(BackTracer& bt,
	    TpgOperator& op);

  /// @brief all モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  all_mode(BackTracer& bt,
	   TpgOperator& op);

  /// @brief 統計情報をクリアする．
  virtual
  void
  clear_stats();

  /// @brief 統計情報を得る．
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  get_stats(DtpgStats& stats) const;

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief single モードの共通処理
  void
  single_sub(BackTracer& bt,
	     TpgOperator& op);

  /// @brief dual モードの共通処理
  void
  dual_sub(BackTracer& bt,
	   TpgOperator& op);

  /// @brief ffr モードの共通処理
  void
  ffr_sub(BackTracer& bt,
	  TpgOperator& op);

  /// @brief mffc モードの共通処理
  void
  mffc_sub(BackTracer& bt,
	   TpgOperator& op);

  /// @brief all モードの共通処理
  void
  all_sub(BackTracer& bt,
	  TpgOperator& op);

  /// @brief 一つの故障に対してテストパタン生成を行う．
  /// @param[in] f 故障
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  dtpg_single(TpgFault* f,
	      BackTracer& bt,
	      TpgOperator& op);

  /// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
  /// @param[in] f0 0縮退故障
  /// @param[in] f1 1縮退故障
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  dtpg_dual(TpgFault* f0,
	    TpgFault* f1,
	    BackTracer& bt,
	    TpgOperator& op);

  /// @brief DFS で FFR を求める．
  void
  dfs_ffr(TpgNode* node);

  /// @brief DFS で MFFC を求める．
  void
  dfs_mffc(TpgNode* node,
	   vector<bool>& mark);

  /// @brief ノードの故障を追加する．
  void
  add_node_faults(TpgNode* node);

  /// @brief 故障を追加する．
  void
  add_fault(TpgFault* fault);

  /// @brief テストパタン生成を行なう．
  void
  do_dtpg(BackTracer& bt,
	  TpgOperator& op);


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

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 統計情報をクリアする．
inline
void
DtpgSat::clear_stats()
{
  mSatEngine->clear_stats();
}

// @brief 統計情報を得る．
// @param[in] stats 結果を格納する構造体
inline
void
DtpgSat::get_stats(DtpgStats& stats) const
{
  mSatEngine->get_stats(stats);
}

// @breif 時間計測を制御する．
inline
void
DtpgSat::timer_enable(bool enable)
{
  mSatEngine->timer_enable(enable);
}

// @brief 故障を追加する．
inline
void
DtpgSat::add_fault(TpgFault* fault)
{
  if ( fault->is_rep() &&
       fault->status() != kFsDetected &&
       !fault->is_skip() ) {
    mFaultList.push_back(fault);
  }
}

// @brief テストパタン生成を行なう．
inline
void
DtpgSat::do_dtpg(BackTracer& bt,
		 TpgOperator& op)
{
  if ( !mFaultList.empty() ) {
    mSatEngine->run(mFaultList, mMaxId, bt, op);
  }
}

END_NAMESPACE_YM_SATPG

#endif // DTPGSAT_H
