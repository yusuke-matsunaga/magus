#ifndef LIBYM_LOGIC_SAT_YMSAT_YMSAT_H
#define LIBYM_LOGIC_SAT_YMSAT_YMSAT_H

/// @file libym_logic/sat/ymsat/YmSat.h
/// @brief YmSat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "../SatSolverImpl.h"
#include "ym_utils/FragAlloc.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/StopWatch.h"
#include "SatClause.h"
#include "SatReason.h"
#include "AssignList.h"
#include "Watcher.h"


BEGIN_NAMESPACE_YM_SAT

class SatAnalyzer;

//////////////////////////////////////////////////////////////////////
/// @class Params YmSat.h "YmSat.h"
/// @brief YmSat の挙動を制御するパラメータ
//////////////////////////////////////////////////////////////////////
struct Params
{
  /// @brief 変数の decay 値
  double mVarDecay;

  /// @brief 変数選択用の閾値
  double mVarFreq;

  /// @brief 学習節の decay 値
  double mClauseDecay;

  /// @brief コンストラクタ
  Params() :
    mVarDecay(1.0),
    mVarFreq(0.0),
    mClauseDecay(1.0)
  {
  }

  /// @brief 値を指定したコンストラクタ
  Params(double var_decay,
	 double var_freq,
	 double clause_decay) :
    mVarDecay(var_decay),
    mVarFreq(var_freq),
    mClauseDecay(clause_decay)
  {
  }

};


