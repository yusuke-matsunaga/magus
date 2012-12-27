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
#include "Dtpg.h"
#include "ym_networks/tgnet.h"
#include "ym_logic/Literal.h"
#include "ym_logic/SatStats.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

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
  /// @param[in] tgnetwork 対象のネットワーク
  /// @param[in] fault_list 故障リスト
  virtual
  void
  set_network(const TgNetwork& tgnetwork,
	      const vector<SaFault*>& fault_list);

  /// @brief モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  /// @param[in] option オプション文字列
  virtual
  void
  run(DtpgOperator& op,
      const string& option = string());

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

  /// @brief single モードの共通処理
  void
  single_sub(DtpgOperator& op);

  /// @brief dual モードの共通処理
  void
  dual_sub(DtpgOperator& op);

  /// @brief ffr モードの共通処理
  void
  ffr_sub(DtpgOperator& op);

  /// @brief mffc モードの共通処理
  void
  mffc_sub(DtpgOperator& op);

  /// @brief all モードの共通処理
  void
  all_sub(DtpgOperator& op);

  /// @brief 一つの FFR に対してテストパタン生成を行う．
  /// @param[in] ffr 対象の FFR
  void
  ffr_mode(DtpgFFR* ffr,
	   DtpgOperator& op);

  /// @brief スキップフラグを解除する．
  void
  clear_skip();

  /// @brief 一つの故障に対してテストパタン生成を行う．
  /// @param[in] f 故障
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  dtpg_single(DtpgFault* f,
	      DtpgOperator& op);

  /// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
  /// @param[in] f0 0縮退故障
  /// @param[in] f1 1縮退故障
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  dtpg_dual(DtpgFault* f0,
	    DtpgFault* f1,
	    DtpgOperator& op);

  /// @brief FFR 内の故障に対してテストパタン生成を行なう．
  /// @@aram[in] network 対象のネットワーク
  /// @param[in] flist 故障リスト
  /// @param[in] root FFR の根のノード
  /// @param[in] node_list FFR 内のノードリスト
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  dtpg_ffr(const vector<DtpgFault*>& flist,
	   DtpgNode* root,
	   const vector<DtpgNode*>& node_list,
	   DtpgOperator& op);

  /// @brief 統計情報を得る．
  /// @param[in] solver SatSolver
  void
  update_stats(SatSolver& solver,
	       ymuint n);

  /// @brief fnode の故障が伝搬する条件を表す CNF を作る．
  /// @param[in] solver SatSolver
  /// @param[in] fnode 対象のノード
  /// @param[in] input_list fnode の TFI に含まれる入力ノードのリスト
  void
  make_prop_cnf(SatSolver& solver,
		DtpgNode* fnode,
		vector<DtpgNode*>& input_list);

  /// @brief 一つの SAT問題を解く．
  void
  solve(SatSolver& solver,
	DtpgFault* f,
	const vector<Literal>& assumptions,
	const vector<DtpgNode*>& input_list,
	DtpgOperator& op);

  /// @brief ノードの変数割り当てフラグを消す．
  void
  clear_node_mark();


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

  // 変数を割り当てたノードを格納するリスト
  vector<DtpgNode*> mUsedNodeList;

  // skip フラグ
  bool mSkip;

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
