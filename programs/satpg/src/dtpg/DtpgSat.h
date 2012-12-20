#ifndef DTPGSAT_H
#define DTPGSAT_H

/// @file DtpgSat.h
/// @brief DtpgSat のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "ym_networks/tgnet.h"
#include "ym_logic/SatStats.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSat DtpgSat.h "DtpgSat.h"
/// @brief SAT を用いたパタン生成器
//////////////////////////////////////////////////////////////////////
class DtpgSat
{
public:

  /// @brief コンストラクタ
  DtpgSat();

  /// @brief デストラクタ
  virtual
  ~DtpgSat();


public:
  //////////////////////////////////////////////////////////////////////
  // パタン生成を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL);

  /// @brief 回路と故障リストを設定する．
  /// @param[in] tgnetwork 対象のネットワーク
  /// @param[in] fault_list 故障リスト
  void
  set_network(const TgNetwork& tgnetwork,
	      const vector<SaFault*>& fault_list);

#if 0
  /// @brief single モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  single(DtpgOperator& op);

  /// @brief dual モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  dual(DtpgOperator& op);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  ffr(DtpgOperator& op);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  mffc(DtpgOperator& op);

  /// @brief all モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  all(DtpgOperator& op);

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  single_posplit(DtpgOperator& op);

  /// @brief dual モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  dual_posplit(DtpgOperator& op);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  ffr_posplit(DtpgOperator& op);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  mffc_posplit(DtpgOperator& op);

  /// @brief all モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  all_posplit(DtpgOperator& op);
#else
  /// @brief single モードでテスト生成を行なう．
  void
  single(FaultMgr& fmgr,
	 TvMgr& tvmgr);

  /// @brief dual モードでテスト生成を行なう．
  void
  dual(FaultMgr& fmgr,
       TvMgr& tvmgr);

  /// @brief ffr モードでテスト生成を行なう．
  void
  ffr(FaultMgr& fmgr,
      TvMgr& tvmgr);

  /// @brief mffc モードでテスト生成を行なう．
  void
  mffc(FaultMgr& fmgr,
       TvMgr& tvmgr);

  /// @brief all モードでテスト生成を行なう．
  void
  all(FaultMgr& fmgr,
      TvMgr& tvmgr);

  /// @brief single モードでテスト生成を行なう．
  void
  single_posplit(FaultMgr& fmgr,
		 TvMgr& tvmgr,
		 bool skip);

  /// @brief dual モードでテスト生成を行なう．
  void
  dual_posplit(FaultMgr& fmgr,
	       TvMgr& tvmgr,
	       bool skip);

  /// @brief ffr モードでテスト生成を行なう．
  void
  ffr_posplit(FaultMgr& fmgr,
	      TvMgr& tvmgr,
	      bool skip);

  /// @brief mffc モードでテスト生成を行なう．
  void
  mffc_posplit(FaultMgr& fmgr,
	       TvMgr& tvmgr,
	       bool skip);

  /// @brief all モードでテスト生成を行なう．
  void
  all_posplit(FaultMgr& fmgr,
	      TvMgr& tvmgr,
	      bool skip);
#endif

  /// @brief 統計情報をクリアする．
  void
  clear_stats();

  /// @brief 統計情報を得る．
  void
  get_stats() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  single(DtpgOperator& op);

  /// @brief dual モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  dual(DtpgOperator& op);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  ffr(DtpgOperator& op);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  mffc(DtpgOperator& op);

  /// @brief all モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  all(DtpgOperator& op);

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  single_posplit(DtpgOperator& op);

  /// @brief dual モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  dual_posplit(DtpgOperator& op);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  ffr_posplit(DtpgOperator& op);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  mffc_posplit(DtpgOperator& op);

  /// @brief all モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  all_posplit(DtpgOperator& op);

  /// @brief 一つのノードに対して dual_mode でテストパタン生成を行なう．
  /// @param[in] node 対象のノード
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  /// @param[out] u_list 検査不能故障を格納するリスト
  void
  dual_mode_node(DtpgNode* node,
		 DtpgOperator& op);

  /// @brief 一つの FFR に対してテストパタン生成を行う．
  /// @param[in] ffr 対象の FFR
  void
  ffr_mode(DtpgFFR* ffr,
	   DtpgOperator& op);

  /// @brief スキップフラグを解除する．
  void
  clear_skip();

  /// @brief 統計情報を得る．
  void
  update_stats(SatSolver& solver,
	       ymuint n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 直前の SAT の実行結果
  SatStats mStats;

  // SAT solver のタイプ
  string mType;

  // SAT solver のオプション
  string mOption;

  // SAT solver の記録用ストリーム
  ostream* mOutP;

  // 対象の回路
  DtpgNetwork* mNetwork;

  // CNF の生成回数
  ymuint32 mRunCount;

  // SAT の実行回数
  ymuint32 mSatCount;

  // restart の回数の総和
  ymuint64 mRestart;

  // 変数の数の総和
  ymuint64 mVarNum;

  // 制約節の数の総和
  ymuint64 mConstrClauseNum;

  // 制約節のリテラル数の総和
  ymuint64 mConstrLitNum;

  // 学習節の数の総和
  ymuint64 mLearntClauseNum;

  // 学習節のリテラル数の総和
  ymuint64 mLearntLitNum;

  // コンフリクト数の総和
  ymuint64 mConflictNum;

  // decision 数の総和
  ymuint64 mDecisionNum;

  // implication数の総和
  ymuint64 mPropagationNum;

  // タイマー
  StopWatch mTimer;

};

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGSAT_H
