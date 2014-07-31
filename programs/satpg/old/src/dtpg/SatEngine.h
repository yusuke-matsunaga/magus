#ifndef SATENGINE_H
#define SATENGINE_H

/// @file SatEngine.h
/// @brief SatEngine のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"

//#include "ym_YmNetworks/tgnet.h"
#include "DtpgNode.h"
#include "ym_YmLogic/Literal.h"
#include "ym_YmLogic/Bool3.h"
#include "ym_YmLogic/sat_nsdef.h"
#include "ym_YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngine SatEngine.h "SatEngine.h"
/// @brief SAT を用いたパタン生成器
//////////////////////////////////////////////////////////////////////
class SatEngine
{
public:

  /// @brief コンストラクタ
  SatEngine();

  /// @brief デストラクタ
  virtual
  ~SatEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL);

  /// @brief skip モードに設定する．
  /// @param[in] threshold 検出不能故障をスキップするしきい値
  void
  set_skip(ymuint32 threshold);

  /// @brief skip モードを解除する．
  void
  clear_skip();

  /// @brief get_pat フラグを設定する．
  void
  set_get_pat(ymuint val);

  /// @brief dry-run フラグを設定する．
  void
  set_dry_run(bool flag);

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  run(const vector<DtpgFault*>& flist,
      ymuint max_id,
      DtpgOperator& op);

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
  update_stats(SatSolver& solver,
	       ymuint n);

  /// @brief 一つの SAT問題を解く．
  void
  solve(SatSolver& solver,
	DtpgFault* f,
	DtpgOperator& op);

  /// @brief テストパタンを求める．
  /// @param[in] fnode 故障のあるノード
  /// @note 結果は mValList に格納される．
  void
  get_pat(DtpgNode* fnode);

  /// @brief テストパタンを求める．
  /// @param[in] fnode 故障のあるノード
  /// @note 結果は mValList に格納される．
  void
  get_pat2(DtpgNode* fnode);

  /// @brief solve 中で故障差を持つノードをたどる．
  /// @param[in] node 対象のノード
  /// @retval true node を通って外部出力まで故障差が伝搬している．
  /// @retval false 故障差が伝搬していない．
  /// @note 故障差の伝搬経路上のノードは mDiffNodeList に格納される．
  /// @note 一旦調べたノードはすべて mark1 がつく．
  /// @note 故障差が伝搬しているノードは mark2 がつく．
  /// @note マークがついたノードは mBwdNodeList に格納される．
  bool
  fwd_dfs(DtpgNode* node);

  /// @brief solve 中で変数割り当ての正当化を行なう．
  /// @param[in] node 対象のノード
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify(DtpgNode* node);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  void
  just_sub1(DtpgNode* node);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val 値
  void
  just_sub2(DtpgNode* node,
	    Bool3 val);

  /// @brief justify の下請け関数
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify_primitive(DtpgPrimitive* prim,
		    DtpgNode* node);

  /// @brief すべてのファンインに対して justify_primitive() を呼ぶ．
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  void
  jp_sub1(DtpgPrimitive* prim,
	  DtpgNode* node);

  /// @brief 指定した値を持つファンインに対して justify_primitive() を呼ぶ．
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  /// @param[in] val 値
  void
  jp_sub2(DtpgPrimitive* prim,
	  DtpgNode* node,
	  Bool3 val);

  /// @brief 入力ノードの値を記録する．
  /// @param[in] node 対象の外部入力ノード
  /// @note node の値を mValList に記録する．
  /// @note 単純だが mModel 上のインデックスと mValList の符号化は異なる．
  void
  record_value(DtpgNode* node);

  /// @brief ノードの変数割り当てフラグを消す．
  void
  clear_node_mark();

  /// tfo マークをつける．
  void
  set_tfo_mark(DtpgNode* node);

  /// @brief tfo マークを読む．
  bool
  tfo_mark(DtpgNode* node);

  /// tfi マークをつける．
  void
  set_tfi_mark(DtpgNode* node);

  /// @brief tfi マークを読む．
  bool
  tfi_mark(DtpgNode* node);

  /// @brief tmp マークをつける．
  void
  set_tmp_mark(DtpgNode* node);

  /// @brief tmp マークを消す．
  void
  clear_tmp_mark(DtpgNode* node);

  /// @brief tmp マークを読む．
  bool
  tmp_mark(DtpgNode* node);

  /// justified マークをつける．
  void
  set_justified_mark(DtpgNode* node);

  /// justified マークを消す．
  void
  clear_justified_mark(DtpgNode* node);

  /// @brief justified マークを読む．
  bool
  justified_mark(DtpgNode* node);


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

  // get_pat フラグ
  ymuint32 mGetPatFlag;

  // SAT 用の assumption を格納するベクタ
  vector<Literal> mAssumptions;

  // SAT 用の model を格納するベクタ
  vector<Bool3> mModel;

  // SAT 用の割り当てリスト
  vector<ymuint> mValList;

  // いくつかのフラグをまとめた配列
  vector<ymuint8> mMarkArray;

  // 故障の TFO のノードリスト
  vector<DtpgNode*> mTfoList;

  // 故障の TFO の TFI のノードリスト
  vector<DtpgNode*> mTfiList;

  // 変数を割り当てたノードを格納するリスト
  vector<DtpgNode*> mUsedNodeList;

  // 現在の故障に関係のありそうな外部入力のリスト
  vector<DtpgNode*> mInputList;

  // 現在の故障に関係ありそうな外部出力のリスト
  vector<DtpgNode*> mOutputList;

  // 作業用のノードリスト
  vector<DtpgNode*> mTmpNodeList;

  // 故障差が伝搬しているノードを格納するリスト
  vector<DtpgNode*> mDiffNodeList;

  // 正当化されたノードのリスト
  vector<DtpgNode*> mJustifiedNodeList;

  // skip フラグ
  bool mSkip;

  // 検出不能故障をスキップするしきい値
  ymuint32 mSkipThreshold;

  // 検出不能と判定された故障のリスト
  vector<DtpgFault*> mUntestFaults;

  // skip フラグのついた故障のリスト
  vector<DtpgFault*> mSkippedFaults;

  // dry-run フラグ
  bool mDryRun;

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
SatEngine::set_tfo_mark(DtpgNode* node)
{
  mMarkArray[node->id()] |= 1U;
}

// @brief tfo マークを読む．
inline
bool
SatEngine::tfo_mark(DtpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// tfi マークをつける．
inline
void
SatEngine::set_tfi_mark(DtpgNode* node)
{
  mMarkArray[node->id()] |= 2U;
}

// @brief tfi マークを読む．
inline
bool
SatEngine::tfi_mark(DtpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief tmp マークをつける．
inline
void
SatEngine::set_tmp_mark(DtpgNode* node)
{
  mMarkArray[node->id()] |= 4U;
}

// @brief tmp マークを消す．
inline
void
SatEngine::clear_tmp_mark(DtpgNode* node)
{
  mMarkArray[node->id()] &= ~4U;
}

// @brief tmp マークを読む．
inline
bool
SatEngine::tmp_mark(DtpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 2) & 1U);
}

// justified マークをつける．
inline
void
SatEngine::set_justified_mark(DtpgNode* node)
{
  mMarkArray[node->id()] |= 8U;
}

// justified マークを消す．
inline
void
SatEngine::clear_justified_mark(DtpgNode* node)
{
  mMarkArray[node->id()] &= ~8U;
}

// @brief justified マークを読む．
inline
bool
SatEngine::justified_mark(DtpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 3) & 1U);
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // SATENGINE_H
