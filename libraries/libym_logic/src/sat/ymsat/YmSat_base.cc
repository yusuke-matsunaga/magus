
/// @file YmSat_base.cc
/// @brief YmSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmSat.h"
#include "YmLogic/SatStats.h"
#include "YmLogic/SatMsgHandler.h"
#include "SatAnalyzer.h"
#include "SatClause.h"


BEGIN_NAMESPACE_YM_SAT

#if 0
const
Params kDefaultParams(0.95, 0.02, 0.999, true, false, false, false);
#else
const
Params kDefaultParams(0.95, 0.00, 0.999, true, false, false, false);
#endif

//////////////////////////////////////////////////////////////////////
// YmSat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmSat::YmSat(const string& option) :
  mSane(true),
  mAlloc(4096),
  mConstrLitNum(0),
  mLearntBinNum(0),
  mLearntLitNum(0),
  mVarNum(0),
  mOldVarNum(0),
  mVarSize(0),
  mVal(NULL),
  mDecisionLevel(NULL),
  mReason(NULL),
  mWatcherList(NULL),
  mWeightArray(NULL),
  mLbdTmp(NULL),
  mLbdTmpSize(0),
  mRootLevel(0),
  mClauseBump(1.0),
  mClauseDecay(1.0),
  mParams(kDefaultParams),
  mConflictNum(0),
  mDecisionNum(0),
  mPropagationNum(0),
  mConflictLimit(0),
  mLearntLimit(0),
  mMaxConflict(1024 * 100)
{
  mAnalyzer = SaFactory::gen_analyzer(this, option);

  mSweep_assigns = -1;
  mSweep_props = 0;

  mLbdTmpSize = 1024;
  mLbdTmp = new bool[mLbdTmpSize];

  mTmpLitsSize = 1024;
  mTmpLits = new Literal[mTmpLitsSize];

  mTmpBinClause = new_clause(2);

  mRestart = 0;

  mTimerOn = false;

  if ( option == "no_phase_cache" ) {
    mParams.mPhaseCache = false;
  }
}

// @brief デストラクタ
YmSat::~YmSat()
{
  for (ymuint i = 0; i < mVarSize * 2; ++ i) {
    mWatcherList[i].finish(mAlloc);
  }

  delete mAnalyzer;
  delete [] mVal;
  delete [] mDecisionLevel;
  delete [] mReason;
  delete [] mWatcherList;
  delete [] mWeightArray;
  delete [] mLbdTmp;
  delete [] mTmpLits;
}

// @brief 正しい状態のときに true を返す．
bool
YmSat::sane() const
{
  return mSane;
}

