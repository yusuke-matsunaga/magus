#ifndef SATENGINE2_H
#define SATENGINE2_H

/// @file SatEngine2.h
/// @brief SatEngine2 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "TpgNode.h"
#include "logic/Literal.h"
#include "logic/Bool3.h"
#include "logic/sat_nsdef.h"
#include "utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngine2 SatEngine2.h "SatEngine2.h"
/// @brief SAT ベースのATPGエンジン
//////////////////////////////////////////////////////////////////////
class SatEngine2
{
public:

  /// @brief コンストラクタ
  SatEngine2();

  /// @brief デストラクタ
  virtual
  ~SatEngine2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL);

  /// @brief テスト生成を行なう．
  /// @param[in] f_tgt 対象の故障
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  run(TpgFault* f_tgt,
      TpgNode* fnode,
      int val,
      ymuint max_id,
      BackTracer& bt,
      DetectOp& dop,
      UntestOp& uop);

  /// @brief 統計情報をクリアする．
  void
  clear_stats();

  /// @brief 統計情報を得る．
  /// @param[in] stats 結果を格納する構造体
  void
  get_stats(DtpgStats& stats) const;

  /// @breif 時間計測を制御する．
  void
  timer_enable(bool enable);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 統計情報を得る．
  /// @param[in] solver SatSolver
  void
  update_stats(SatSolver& solver);

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

  // SAT solver のタイプ
  string mType;

  // SAT solver のオプション
  string mOption;

  // SAT solver の記録用ストリーム
  ostream* mOutP;

  // 故障箇所の TFO ノードを入れるリスト
  vector<TpgNode*> mTfoList;

  // ノードごとのいくつかのフラグをまとめた配列
  vector<ymuint8> mMarkArray;

  // 故障に関係する入力ノードのリスト
  vector<TpgNode*> mInputList;

  // 故障に関係する出力ノードのリスト
  vector<TpgNode*> mOutputList;

  // 作業領域として使うリテラルのリスト
  vector<Literal> mTmpLits;

  // SAT のモデル．
  vector<Bool3> mModel;

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
SatEngine2::set_tfo_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 1U;
  mTfoList.push_back(node);
  if ( node->is_output() ) {
    mOutputList.push_back(node);
  }
}

// @brief tfo マークを読む．
inline
bool
SatEngine2::tfo_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// tfi マークをつける．
inline
void
SatEngine2::set_tfi_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 2U;
  mTfoList.push_back(node);
  if ( node->is_input() ) {
    mInputList.push_back(node);
  }
}

// @brief tfi マークを読む．
inline
bool
SatEngine2::tfi_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief tmp マークをつける．
inline
void
SatEngine2::set_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 4U;
}

// @brief tmp マークを消す．
inline
void
SatEngine2::clear_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] &= ~4U;
}

// @brief tmp マークを読む．
inline
bool
SatEngine2::tmp_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 2) & 1U);
}

END_NAMESPACE_YM_SATPG

#endif // SATENGINEIMPL_H