//////////////////////////////////////////////////////////////////////
/// @class YmSat YmSat.h "YmSat.h"
/// @brief SatSolver の実装クラス
//////////////////////////////////////////////////////////////////////
class YmSat :
  public SatSolverImpl
{
  friend class SatAnalyzer;

public:

  /// @brief コンストラクタ
  /// @param[in] analyzer 解析器のポインタ
  /// @param[in] option オプション文字列
  YmSat(SatAnalyzer* analyzer,
	const string& option = string());

  /// @brief デストラクタ
  virtual
  ~YmSat();


public:
  //////////////////////////////////////////////////////////////////////
  // SatSolver で定義されている仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正しい状態のときに true を返す．
  virtual
  bool
  sane() const;

  /// @brief 変数を追加する．
  /// @return 新しい変数番号を返す．
  /// @note 変数番号は 0 から始まる．
  virtual
  VarId
  new_var();

  /// @brief 節を追加する．
  /// @param[in] lits リテラルのベクタ
  virtual
  void
  add_clause(const vector<Literal>& lits);

  /// @brief 節を追加する．
  /// @param[in] lit_num リテラル数
  /// @param[in] lits リテラルの配列
  virtual
  void
  add_clause(ymuint lit_num,
	     Literal* lits);

  /// @brief SAT 問題を解く．
  /// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
  /// @param[out] model 充足するときの値の割り当てを格納する配列．
  /// @retval kB3True 充足した．
  /// @retval kB3False 充足不能が判明した．
  /// @retval kB3X わからなかった．
  /// @note i 番めの変数の割り当て結果は model[i] に入る．
  virtual
  Bool3
  solve(const vector<Literal>& assumptions,
	vector<Bool3>& model);

  /// @brief 学習節の整理を行なう．
  virtual
  void
  reduce_learnt_clause();

  /// @brief 現在の内部状態を得る．
  /// @param[out] stats 状態を格納する構造体
  virtual
  void
  get_stats(SatStats& stats) const;

  /// @brief 変数の数を得る．
  virtual
  ymuint
  variable_num() const;

  /// @brief 制約節の数を得る．
  virtual
  ymuint
  clause_num() const;

  /// @brief 制約節のリテラルの総数を得る．
  virtual
  ymuint
  literal_num() const;

  /// @brief conflict_limit の最大値
  /// @param[in] val 設定する値
  /// @return 以前の設定値を返す．
  virtual
  ymuint64
  set_max_conflict(ymuint64 val);

  /// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
  /// @param[in] msg_handler 登録するメッセージハンドラ
  virtual
  void
  reg_msg_handler(SatMsgHandler* msg_handler);

  /// @brief 時間計測機能を制御する
  virtual
  void
  timer_on(bool enable);


private:
  //////////////////////////////////////////////////////////////////////
  // 実装用のプライベート関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 探索を行う本体の関数
  Bool3
  search();

  /// @brief 割当てキューに基づいて implication を行う．
  SatReason
  implication();

  /// @brief 割当てキューに基づいて implication を行う．
  SatReason
  implication0();

  /// @brief level までバックトラックする
  /// @param[in] level バックトラックするレベル
  void
  backtrack(int level);

  /// @brief 次の割り当てを選ぶ．
  /// @note 割り当てられる変数がない場合には kLiteralX を返す．
  Literal
  next_decision();

  /// @brief 値の割当てか可能かチェックする．
  /// @param[in] lit 割り当てるリテラル
  /// @return 矛盾が起きたら false を返す．
  bool
  check_and_assign(Literal lit);

  /// @brief 値の割当てを行う．
  /// @param[in] lit 割り当てるリテラル
  /// @param[in] reason 割り当ての理由
  /// @note 実際にはキューに積む．
  /// @return 矛盾が起きたら false を返す．
  void
  assign(Literal lit,
	 SatReason reason = SatReason());

  /// @brief CNF を簡単化する．
  void
  simplifyDB();

  /// @brief 使われていない学習節を削除する．
  void
  reduceDB();

  /// @brief 学習節を追加する．
  /// @param[in] lits 追加する節を構成するリテラルのリスト
  void
  add_learnt_clause(const vector<Literal>& lits);

  /// @brief add_clause() の下請け関数
  void
  add_clause_sub(ymuint lit_num);

  /// @brief 新しい節を生成する．
  /// @param[in] lit_num リテラル数
  /// @param[in] learnt 学習節のとき true とするフラグ
  /// @note リテラルは mTmpLits に格納されている．
  SatClause*
  new_clause(ymuint lit_num,
	     bool learnt = false);

  /// @brief mTmpLits を確保する．
  void
  alloc_lits(ymuint lit_num);

  /// @brief 節を削除する．
  /// @param[in] clause 削除する節
  void
  delete_clause(SatClause* clause);

  /// @brief watcher list を得る．
  /// @param[in] lit リテラル
  WatcherList&
  watcher_list(Literal lit);

  /// @brief Watcher を追加する．
  /// @param[in] watch_lit リテラル
  /// @param[in] reason 理由
  void
  add_watcher(Literal watch_lit,
	      SatReason reason);

  /// @brief watcher を削除する．
  /// @param[in] watch_lit リテラル
  /// @param[in] reason 理由
  void
  del_watcher(Literal watch_lit,
	      SatReason reason);

  /// @brief 変数1の評価を行う．
  /// @param[in] id 変数番号
  Bool3
  eval(VarId id) const;

  /// @brief literal の評価を行う．
  /// @param[in] l リテラル
  Bool3
  eval(Literal l) const;

  /// @brief 現在の decision level を返す．
  int
  decision_level() const;

  /// @brief 変数の decision level を返す．
  /// @param[in] varid 変数番号
  int
  decision_level(VarId varid) const;

  /// @brief 変数の割り当て理由を返す．
  /// @param[in] varid 変数番号
  SatReason
  reason(VarId varid) const;

  /// @brief 学習節が使われているか調べる．
  /// @param[in] clause 対象の節
  bool
  is_locked(SatClause* clause) const;

  /// @brief 変数のアクティビティを増加させる．
  /// @param[in] var 変数番号
  void
  bump_var_activity(VarId var);

  /// @brief 変数のアクティビティを定率で減少させる．
  void
  decay_var_activity();

  /// @brief リスタート時の変数のアクティビティの低減率
  void
  decay_var_activity2();

  /// @brief 学習節のアクティビティを増加させる．
  /// @param[in] clause 対象の学習節
  void
  bump_clause_activity(SatClause* clause);

  /// @brief 学習節のアクティビティを定率で減少させる．
  void
  decay_clause_activity();

  /// @brief 実際に変数に関するデータ構造を生成する．
  void
  alloc_var();

  /// @brief 変数に関する配列を拡張する．
  void
  expand_var();


private:
  //////////////////////////////////////////////////////////////////////
  // ヒープ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 空にする．
  void
  heap_clear();

  /// @brief 要素が空の時 true を返す．
  bool
  heap_empty() const;

  /// @brief 変数を始めてヒープに追加する．
  /// @param[in] vindex 追加する変数番号
  void
  heap_add_var(ymuint vindex);

  /// @brief 変数を再びヒープに追加する．
  /// @param[in] vindex 追加する変数番号
  void
  heap_push(ymuint vindex);

  /// @brief アクティビティ最大の変数番号を取り出す．
  /// @note 該当の変数はヒープから取り除かれる．
  ymuint
  heap_pop_top();

  /// @brief 引数の位置にある要素を適当な位置まで沈めてゆく
  /// @param[in] pos 対象の要素の位置
  void
  heap_move_down(ymuint pos);

  /// @brief 引数の位置にある要素を適当な位置まで上げてゆく
  /// @param[in] vindex 対象の変数番号
  void
  heap_move_up(ymuint vindex);

  /// @brief 変数を配列にセットする．
  /// @param[in] vindex 対象の変数番号
  /// @param[in] pos 位置
  /// @note mHeap と mHeapPos の一貫性を保つためにはこの関数を使うこと．
  void
  heap_set(ymuint vindex,
	   ymuint pos);

  /// @brief 左の子供の位置を計算する
  /// @param[in] pos 親の位置
  static
  ymuint
  heap_left(ymuint pos);

  /// @brief 右の子供の位置を計算する．
  /// @param[in] pos 親の位置
  static
  ymuint
  heap_right(ymuint pos);

  /// @brief 親の位置を計算する．
  /// @param[in] pos 子供の位置
  /// @note 左の子供でも右の子供でも同じ
  static
  ymuint
  heap_parent(ymuint pos);

  /// @brief 内容を出力する
  /// @param[in] s 出力先のストリーム
  void
  heap_dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 解析器
  SatAnalyzer* mAnalyzer;

  // 正常の時に true となっているフラグ
  bool mSane;

  // SatClause のメモリ領域確保用のアロケータ
  FragAlloc mAlloc;

  // 制約節の配列
  vector<SatClause*> mConstrClause;

  // 学習節の配列
  vector<SatClause*> mLearntClause;

  // 変数の数
  ymuint32 mVarNum;

  // 前回の alloc_var で処理した時の変数の数
  ymuint32 mOldVarNum;

  // 変数関係の配列のサイズ
  ymuint32 mVarSize;

  // 値の配列
  // サイズは mVarSize
  Bool3* mVal;

  // 値が割り当てられたときのレベルの配列
  // サイズは mVarSize
  int* mDecisionLevel;

  // 値が割り当てられた理由の配列
  // サイズは mVarSize
  SatReason* mReason;

  // ヒープ上の位置の配列
  // サイズは mVarSize
  ymint32* mHeapPos;

  // アクティビティ
  // サイズは mVarSize
  double* mActivity;

  // watcher list の配列
  // サイズは mVarSize * 2
  WatcherList* mWatcherList;

  // ヒープ用の配列
  // サイズは mVarSize
  ymuint32* mHeap;

  // ヒープの要素数
  ymuint32 mHeapNum;

  // 矛盾の解析時にテンポラリに使用される節
  SatClause* mTmpBinClause;

  // search 開始時の decision level
  int mRootLevel;

  // 値割り当てを保持するリスト
  AssignList mAssignList;

  // 変数のアクティビティの増加量
  double mVarBump;

  // 変数のアクティビティの減衰量
  double mVarDecay;

  // 学習節のアクティビティの増加量
  double mClauseBump;

  // 学習節のアクティビティの減衰量
  double mClauseDecay;

  // 変数選択用乱数発生器
  RandGen mRandGen;

  // mTimer を使うとき true にするフラグ
  bool mTimerOn;

  // 時間計測器
  StopWatch mTimer;

  // 制御用パラメータ
  Params mParams;

  // restart 数
  ymuint mRestart;

  // 総コンフリクト数
  ymuint64 mConflictNum;

  // 総 decision 数
  ymuint64 mDecisionNum;

  // 総 implication 数
  ymuint64 mPropagationNum;

  // watch literal を変更した数
  ymuint64 mWatchNum;

  // 二項制約節の数
  ymuint64 mConstrBinNum;

  // 制約節の総リテラル数
  ymuint64 mConstrLitNum;

  // 二項学習節の数
  ymuint64 mLearntBinNum;

  // 学習節の総リテラル数
  ymuint64 mLearntLitNum;

  // コンフリクト数の制限
  ymuint64 mConflictLimit;

  // 学習節の制限
  ymuint64 mLearntLimit;

  // mConflistLimit の最大値
  ymuint64 mMaxConflict;

  // メッセージハンドラのリスト
  list<SatMsgHandler*> mMsgHandlerList;

  // add_clause で一時的に利用するリテラル配列
  Literal* mTmpLits;

  // mTmpLits のサイズ
  ymuint32 mTmpLitsSize;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を得る．
inline
ymuint
YmSat::variable_num() const
{
  return mVarNum;
}

// @brief 制約節の数を得る．
inline
ymuint
YmSat::clause_num() const
{
  return mConstrClause.size() + mConstrBinNum;
}

// @brief 制約節のリテラルの総数を得る．
inline
ymuint
YmSat::literal_num() const
{
  return mConstrLitNum;
}

// watcher list を得る．
inline
WatcherList&
YmSat::watcher_list(Literal lit)
{
  ymuint index = lit.index();
  return mWatcherList[index];
}

// Watcher を追加する．
inline
void
YmSat::add_watcher(Literal watch_lit,
		   SatReason reason)
{
  watcher_list(watch_lit).add(Watcher(reason));
}

// 変数の評価を行う．
inline
Bool3
YmSat::eval(VarId id) const
{
  return mVal[id.val()];
}

// literal の評価を行う．
inline
Bool3
YmSat::eval(Literal l) const
{
  ymuint index = l.index();
  Bool3 val = mVal[index / 2];
  int d = 1 - (index & 1U) * 2;
  return static_cast<Bool3>(static_cast<int>(val) * d);
}

// 値の割当てか可能かチェックする．
// 矛盾が起きたら false を返す．
inline
bool
YmSat::check_and_assign(Literal lit)
{
  Bool3 old_val = eval(lit);
  if ( old_val != kB3X ) {
    return old_val == kB3True;
  }
  assign(lit);
  return true;
}

// 値の割当てを行う．
inline
void
YmSat::assign(Literal lit,
	      SatReason reason)
{
  ymuint lindex = lit.index();
  ymuint vindex = lindex / 2;
  mVal[vindex] = static_cast<Bool3>(1 - static_cast<int>(lindex & 1U) * 2);
  mDecisionLevel[vindex] = decision_level();
  mReason[vindex] = reason;

  // mAssignList に格納する．
  mAssignList.put(lit);
}

// 現在の decision level を返す．
inline
int
YmSat::decision_level() const
{
  return mAssignList.cur_level();
}

// 変数の decision level を返す．
inline
int
YmSat::decision_level(VarId varid) const
{
  return mDecisionLevel[varid.val()];
}

// 変数の割り当て理由を返す．
inline
SatReason
YmSat::reason(VarId varid) const
{
  return mReason[varid.val()];
}

// @brief clase が含意の理由になっているか調べる．
inline
bool
YmSat::is_locked(SatClause* clause) const
{
  // 直感的には分かりにくいが，節の最初のリテラルは
  // 残りのリテラルによって含意されていることになっている．
  // そこで最初のリテラルの変数の割り当て理由が自分自身か
  // どうかを調べれば clause が割り当て理由として用いられて
  // いるかわかる．
  return reason(clause->wl0().varid()) == SatReason(clause);
}

// @brief 時間計測機能を制御する
inline
void
YmSat::timer_on(bool enable)
{
  mTimerOn = enable;
}

// 実際に変数に関するデータ構造を生成する．
inline
void
YmSat::alloc_var()
{
  if ( mOldVarNum < mVarNum ) {
    if ( mVarSize < mVarNum ) {
      expand_var();
    }
    for (ymuint i = mOldVarNum; i < mVarNum; ++ i) {
      mVal[i] = kB3X;
      mActivity[i] = 0.0;
      heap_add_var(i);
    }
    mOldVarNum = mVarNum;
  }
}

// @brief 空にする．
inline
void
YmSat::heap_clear()
{
  mHeapNum = 0;
}

// @brief 要素が空の時 true を返す．
inline
bool
YmSat::heap_empty() const
{
  return mHeapNum == 0;
}

// @brief 変数を始めてヒープに追加する．
// @param[in] vindex 追加する変数番号
inline
void
YmSat::heap_add_var(ymuint vindex)
{
  heap_set(vindex, mHeapNum);
  ++ mHeapNum;
}

// @brief 要素を追加する．
inline
void
YmSat::heap_push(ymuint vindex)
{
  if ( mHeapPos[vindex] == -1 ) {
    ymuint pos = mHeapNum;
    ++ mHeapNum;
    heap_set(vindex, pos);
    heap_move_up(pos);
  }
}

// @brief もっともアクティビティの高い変数を返す．
inline
ymuint
YmSat::heap_pop_top()
{
  // この assert は重いのでコメントアウトしておく
  //assert_cond(mHeapNum > 0, __FILE__, __LINE__);
  ymuint ans = mHeap[0];
  mHeapPos[ans] = -1;
  -- mHeapNum;
  if ( mHeapNum > 0 ) {
    ymuint vindex = mHeap[mHeapNum];
    heap_set(vindex, 0);
    heap_move_down(0);
  }
  return ans;
}

// 引数の位置にある要素を適当な位置まで上げてゆく
inline
void
YmSat::heap_move_up(ymuint pos)
{
  ymuint vindex = mHeap[pos];
  double val = mActivity[vindex];
  while ( pos > 0 ) {
    ymuint pos_p = heap_parent(pos);
    ymuint vindex_p = mHeap[pos_p];
    double val_p = mActivity[vindex_p];
    if ( val_p >= val ) {
      break;
    }
    heap_set(vindex, pos_p);
    heap_set(vindex_p, pos);
    pos = pos_p;
  }
}

// 変数を配列にセットする．
inline
void
YmSat::heap_set(ymuint vindex,
		ymuint pos)
{
  mHeap[pos] = vindex;
  mHeapPos[vindex] = pos;
}

// @brief 左の子供の位置を計算する
inline
ymuint
YmSat::heap_left(ymuint pos)
{
  return pos + pos + 1;
}

// @brief 右の子供の位置を計算する．
inline
ymuint
YmSat::heap_right(ymuint pos)
{
  return pos + pos + 2;
}

// @brief 親の位置を計算する．
inline
ymuint
YmSat::heap_parent(ymuint pos)
{
  return (pos - 1) >> 1;
}

END_NAMESPACE_YM_SAT

#endif // LIBYM_LOGIC_SAT_YMSAT_YMSAT_H