// @brief 変数を追加する．
// @return 新しい変数番号を返す．
// @note 変数番号は 0 から始まる．
VarId
YmSat::new_var()
{
  if ( decision_level() != 0 ) {
    // エラー
    cout << "Error!: decision_level() != 0" << endl;
    ASSERT_NOT_REACHED;
    return kVarIdIllegal;
  }

  // ここではカウンタを増やすだけ
  // 実際の処理は alloc_var() でまとめて行う．
  ymuint n = mVarNum;
  ++ mVarNum;
  return VarId(n);
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
void
YmSat::add_clause(const vector<Literal>& lits)
{
  // add_clause_sub() 中でリテラルの並び替えを行うので
  // 一旦 mTmpLits にコピーする．
  ymuint n = lits.size();
  alloc_lits(n);
  for (ymuint i = 0; i < n; ++ i) {
    mTmpLits[i] = lits[i];
  }

  // 節を追加する本体
  add_clause_sub(n);
}

// @brief 節を追加する．
// @param[in] lit_num リテラル数
// @param[in] lits リテラルの配列
void
YmSat::add_clause(ymuint lit_num,
		  const Literal* lits)
{
  // add_clause_sub() 中でリテラルの並び替えを行うので
  // 一旦 mTmpLits にコピーする．
  alloc_lits(lit_num);
  for (ymuint i = 0; i < lit_num; ++ i) {
    mTmpLits[i] = lits[i];
  }

  // 節を追加する本体
  add_clause_sub(lit_num);
}

// @brief 1項の節(リテラル)を追加する．
void
YmSat::add_clause(Literal lit1)
{
  alloc_lits(1);
  mTmpLits[0] = lit1;

  // 節を追加する本体
  add_clause_sub(1);
}

// @brief 2項の節を追加する．
void
YmSat::add_clause(Literal lit1,
		  Literal lit2)
{
  alloc_lits(2);
  mTmpLits[0] = lit1;
  mTmpLits[1] = lit2;

  // 節を追加する本体
  add_clause_sub(2);
}

// @brief 3項の節を追加する．
void
YmSat::add_clause(Literal lit1,
		  Literal lit2,
		  Literal lit3)
{
  alloc_lits(3);
  mTmpLits[0] = lit1;
  mTmpLits[1] = lit2;
  mTmpLits[2] = lit3;

  // 節を追加する本体
  add_clause_sub(3);
}

// @brief 4項の節を追加する．
void
YmSat::add_clause(Literal lit1,
		  Literal lit2,
		  Literal lit3,
		  Literal lit4)
{
  alloc_lits(4);
  mTmpLits[0] = lit1;
  mTmpLits[1] = lit2;
  mTmpLits[2] = lit3;
  mTmpLits[3] = lit4;

  // 節を追加する本体
  add_clause_sub(4);
}

// @brief 5項の節を追加する．
void
YmSat::add_clause(Literal lit1,
		  Literal lit2,
		  Literal lit3,
		  Literal lit4,
		  Literal lit5)
{
  alloc_lits(5);
  mTmpLits[0] = lit1;
  mTmpLits[1] = lit2;
  mTmpLits[2] = lit3;
  mTmpLits[3] = lit4;
  mTmpLits[4] = lit5;

  // 節を追加する本体
  add_clause_sub(5);
}

// @brief 現在の内部状態を得る．
// @param[out] stats 状態を格納する構造体
void
YmSat::get_stats(SatStats& stats) const
{
  stats.mRestart = mRestart;
  stats.mVarNum = mVarNum;
  stats.mConstrClauseNum = clause_num();
  stats.mConstrLitNum = mConstrLitNum;
  stats.mLearntClauseNum = mLearntClause.size() + mLearntBinNum;
  stats.mLearntLitNum = mLearntLitNum;
  stats.mConflictNum = mConflictNum;
  stats.mDecisionNum = mDecisionNum;
  stats.mPropagationNum = mPropagationNum;
  stats.mConflictLimit = mConflictLimit;
  stats.mLearntLimit = mLearntLimit;
  stats.mTime = mTimer.time();
}

// @brief 変数の数を得る．
ymuint
YmSat::variable_num() const
{
  return mVarNum;
}

// @brief 制約節の数を得る．
ymuint
YmSat::clause_num() const
{
  return mConstrClause.size() + mConstrBinClause.size();
}

// @brief 制約節のリテラルの総数を得る．
ymuint
YmSat::literal_num() const
{
  return mConstrLitNum;
}

// @brief conflict_limit の最大値
// @param[in] val 設定する値
// @return 以前の設定値を返す．
ymuint64
YmSat::set_max_conflict(ymuint64 val)
{
  ymuint64 old_val = mMaxConflict;
  mMaxConflict = val;
  return old_val;
}

// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
// @param[in] msg_handler 登録するメッセージハンドラ
void
YmSat::reg_msg_handler(SatMsgHandler* msg_handler)
{
  mMsgHandlerList.push_back(msg_handler);
}

// @brief 時間計測機能を制御する
void
YmSat::timer_on(bool enable)
{
  mTimerOn = enable;
}

// @brief add_clause() の下請け関数
void
YmSat::add_clause_sub(ymuint lit_num)
{
  if ( decision_level() != 0 ) {
    // エラー
    cout << "Error![YmSat]: decision_level() != 0" << endl;
    ASSERT_NOT_REACHED;
    return;
  }

  if ( !mSane ) {
    // すでに充足不能になっていたらなにもしない．
    return;
  }

  // 変数領域の確保を行う．
  alloc_var();

  // mTmpLits をソートする．
  // たぶん要素数が少ないので挿入ソートが速いはず．
  for (ymuint i = 1; i < lit_num; ++ i) {
    // この時点で [0 : i - 1] までは整列している．
    Literal l = mTmpLits[i];
    if ( mTmpLits[i - 1] <= l ) {
      // このままで [0 : i] まで整列していることになる．
      continue;
    }

    // l の挿入位置を探す．
    ymuint j = i;
    for ( ; ; ) {
      mTmpLits[j] = mTmpLits[j - 1];
      -- j;
      if ( j == 0 || mTmpLits[j - 1] <= l ) {
	// 先頭に達するか，l よりも小さい要素があった．
	break;
      }
    }
    mTmpLits[j] = l;
  }

  // - 重複したリテラルの除去
  //   整列したのでおなじリテラルは並んでいるはず．
  // - false literal の除去
  // - true literal を持つかどうかのチェック
  ymuint wpos = 0;
  for (ymuint rpos = 0; rpos < lit_num; ++ rpos) {
    Literal l = mTmpLits[rpos];
    if ( wpos != 0 ) {
      Literal l1 = mTmpLits[wpos - 1];
      if ( l1 == l ) {
	// 重複している．
	continue;
      }
      if ( l1.varid() == l.varid() ) {
	// 同じ変数の相反するリテラル
	// この節は常に充足する．
	return;
      }
    }

    Bool3 v = eval(l);
    if ( v == kB3False ) {
      // false literal は追加しない．
      continue;
    }
    if ( v == kB3True ) {
      // true literal があったら既に充足している
      return;
    }
    if ( l.varid().val() >= mVarNum ) {
      // 範囲外
      // new_var() で確保した変数番号よりも大きい変数番号が
      // 使われていた．
      // TODO: エラー対策．
      cout << "Error![YmSat]: literal(" << l << "): out of range"
	   << endl;
      ASSERT_NOT_REACHED;
      return;
    }
    // 追加する．
    mTmpLits[wpos] = l;
    ++ wpos;
  }
  lit_num = wpos;

  mConstrLitNum += lit_num;

  if ( lit_num == 0 ) {
    // empty clause があったら unsat
    mSane = false;
    return;
  }

  Literal l0 = mTmpLits[0];
  if ( lit_num == 1 ) {
    // unit clause があったら値の割り当てを行う．
    bool stat = check_and_assign(l0);

    if ( debug & debug_assign ) {
      cout << "\tassign " << l0 << " @" << decision_level()
	   << endl;
      if ( !stat )  {
	cout << "\t--> conflict with previous assignment" << endl
	     << "\t    " << ~l0 << " was assigned at level "
	     << decision_level(l0.varid()) << endl;
      }
    }

    if ( stat ) {
      // 今の割当に基づく含意を行う．
      SatReason conflict = implication();
      if ( conflict != kNullSatReason ) {
	// 矛盾が起こった．
	stat = false;
      }
    }

    if ( !stat ) {
      // 矛盾が起こった．
      mSane = false;
      // ここは decision_level() == 0 のはずなのでバックトラックの必要はない．
    }
    return;
  }

  for (ymuint i = 0; i < lit_num; ++ i) {
    Literal l = mTmpLits[i];
    ymuint index = l.index();
    mWeightArray[index] += 1.0 / static_cast<double>(lit_num);
  }

  Literal l1 = mTmpLits[1];

  // 節の生成
  SatClause* clause = new_clause(lit_num);

  if ( debug & debug_assign ) {
    cout << "add_clause: " << *clause << endl;
  }

  mAllConstrClause.push_back(clause);

  if ( lit_num == 2 ) {
    // 二項節の watcher は相方のリテラルなので clause は使われない
    // ただし，デバッグ，検証用に別のリストに入れておく．
    mConstrBinClause.push_back(clause);

    // watcher-list の設定
    add_watcher(~l0, SatReason(l1));
    add_watcher(~l1, SatReason(l0));
  }
  else {
    mConstrClause.push_back(clause);

    // watcher-list の設定
    add_watcher(~l0, SatReason(clause));
    add_watcher(~l1, SatReason(clause));
  }
}

// 学習節を追加する．
// @param[in] learnt_lits 追加する節のもととなるリテラルのリスト
void
YmSat::add_learnt_clause(const vector<Literal>& learnt_lits)
{
  // learnt_lits の 0 番目のリテラルに値の割り当てが生じる．
  // 残りのリテラルがその割り当ての原因となる．

  ymuint n = learnt_lits.size();
  mLearntLitNum += n;

  if ( n == 0 ) {
    // empty clause があったら unsat
    mSane = false;
    return;
  }

  Literal l0 = learnt_lits[0];
  if ( n == 1 ) {
    // unit clause があったら値の割り当てを行う．
    bool stat = check_and_assign(l0);
    if ( debug & debug_assign ) {
      cout << "\tassign " << l0 << " @" << decision_level()
	   << endl;
      if ( !stat )  {
	cout << "\t--> conflict with previous assignment" << endl
	     << "\t    " << ~l0 << " was assigned at level "
	     << decision_level(l0.varid()) << endl;
      }
    }
    if ( !stat ) {
      mSane = false;
    }
    return;
  }

  SatReason reason;
  Literal l1 = learnt_lits[1];
  if ( n == 2 ) {
    // binary-clause の場合

    if ( debug & debug_assign ) {
      cout << "add_learnt_clause: "
	   << "(" << l0 << " + " << l1 << ")" << endl
	   << "\tassign " << l0 << " @" << decision_level()
	   << " from (" << l0 << " + " << l1 << ")" << endl;
    }

    // watcher-list の設定
    add_watcher(~l0, SatReason(l1));
    add_watcher(~l1, SatReason(l0));

    reason = SatReason(l1);

    ++ mLearntBinNum;
  }
  else {
    // 節の生成
    alloc_lits(n);
    for (ymuint i = 0; i < n; ++ i) {
      mTmpLits[i] = learnt_lits[i];
    }
    SatClause* clause = new_clause(n, true);

    if ( debug & debug_assign ) {
      cout << "add_learnt_clause: " << *clause << endl
	   << "\tassign " << l0 << " @" << decision_level()
	   << " from " << *clause << endl;
    }

    bump_clause_activity(clause);

    if ( mParams.mUseLbd ) {
      // LBD の計算
      ymuint lbd = calc_lbd(clause);
      clause->set_lbd(lbd);
    }

    mLearntClause.push_back(clause);

    reason = SatReason(clause);

    // watcher-list の設定
    add_watcher(~l0, reason);
    add_watcher(~l1, reason);
  }

  // learnt clause の場合には必ず unit clause になっているはず．
  ASSERT_COND( eval(l0) != kB3False );

  assign(l0, reason);
}

// @brief mTmpLits を確保する．
void
YmSat::alloc_lits(ymuint lit_num)
{
  ymuint old_size = mTmpLitsSize;
  while ( mTmpLitsSize <= lit_num ) {
    mTmpLitsSize <<= 1;
  }
  if ( old_size < mTmpLitsSize ) {
    delete [] mTmpLits;
    mTmpLits = new Literal[mTmpLitsSize];
  }
}

// @brief 新しい節を生成する．
// @param[in] lit_num リテラル数
// @param[in] learnt 学習節のとき true とするフラグ
// @param[in] lbd 学習節のときの literal block distance
// @note リテラルは mTmpLits に格納されている．
SatClause*
YmSat::new_clause(ymuint lit_num,
		  bool learnt)
{
  ymuint size = sizeof(SatClause) + sizeof(Literal) * (lit_num - 1);
  void* p = mAlloc.get_memory(size);
  SatClause* clause = new (p) SatClause(lit_num, mTmpLits, learnt);

  return clause;
}

// @brief 節を削除する．
// @param[in] clause 削除する節
void
YmSat::delete_clause(SatClause* clause)
{
  if ( debug & debug_assign ) {
    cout << " delete_clause: " << (*clause) << endl;
  }

  // watch list を更新
  del_watcher(~clause->wl0(), SatReason(clause));
  del_watcher(~clause->wl1(), SatReason(clause));

  if ( clause->is_learnt() ) {
    mLearntLitNum -= clause->lit_num();
  }
  else {
    mConstrLitNum -= clause->lit_num();
  }

  ymuint size = sizeof(SatClause) + sizeof(Literal) * (clause->lit_num() - 1);
  mAlloc.put_memory(size, static_cast<void*>(clause));
}

// @brief watcher を削除する．
// @param[in] watch_lit リテラル
// @param[in] reason 理由
void
YmSat::del_watcher(Literal watch_lit,
		   SatReason reason)
{
  // watch_lit に関係する watcher リストから
  // reason を探して削除する．
  // watcher リストを配列で実装しているので
  // あたまからスキャンして該当の要素以降を
  // 1つづつ前に詰める．
  Watcher w0(reason);
  WatcherList& wlist = watcher_list(watch_lit);
  ymuint n = wlist.num();
  ymuint wpos = 0;
  for ( ; wpos < n; ++ wpos) {
    Watcher w = wlist.elem(wpos);
    if ( w == w0 ) {
      break;
    }
  }
  ASSERT_COND( wpos < n );
  -- n;
  for ( ; wpos < n; ++ wpos) {
    Watcher w = wlist.elem(wpos + 1);
    wlist.set_elem(wpos, w);
  }
  wlist.erase(n);
}

// @brief 充足された watcher を削除する．
// @param[in] watch_lit リテラル
void
YmSat::del_satisfied_watcher(Literal watch_lit)
{
  // watch_lit に関係する watcher リストをスキャンして
  // literal watcher が充足していたらその watcher を削除する．
  // watcher リストを配列で実装しているので
  // あたまからスキャンして該当の要素以降を
  // 1つづつ前に詰める．
  WatcherList& wlist = watcher_list(watch_lit);
  ymuint n = wlist.num();
  ymuint wpos = 0;
  for (ymuint rpos; rpos < n; ++ rpos) {
    Watcher w = wlist.elem(rpos);
    if ( w.is_literal() ) {
      Literal l = w.literal();
      Bool3 val = eval(l);
      ASSERT_COND( val != kB3False );
      if ( val == kB3True ) {
	// この watcher は削除する．
	continue;
      }
    }
    if ( wpos != rpos ) {
      wlist.set_elem(wpos, w);
    }
    ++ wpos;
  }
  wlist.erase(wpos);
}

// 実際に変数に関するデータ構造を生成する．
void
YmSat::alloc_var()
{
  if ( mOldVarNum < mVarNum ) {
    if ( mVarSize < mVarNum ) {
      expand_var();
    }
    for (ymuint i = mOldVarNum; i < mVarNum; ++ i) {
      mVal[i] = conv_from_Bool3(kB3X) | (conv_from_Bool3(kB3X) << 2);
      mWeightArray[i * 2 + 0] = 0.0;
      mWeightArray[i * 2 + 1] = 0.0;
      mVarHeap.add_var(VarId(i));
    }
    mOldVarNum = mVarNum;
  }
}

// 変数に関する配列を拡張する．
void
YmSat::expand_var()
{
  ymuint old_size = mVarSize;
  ymuint8* old_val = mVal;
  int* old_decision_level = mDecisionLevel;
  SatReason* old_reason = mReason;
  WatcherList* old_watcher_list = mWatcherList;
  double* old_weight_array = mWeightArray;
  if ( mVarSize == 0 ) {
    mVarSize = 1024;
  }
  while ( mVarSize < mVarNum ) {
    mVarSize <<= 1;
  }
  mVal = new ymuint8[mVarSize];
  mDecisionLevel = new int[mVarSize];
  mReason = new SatReason[mVarSize];
  mWatcherList = new WatcherList[mVarSize * 2];
  mWeightArray = new double[mVarSize * 2];
  for (ymuint i = 0; i < mOldVarNum; ++ i) {
    mVal[i] = old_val[i];
    mDecisionLevel[i] = old_decision_level[i];
    mReason[i] = old_reason[i];
  }
  ymuint n2 = mOldVarNum * 2;
  for (ymuint i = 0; i < n2; ++ i) {
    mWatcherList[i].move(old_watcher_list[i]);
    mWeightArray[i] = old_weight_array[i];
  }
  if ( old_size > 0 ) {
    delete [] old_val;
    delete [] old_decision_level;
    delete [] old_reason;
    delete [] old_watcher_list;
    delete [] old_weight_array;
  }
  mAssignList.reserve(mVarSize);
  mVarHeap.alloc_var(mVarSize);
  mAnalyzer->alloc_var(mVarSize);
}

// @brief DIMACS 形式で制約節を出力する．
// @param[in] s 出力先のストリーム
void
YmSat::write_DIMACS(ostream& s) const
{
  s << "p cnf " << variable_num() << " " << clause_num() << endl;
#if 0
  for (ymuint i = 0; i < mConstrBinClause.size(); ++ i) {
    SatClause* clause = mConstrBinClause[i];
    ymuint nl = clause->lit_num();
    for (ymuint j = 0; j < nl; ++ j) {
      Literal lit = clause->lit(j);
      VarId var = lit.varid();
      ymint idx = var.val() + 1;
      if ( lit.is_negative() ) {
	s << " -" << idx;
      }
      else {
	s << " " << idx;
      }
    }
    s << " 0" << endl;
  }
  for (ymuint i = 0; i < mConstrClause.size(); ++ i) {
    SatClause* clause = mConstrClause[i];
    ymuint nl = clause->lit_num();
    for (ymuint j = 0; j < nl; ++ j) {
      Literal lit = clause->lit(j);
      VarId var = lit.varid();
      ymint idx = var.val() + 1;
      if ( lit.is_negative() ) {
	s << " -" << idx;
      }
      else {
	s << " " << idx;
      }
    }
    s << " 0" << endl;
  }
#else
  for (ymuint i = 0; i < mAllConstrClause.size(); ++ i) {
    SatClause* clause = mAllConstrClause[i];
    ymuint nl = clause->lit_num();
    for (ymuint j = 0; j < nl; ++ j) {
      Literal lit = clause->lit(j);
      VarId var = lit.varid();
      ymint idx = var.val() + 1;
      if ( lit.is_negative() ) {
	s << " -" << idx;
      }
      else {
	s << " " << idx;
      }
    }
    s << " 0" << endl;
  }
#endif
}

END_NAMESPACE_YM_SAT
