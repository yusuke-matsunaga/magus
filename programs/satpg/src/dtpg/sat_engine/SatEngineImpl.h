#ifndef SATENGINEIMPL_H
#define SATENGINEIMPL_H

/// @file SatEngine.h
/// @brief SatEngine のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "../SatEngine.h"
#include "sat_engine_nsdef.h"
#include "TpgNode.h"
#include "TpgPrimitive.h"
#include "ym_logic/Literal.h"
#include "ym_logic/Bool3.h"
#include "ym_logic/sat_nsdef.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_SAT_ENGINE

//////////////////////////////////////////////////////////////////////
/// @class SatEngineImpl SatEngineImpl.h "SatEngineImpl.h"
/// @brief SatEngine の実装クラス
//////////////////////////////////////////////////////////////////////
class SatEngineImpl :
  public SatEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] dtpg DtpgSat
  SatEngineImpl(DtpgSat& dtpg);

  /// @brief デストラクタ
  virtual
  ~SatEngineImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  virtual
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL);

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] op テスト生成後に呼ばれるファンクター
  virtual
  void
  run(const vector<TpgFault*>& flist,
      ymuint max_id,
      BackTracer& bt);

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

  /// @brief 統計情報を得る．
  /// @param[in] solver SatSolver
  void
  update_stats(SatSolver& solver,
	       ymuint n);

  /// @brief 一つの SAT問題を解く．
  void
  solve(SatSolver& solver,
	TpgFault* f,
	BackTracer& bt);

  /// @brief ノードの変数割り当てフラグを消す．
  void
  clear_node_mark();

  /// @brief tfo マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tfo_mark(TpgNode* node);

  /// @brief tfo マークを読む．
  /// @param[in] node 対象のノード
  bool
  tfo_mark(TpgNode* node);

  /// @brief tfi マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tfi_mark(TpgNode* node);

  /// @brief tfi マークを読む．
  /// @param[in] node 対象のノード
  bool
  tfi_mark(TpgNode* node);

  /// @brief tmp マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tmp_mark(TpgNode* node);

  /// @brief tmp マークを消す．
  /// @param[in] node 対象のノード
  void
  clear_tmp_mark(TpgNode* node);

  /// @brief tmp マークを読む．
  /// @param[in] node 対象のノード
  bool
  tmp_mark(TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // DtpgSat
  DtpgSat& mDtpg;

  // SAT solver のタイプ
  string mType;

  // SAT solver のオプション
  string mOption;

  // SAT solver の記録用ストリーム
  ostream* mOutP;

  // SAT 用の assumption を格納するベクタ
  vector<Literal> mAssumptions;

  // SAT の結果を格納する配列
  vector<Bool3> mModel;

  // ノードごとのいくつかのフラグをまとめた配列
  vector<ymuint8> mMarkArray;

  // 故障の TFO のノードリスト
  vector<TpgNode*> mTfoList;

  // 故障の TFO の TFI のノードリスト
  vector<TpgNode*> mTfiList;

  // 変数を割り当てたノードを格納するリスト
  vector<TpgNode*> mUsedNodeList;

  // 現在の故障に関係のありそうな外部入力のリスト
  vector<TpgNode*> mInputList;

  // 現在の故障に関係ありそうな外部出力のリスト
  vector<TpgNode*> mOutputList;

  // 作業用のノードリスト
  vector<TpgNode*> mTmpNodeList;

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

  // 時間計測を行なうかどうかの制御フラグ
  bool mTimerEnable;

  // 時間計測用のタイマー
  StopWatch mTimer;

  // CNF 式を生成した回数
  ymuint32 mCnfCount;

  // CNF 式を生成する時の時間
  USTime mCnfTime;

  // テスト生成が成功した回数
  ymuint32 mDetCount;

  // テスト生成が成功した場合の実行時間
  USTime mDetTime;

  // 冗長故障の判定回数
  ymuint32 mUndetCount;

  // 冗長故障の場合の実行時間
  USTime mUndetTime;

  // アボート回数
  ymuint32 mAbortCount;

  // アボートの場合の実行時間
  USTime mAbortTime;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// tfo マークをつける．
inline
void
SatEngineImpl::set_tfo_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 1U;
}

// @brief tfo マークを読む．
inline
bool
SatEngineImpl::tfo_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// tfi マークをつける．
inline
void
SatEngineImpl::set_tfi_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 2U;
}

// @brief tfi マークを読む．
inline
bool
SatEngineImpl::tfi_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief tmp マークをつける．
inline
void
SatEngineImpl::set_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 4U;
}

// @brief tmp マークを消す．
inline
void
SatEngineImpl::clear_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] &= ~4U;
}

// @brief tmp マークを読む．
inline
bool
SatEngineImpl::tmp_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 2) & 1U);
}

END_NAMESPACE_YM_SATPG_SAT_ENGINE

#endif // SATENGINEIMPL_H
