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

#include "TpgNode.h"
#include "TpgPrimitive.h"
#include "ym_logic/Literal.h"
#include "ym_logic/Bool3.h"
#include "ym_logic/sat_nsdef.h"
#include "ym_utils/StopWatch.h"


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
  run(const vector<TpgFault*>& flist,
      ymuint max_id,
      TpgOperator& op);

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
	TpgFault* f,
	TpgOperator& op);

  /// @brief テストパタンを求める．
  /// @param[in] fnode 故障のあるノード
  /// @note 結果は mValList に格納される．
  void
  get_pat(TpgNode* fnode);

  /// @brief テストパタンを求める．
  /// @param[in] fnode 故障のあるノード
  /// @note 結果は mValList に格納される．
  void
  get_pat2(TpgNode* fnode);

  /// @brief solve 中で故障差を持つノードをたどる．
  /// @param[in] node 対象のノード
  /// @retval true node を通って外部出力まで故障差が伝搬している．
  /// @retval false 故障差が伝搬していない．
  /// @note 故障差の伝搬経路上のノードは mDiffNodeList に格納される．
  /// @note 一旦調べたノードはすべて mark1 がつく．
  /// @note 故障差が伝搬しているノードは mark2 がつく．
  /// @note マークがついたノードは mBwdNodeList に格納される．
  bool
  fwd_dfs(TpgNode* node);

  /// @brief solve 中で変数割り当ての正当化を行なう．
  /// @param[in] node 対象のノード
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify(TpgNode* node);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  void
  just_sub1(TpgNode* node);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val 値
  void
  just_sub2(TpgNode* node,
	    Bool3 val);

  /// @brief justify の下請け関数
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify_primitive(TpgPrimitive* prim,
		    TpgNode* node);

  /// @brief すべてのファンインに対して justify_primitive() を呼ぶ．
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  void
  jp_sub1(TpgPrimitive* prim,
	  TpgNode* node);

  /// @brief 指定した値を持つファンインに対して justify_primitive() を呼ぶ．
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  /// @param[in] val 値
  void
  jp_sub2(TpgPrimitive* prim,
	  TpgNode* node,
	  Bool3 val);

  /// @brief 入力ノードの値を記録する．
  /// @param[in] node 対象の外部入力ノード
  /// @note node の値を mValList に記録する．
  /// @note 単純だが mModel 上のインデックスと mValList の符号化は異なる．
  void
  record_value(TpgNode* node);

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

  /// @brief justified マークをつける．
  /// @param[in] node 対象のノード
  void
  set_justified_mark(TpgNode* node);

  /// @brief justified マークを消す．
  /// @param[in] node 対象のノード
  void
  clear_justified_mark(TpgNode* node);

  /// @brief justified マークを読む．
  /// @param[in] node 対象のノード
  bool
  justified_mark(TpgNode* node);

  /// @brief ノードの正常値を読み出す．
  /// @param[in] node 対象のノード
  /// @note mModel の値を読む．
  Bool3
  node_gval(TpgNode* node);

  /// @brief ノードの故障値を読み出す．
  /// @param[in] node 対象のノード
  /// @note mModel の値を読む．
  Bool3
  node_fval(TpgNode* node);

  /// @brief プリミティブの正常値を読み出す．
  /// @param[in] prim 対象のプリミティブ
  /// @note mModel の値を読む．
  Bool3
  primitive_gval(TpgPrimitive* prim);

  /// @brief プリミティブの故障値を読み出す．
  /// @param[in] prim 対象のプリミティブ
  /// @note mModel の値を読む．
  Bool3
  primitive_fval(TpgPrimitive* prim);


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

  // 故障差が伝搬しているノードを格納するリスト
  vector<TpgNode*> mDiffNodeList;

  // 正当化されたノードのリスト
  vector<TpgNode*> mJustifiedNodeList;

  // skip フラグ
  bool mSkip;

  // 検出不能故障をスキップするしきい値
  ymuint32 mSkipThreshold;

  // 検出不能と判定された故障のリスト
  vector<TpgFault*> mUntestFaults;

  // skip フラグのついた故障のリスト
  vector<TpgFault*> mSkippedFaults;

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
SatEngine::set_tfo_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 1U;
}

// @brief tfo マークを読む．
inline
bool
SatEngine::tfo_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// tfi マークをつける．
inline
void
SatEngine::set_tfi_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 2U;
}

// @brief tfi マークを読む．
inline
bool
SatEngine::tfi_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief tmp マークをつける．
inline
void
SatEngine::set_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 4U;
}

// @brief tmp マークを消す．
inline
void
SatEngine::clear_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] &= ~4U;
}

// @brief tmp マークを読む．
inline
bool
SatEngine::tmp_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 2) & 1U);
}

// justified マークをつける．
inline
void
SatEngine::set_justified_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 8U;
}

// justified マークを消す．
inline
void
SatEngine::clear_justified_mark(TpgNode* node)
{
  mMarkArray[node->id()] &= ~8U;
}

// @brief justified マークを読む．
inline
bool
SatEngine::justified_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 3) & 1U);
}

// @brief ノードの正常値を読み出す．
// @note mModel の値を読む．
inline
Bool3
SatEngine::node_gval(TpgNode* node)
{
  return mModel[node->gvar().val()];
}

// @brief ノードの故障値を読み出す．
// @note mModel の値を読む．
inline
Bool3
SatEngine::node_fval(TpgNode* node)
{
  return mModel[node->fvar().val()];
}

// @brief プリミティブの正常値を読み出す．
// @note mModel の値を読む．
inline
Bool3
SatEngine::primitive_gval(TpgPrimitive* prim)
{
  Literal lit = prim->glit();
  Bool3 val = mModel[lit.varid().val()];
  if ( lit.pol() == kPolNega ) {
    val = ~val;
  }
  return val;
}

// @brief プリミティブの故障値を読み出す．
// @note mModel の値を読む．
inline
Bool3
SatEngine::primitive_fval(TpgPrimitive* prim)
{
  Literal lit = prim->flit();
  Bool3 val = mModel[lit.varid().val()];
  if ( lit.pol() == kPolNega ) {
    val = ~val;
  }
  return val;
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // SATENGINE_H
