
/// @file libym_logic/sat/ymsat/YmSat.cc
/// @brief YmSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: YmSat.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmSat.h"
#include "ym_logic/SatStats.h"
#include "ym_logic/SatMsgHandler.h"
#include "SatAnalyzer.h"
#include "SatClause.h"


BEGIN_NAMESPACE_YM_SAT

BEGIN_NONAMESPACE

const ymuint debug_none        = 0x00;

const ymuint debug_implication = 0x01;

const ymuint debug_analyze     = 0x02;

const ymuint debug_assign      = 0x04;

const ymuint debug_decision    = 0x08;

const ymuint debug_solve       = 0x10;

const ymuint debug_all         = 0xffffffff;

//const ymuint debug = debug_decision | debug_analyze | debug_assign;
//const ymuint debug = debug_assign;
//const ymuint debug = debug_assign | debug_implication;
//const ymuint debug = debug_assign | debug_analyze;
//const ymuint debug = debug_solve | debug_decision;
//const ymuint debug = debug_solve | debug_assign;
//const ymuint debug = debug_all;
const ymuint debug = debug_none;

END_NONAMESPACE


const
Params kDefaultParams(0.95, 0.02, 0.999);


//////////////////////////////////////////////////////////////////////
// YmSat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmSat::YmSat(SatAnalyzer* analyzer,
	     const string& option) :
  mAnalyzer(analyzer),
  mSane(true),
  mAlloc(4096),
  mConstrBinNum(0),
  mConstrLitNum(0),
  mLearntBinNum(0),
  mLearntLitNum(0),
  mVarNum(0),
  mOldVarNum(0),
  mVarSize(0),
  mVal(NULL),
  mDecisionLevel(NULL),
  mReason(NULL),
  mHeapPos(NULL),
  mActivity(NULL),
  mWatcherList(NULL),
  mHeap(NULL),
  mHeapNum(0),
  mRootLevel(0),
  mVarBump(1.0),
  mVarDecay(1.0),
  mClauseBump(1.0),
  mClauseDecay(1.0),
  mParams(kDefaultParams),
  mConflictNum(0),
  mDecisionNum(0),
  mPropagationNum(0),
  mConflictLimit(0),
  mLearntLimit(0),
  mMaxConflict(1024 * 10)
{
  mAnalyzer->set_solver(this);

  mTmpLitsSize = 1024;
  mTmpLits = new Literal[mTmpLitsSize];

  mTmpBinClause = new_clause(2);

  mRestart = 0;

  mTimerOn = false;
}

// @brief デストラクタ
YmSat::~YmSat()
{
  delete mAnalyzer;
  delete [] mVal;
  delete [] mDecisionLevel;
  delete [] mReason;
  delete [] mHeapPos;
  delete [] mActivity;
  delete [] mWatcherList;
  delete [] mHeap;
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
    return kVarIdIllegal;
  }

  // ここではカウンタを増やすだけ
  // 実際の処理は alloc_var() でまとめて行う．
  ymuint n = mVarNum;
  ++ mVarNum;
  return VarId(n);
}

// 変数に関する配列を拡張する．
void
YmSat::expand_var()
{
  ymuint old_size = mVarSize;
  Bool3* old_val = mVal;
  int* old_decision_level = mDecisionLevel;
  SatReason* old_reason = mReason;
  ymint32* old_heap_pos = mHeapPos;
  double* old_activity = mActivity;
  WatcherList* old_watcher_list = mWatcherList;
  ymuint32* old_heap = mHeap;
  if ( mVarSize == 0 ) {
    mVarSize = 1024;
  }
  while ( mVarSize < mVarNum ) {
    mVarSize <<= 1;
  }
  mVal = new Bool3[mVarSize];
  mDecisionLevel = new int[mVarSize];
  mReason = new SatReason[mVarSize];
  mHeapPos = new ymint32[mVarSize];
  mActivity = new double[mVarSize];
  mWatcherList = new WatcherList[mVarSize * 2];
  mHeap = new ymuint32[mVarSize];
  for (ymuint i = 0; i < mOldVarNum; ++ i) {
    mVal[i] = old_val[i];
    mDecisionLevel[i] = old_decision_level[i];
    mReason[i] = old_reason[i];
    mHeapPos[i] = old_heap_pos[i];
    mActivity[i] = old_activity[i];
  }
  ymuint n2 = mOldVarNum * 2;
  for (ymuint i = 0; i < n2; ++ i) {
    mWatcherList[i].move(old_watcher_list[i]);
  }
  for (ymuint i = 0; i < mHeapNum; ++ i) {
    mHeap[i] = old_heap[i];
  }
  if ( old_size > 0 ) {
    delete [] old_val;
    delete [] old_decision_level;
    delete [] old_reason;
    delete [] old_heap_pos;
    delete [] old_activity;
    delete [] old_watcher_list;
    delete [] old_heap;
  }
  mAssignList.reserve(mVarSize);
  mAnalyzer->alloc_var(mVarSize);
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
void
YmSat::add_clause(const vector<Literal>& lits)
{
  ymuint n = lits.size();
  alloc_lits(n);
  for (ymuint i = 0; i < n; ++ i) {
    mTmpLits[i] = lits[i];
  }
  add_clause_sub(n);
}

// @brief 節を追加する．
// @param[in] lit_num リテラル数
// @param[in] lits リテラルの配列
void
YmSat::add_clause(ymuint lit_num,
		  Literal* lits)
{
  alloc_lits(lit_num);
  for (ymuint i = 0; i < lit_num; ++ i) {
    mTmpLits[i] = lits[i];
  }
  add_clause_sub(lit_num);
}

// @brief add_clause() の下請け関数
void
YmSat::add_clause_sub(ymuint lit_num)
{
  if ( decision_level() != 0 ) {
    // エラー
    cout << "Error![YmSat]: decision_level() != 0" << endl;
    return;
  }

  if ( !mSane ) {
    cout << "Error![YmSat]: mSane == false" << endl;
    return;
  }

  alloc_var();

  // - 重複したリテラルの除去
  // - false literal の除去
  // - true literal を持つかどうかのチェック
  ymuint wpos = 0;
  for (ymuint rpos = 0; rpos < lit_num; ++ rpos) {
    Literal l = mTmpLits[rpos];
    if ( wpos != 0 && mTmpLits[wpos - 1] == l ) {
      // 重複している．
      continue;
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
      cout << "Error![YmSat]: literal(" << l << "): out of range"
	   << endl;
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
    if ( !stat ) {
      mSane = false;
    }
    return;
  }

  Literal l1 = mTmpLits[1];

  if ( lit_num == 2 ) {
    // watcher-list の設定
    add_watcher(~l0, SatReason(l1));
    add_watcher(~l1, SatReason(l0));
    ++ mConstrBinNum;
  }
  else {
    // 節の生成
    SatClause* clause = new_clause(lit_num, mTmpLits);
    mConstrClause.push_back(clause);

    // watcher-list の設定
    SatReason reason(clause);
    add_watcher(~l0, reason);
    add_watcher(~l1, reason);
  }
}

// 学習節を追加する．
void
YmSat::add_learnt_clause(const vector<Literal>& lits)
{
  ymuint n = lits.size();
  mLearntLitNum += n;

  if ( n == 0 ) {
    // empty clause があったら unsat
    mSane = false;
    return;
  }

  Literal l0 = lits[0];
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
  Literal l1 = lits[1];
  if ( n == 2 ) {
    reason = SatReason(l1);

    // watcher-list の設定
    add_watcher(~l0, SatReason(l1));
    add_watcher(~l1, SatReason(l0));
    ++ mLearntBinNum;
  }
  else {
    // 節の生成
    alloc_lits(n);
    for (ymuint i = 0; i < n; ++ i) {
      mTmpLits[i] = lits[i];
    }
    SatClause* clause = new_clause(n, true);
    mLearntClause.push_back(clause);

    reason = SatReason(clause);

    // watcher-list の設定
    add_watcher(~l0, reason);
    add_watcher(~l1, reason);
  }

  // learnt clause の場合には必ず unit clause になっているはず．
  assert_cond(eval(l0) != kB3False, __FILE__, __LINE__);
  if ( debug & debug_assign ) {
    cout << "\tassign " << l0 << " @" << decision_level()
	 << " from " << reason << endl;
  }

  assign(l0, reason);
}

// @brief watcher を削除する．
// @param[in] watch_lit リテラル
// @param[in] reason 理由
void
YmSat::del_watcher(Literal watch_lit,
		   SatReason reason)
{
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
  assert_cond( wpos < n, __FILE__, __LINE__);
  -- n;
  for ( ; wpos < n; ++ wpos) {
    Watcher w = wlist.elem(wpos + 1);
    wlist.set_elem(wpos, w);
  }
  wlist.erase(n);
}

// @brief SAT 問題を解く．
// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
// @param[out] model 充足するときの値の割り当てを格納する配列．
// @retval kB3True 充足した．
// @retval kB3False 充足不能が判明した．
// @retval kB3X わからなかった．
// @note i 番めの変数の割り当て結果は model[i] に入る．
Bool3
YmSat::solve(const vector<Literal>& assumptions,
	     vector<Bool3>& model)
{
  if ( debug & debug_solve ) {
    cout << "YmSat::solve starts" << endl;
    cout << " Assumptions: ";
    const char* and_str = "";
    for (vector<Literal>::const_iterator p = assumptions.begin();
	 p != assumptions.end(); ++ p) {
      cout << and_str << *p;
      and_str = " & ";
    }
    cout << endl;
    cout << " Clauses:" << endl;
    for (vector<SatClause*>::const_iterator p = mConstrClause.begin();
	 p != mConstrClause.end(); ++ p) {
      cout << "  " << *(*p) << endl;
    }
  }

  if ( mTimerOn ) {
    mTimer.stop();
    mTimer.reset();
    mTimer.start();
  }

  // 自明な簡単化を行う．
  simplifyDB();
  if ( !mSane ) {
    // その時点で充足不可能なら終わる．
    if ( debug & debug_solve ) {
      cout << "UNSAT in simplifyDB()" << endl;
    }
    return kB3False;
  }

  // パラメータの初期化
  double confl_limit = 100;
  double learnt_limit = mConstrClause.size() / 3;
  mVarDecay = mParams.mVarDecay;
  mClauseDecay = mParams.mClauseDecay;

  assert_cond(decision_level() == 0, __FILE__, __LINE__);

  // assumption の割り当てを行う．
  for (vector<Literal>::const_iterator p = assumptions.begin();
       p != assumptions.end(); ++ p) {
    Literal lit = *p;

    mAssignList.set_marker();
    bool stat = check_and_assign(lit);

    if ( debug & (debug_assign | debug_decision) ) {
      cout << endl
	   << "assume " << lit << " @" << decision_level()
	   << endl;
      if ( !stat )  {
	cout << "\t--> conflict with previous assignment" << endl
	     << "\t    " << ~lit << " was assigned at level "
	     << decision_level(lit.varid()) << endl;
      }
    }

    // 条件式のなかに重要な手続きが書いてあるあんまり良くないコード
    // だけど implication() は stat == true の時しか実行しないのでしょうがない．
    if ( !stat || implication() != kNullSatReason ) {
      // 矛盾が起こった．
      backtrack(0);
      if ( debug & debug_solve ) {
	cout << "UNSAT" << endl;
      }

      return kB3False;
    }
  }

  // 以降，現在のレベルが基底レベルとなる．
  mRootLevel = decision_level();
  if ( debug & (debug_assign | debug_decision) ) {
    cout << "RootLevel = " << mRootLevel << endl;
  }

  Bool3 stat = kB3X;
  for ( ; ; ) {
    // 実際の探索を行う．
    mConflictLimit = static_cast<ymuint64>(confl_limit);
    if ( mConflictLimit > mMaxConflict ) {
      mConflictLimit = mMaxConflict;
    }
    mLearntLimit = static_cast<ymuint64>(learnt_limit);
    SatStats stats;
    get_stats(stats);
    for (list<SatMsgHandler*>::iterator p = mMsgHandlerList.begin();
	 p != mMsgHandlerList.end(); ++ p) {
      SatMsgHandler& handler = *(*p);
      handler(stats);
    }
    ++ mRestart;
    stat = search();
    if ( stat != kB3X ) {
      // 結果が求められた．
      break;
    }
    if ( mConflictLimit == mMaxConflict ) {
      // 制限値に達した．(アボート)
      break;
    }

    // 判定できなかったのでパラメータを更新して次のラウンドへ
    decay_var_activity2();
    confl_limit = confl_limit * 1.5;
    learnt_limit = learnt_limit * 1.1;
  }
  if ( stat == kB3True ) {
    // SAT ならモデル(充足させる変数割り当てのリスト)を作る．
    model.resize(mVarNum);
    for (ymuint i = 0; i < mVarNum; ++ i) {
      Bool3 val = mVal[i];
      assert_cond(val != kB3X, __FILE__, __LINE__);
      model[i] = val;
    }
  }
  backtrack(0);

  if ( mTimerOn ) {
    mTimer.stop();
  }

  if ( debug & debug_solve ) {
    switch ( stat ) {
    case kB3True:  cout << "SAT" << endl; break;
    case kB3False: cout << "UNSAT" << endl; break;
    case kB3X:     cout << "UNKNOWN" << endl; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
  }

  return stat;
}

// @brief 学習節の整理を行なう．
void
YmSat::reduce_learnt_clause()
{
  reduceDB();
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

// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
// @param[in] msg_handler 登録するメッセージハンドラ
void
YmSat::reg_msg_handler(SatMsgHandler* msg_handler)
{
  mMsgHandlerList.push_back(msg_handler);
}

// 探索を行う本体の関数
Bool3
YmSat::search()
{
  // 矛盾の解析時に用いられるベクタ
  vector<Literal> learnt;

  // コンフリクトの起こった回数
  ymuint n_confl = 0;
  for ( ; ; ) {
    // キューにつまれている割り当てから含意される値の割り当てを行う．
    SatReason conflict = implication();
    if ( conflict != kNullSatReason ) {
      // 矛盾が生じた．
      ++ n_confl;
      ++ mConflictNum;
      if ( decision_level() == mRootLevel ) {
	// トップレベルで矛盾が起きたら充足不可能
	return kB3False;
      }
      if ( n_confl > mConflictLimit ) {
	// 矛盾の回数が制限値を越えた．
	backtrack(mRootLevel);
	return kB3X;
      }

      // 今の矛盾の解消に必要な条件を「学習」する．
      int bt_level = mAnalyzer->analyze(conflict, learnt);

      if ( debug & debug_analyze ) {
	cout << endl
	     << "analyze for " << conflict << endl
	     << "learnt clause is ";
	const char* plus = "";
	for (ymuint i = 0; i < learnt.size(); ++ i) {
	  Literal l = learnt[i];
	  cout << plus << l << " @" << decision_level(l.varid());
	  plus = " + ";
	}
	cout << endl;
      }

      // バックトラック
      if ( bt_level < mRootLevel ) {
	bt_level = mRootLevel;
      }
      backtrack(bt_level);

      // 学習節の生成
      add_learnt_clause(learnt);

      decay_var_activity();
      decay_clause_activity();
    }
    else {
      if ( decision_level() == 0 ) {
	simplifyDB();
      }
      if ( mLearntClause.size() > mLearntLimit ) {
	reduceDB();
      }

      // 次の割り当てを選ぶ．
      Literal lit = next_decision();
      if ( lit == kLiteralX ) {
	// すべての変数を割り当てた．
	// ということは充足しているはず．
	return kB3True;
      }
      ++ mDecisionNum;

      // バックトラックポイントを記録
      mAssignList.set_marker();

      // 未割り当ての変数を選んでいるのでエラーになるはずはない．
      if ( debug & (debug_assign | debug_decision) ) {
	cout << endl
	     << "choose " << lit << " @" << decision_level() << endl;
      }
      assign(lit);
    }
  }
}

// 割当てキューに基づいて implication を行う．
SatReason
YmSat::implication()
{
  SatReason conflict = kNullSatReason;
  while ( mAssignList.has_elem() ) {
    Literal l = mAssignList.get_next();
    ++ mPropagationNum;

    if ( debug & debug_implication ) {
      cout << "\tpick up " << l << endl;
    }
    // l の割り当てによって無効化された watcher-list の更新を行う．
    Literal nl = ~l;

    WatcherList& wlist = watcher_list(l);
    ymuint n = wlist.num();
    ymuint rpos = 0;
    ymuint wpos = 0;
    while ( rpos < n ) {
      Watcher w = wlist.elem(rpos);
      wlist.set_elem(wpos, w);
      ++ rpos;
      ++ wpos;
      if ( w.is_literal() ) {
	// 2-リテラル節の場合は相方のリテラルに基づく値の割り当てを行う．
	Literal l0 = w.literal();
	Bool3 val0 = eval(l0);
	if ( val0 == kB3X ) {
	  if ( debug & debug_assign ) {
	    cout << "\tassign " << l0 << " @" << decision_level()
		 << " from " << l << endl;
	  }
	  assign(l0, SatReason(nl));
	}
	else if ( val0 == kB3False ) {
	  // 矛盾がおこった．
	  if ( debug & debug_assign ) {
	    cout << "\t--> conflict with previous assignment" << endl
		 << "\t    " << ~l0 << " was assigned at level "
		 << decision_level(l0.varid()) << endl;
	  }

	  // ループを抜けるためにキューの末尾まで先頭を動かす．
	  mAssignList.skip_all();

	  // 矛盾の理由を表す節を作る．
	  mTmpBinClause->set(l0, nl);
	  conflict = SatReason(mTmpBinClause);
	  break;
	}
      }
      else { // w.is_clause()
	// 3つ以上のリテラルを持つ節の場合は，
	// - nl(~l) を wl1() にする．(場合によっては wl0 を入れ替える)
	// - wl0() が充足していたらなにもしない．
	// - wl0() が不定，もしくは偽なら，nl の代わりの watch literal を探す．
	// - 代わりが見つかったらそのリテラルを wl1() にする．
	// - なければ wl0() に基づいた割り当てを行う．場合によっては矛盾が起こる．
	SatClause& c = w.clause();
	Literal l0 = c.wl0();
	if ( l0 == nl ) {
	  // nl を 1番めのリテラルにする．
	  c.xchange_wl();
	  // 新しい wl0 を得る．
	  l0 = c.wl0();
	}
	else { // l1 == nl
	  if ( debug & debug_implication ) {
	    // この assert は重いのでデバッグ時にしかオンにしない．
	    // ※ debug と debug_implication が const なので結果が0の
	    // ときにはコンパイル時に消されることに注意
	    assert_cond(c.wl1() == nl, __FILE__, __LINE__);
	  }
	}

	Bool3 val0 = eval(l0);
	if ( val0 == kB3True ) {
	  // すでに充足していた．
	  continue;
	}

	if ( debug & debug_implication ) {
	  cout << "\t\texamining watcher clause " << c << endl;
	}

	// nl の替わりのリテラルを見つける．
	// この時，替わりのリテラルが未定かすでに充足しているかどうか
	// は問題でない．
	bool found = false;
	ymuint n = c.lit_num();
	for (ymuint i = 2; i < n; ++ i) {
	  Literal l2 = c.lit(i);
	  if ( eval(l2) != kB3False ) {
	    // l2 を 1番めの watch literal にする．
	    c.insert(i, 1);
	    if ( debug & debug_implication ) {
	      cout << "\t\t\tsecond watching literal becomes "
		   << l2 << endl;
	    }
	    // l の watcher list から取り除く
	    -- wpos;
	    // ~l2 の watcher list に追加する．
	    watcher_list(~l2).add(w);

	    found = true;
	    break;
	  }
	}
	if ( found ) {
	  continue;
	}

	if ( debug & debug_implication ) {
	  cout << "\t\tno other watching literals" << endl;
	}

	// 見付からなかったので l0 に従った割り当てを行う．
	if ( val0 == kB3X ) {
	  if ( debug & debug_assign ) {
	    cout << "\tassign " << l0 << " @" << decision_level()
		 << " from " << w << endl;
	  }
	  assign(l0, w);
	}
	else {
	  // 矛盾がおこった．
	  if ( debug & debug_assign ) {
	    cout << "\t--> conflict with previous assignment" << endl
		 << "\t    " << ~l0 << " was assigned at level "
		 << decision_level(l0.varid()) << endl;
	  }

	  // ループを抜けるためにキューの末尾まで先頭を動かす．
	  mAssignList.skip_all();

	  // この場合は w が矛盾の理由を表す節になっている．
	  conflict = w;
	  break;
	}
      }
    }
    // 途中でループを抜けた場合に wlist の後始末をしておく．
    if ( wpos != rpos ) {
      for ( ; rpos < n; ++ rpos) {
	wlist.set_elem(wpos, wlist.elem(rpos));
	++ wpos;
      }
      wlist.erase(wpos);
    }
  }

  return conflict;
}

// level までバックトラックする
void
YmSat::backtrack(int level)
{
  if ( debug & (debug_assign | debug_decision) ) {
    cout << endl
	 << "backtrack until @" << level << endl;
  }

  if ( level < decision_level() ) {
    mAssignList.backtrack(level);
    while ( mAssignList.has_elem() ) {
      Literal p = mAssignList.get_prev();
      VarId varid = p.varid();
      ymuint vindex = varid.val();
      mVal[vindex] = kB3X;
      heap_push(vindex);
      if ( debug & debug_assign ) {
	cout << "\tdeassign " << p << endl;
      }
    }
  }

  if ( debug & (debug_assign | debug_decision) ) {
    cout << endl;
  }
}

// 次の割り当てを選ぶ
Literal
YmSat::next_decision()
{
#if 0
  // 一定確率でランダムな変数を選ぶ．
  if ( mRandGen.real1() < mParams.mVarFreq && !heap_empty() ) {
    ymuint pos = mRandGen.int32() % mVarNum;
    VarId vid(pos);
    tPol pol = kPolNega;
    if ( eval(VarId(vid)) == kB3X ) {
      return Literal(vid, pol);
    }
  }
#endif
  while ( !heap_empty() ) {
    ymuint vindex = heap_pop_top();
    if ( mVal[vindex] == kB3X ) {
      // Watcher の多い方の極性を(わざと)選ぶ
      ymuint v2 = vindex * 2;
      tPol pol = kPolNega;
      if ( mWatcherList[v2 + 0].num() >= mWatcherList[v2 + 1].num() ) {
	pol = kPolPosi;
      }
      return Literal(VarId(vindex), pol);
    }
  }
  return kLiteralX;
}

// CNF を簡単化する．
void
YmSat::simplifyDB()
{
  if ( !mSane ) {
    return;
  }
  assert_cond(decision_level() == 0, __FILE__, __LINE__);

  if ( implication() != kNullSatReason ) {
    mSane = false;
    return;
  }

}

BEGIN_NONAMESPACE
// reduceDB で用いる SatClause の比較関数
class SatClauseLess
{
public:
  bool
  operator()(SatClause* a,
	     SatClause* b)
  {
    return a->lit_num() > 2 && (b->lit_num() == 2 || a->activity() < b->activity() );
  }
};
END_NONAMESPACE

// 使われていない学習節を削除する．
void
YmSat::reduceDB()
{
  ymuint n = mLearntClause.size();
  ymuint n2 = n / 2;

  // 足切りのための制限値
  double abs_limit = mClauseBump / n;

  sort(mLearntClause.begin(), mLearntClause.end(), SatClauseLess());

  vector<SatClause*>::iterator wpos = mLearntClause.begin();
  for (ymuint i = 0; i < n2; ++ i) {
    SatClause* clause = mLearntClause[i];
    if ( clause->lit_num() > 2 && !is_locked(clause) ) {
      delete_clause(clause);
    }
    else {
      *wpos = clause;
      ++ wpos;
    }
  }
  for (ymuint i = n2; i < n; ++ i) {
    SatClause* clause = mLearntClause[i];
    if ( clause->lit_num() > 2 && !is_locked(clause) &&
	 clause->activity() < abs_limit ) {
      delete_clause(clause);
    }
    else {
      *wpos = clause;
      ++ wpos;
    }
  }
  if ( wpos != mLearntClause.end() ) {
    mLearntClause.erase(wpos, mLearntClause.end());
  }
}

// 新しい節を生成する．
SatClause*
YmSat::new_clause(ymuint lit_num,
		  bool learnt)
{
  ymuint size = sizeof(SatClause) + sizeof(Literal) * (lit_num - 1);
  void* p = mAlloc.get_memory(size);
  SatClause* clause = new (p) SatClause(lit_num, mTmpLits, learnt);

  return clause;
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

// 節を捨てる．
void
YmSat::delete_clause(SatClause* clause)
{
  assert_cond( clause->is_learnt(), __FILE__, __LINE__);

  // watch list を更新
  del_watcher(~clause->wl0(), SatReason(clause));
  del_watcher(~clause->wl1(), SatReason(clause));

  mLearntLitNum -= clause->lit_num();

  ymuint size = sizeof(SatClause) + sizeof(Literal) * (clause->lit_num() - 1);
  mAlloc.put_memory(size, static_cast<void*>(clause));
}

// 変数のアクティビティを増加させる．
void
YmSat::bump_var_activity(VarId varid)
{
  ymuint vindex = varid.val();
  double& act = mActivity[vindex];
  act += mVarBump;
  if ( act > 1e+100 ) {
    for (ymuint i = 0; i < mVarNum; ++ i) {
      mActivity[i] *= 1e-100;
    }
    mVarBump *= 1e-100;
  }
  ymint pos = mHeapPos[vindex];
  if ( pos >= 0 ) {
    heap_move_up(pos);
  }
}

// 変数のアクティビティを定率で減少させる．
void
YmSat::decay_var_activity()
{
  mVarBump /= mVarDecay;
}

// リスタート時の変数のアクティビティの低減率
void
YmSat::decay_var_activity2()
{
#if 0
  for (ymuint i = 0; i < mVarNum; ++ i) {
    mActivity[i] /= mConflictNum;
  }
  mVarBump = 1.0;
#endif
}

// 学習節のアクティビティを増加させる．
void
YmSat::bump_clause_activity(SatClause* clause)
{
  clause->increase_activity(mClauseBump);
  if ( clause->activity() > 1e+100 ) {
    for (vector<SatClause*>::iterator p = mLearntClause.begin();
	 p != mLearntClause.end(); ++ p) {
      SatClause* clause1 = *p;
      clause1->factor_activity(1e-100);
    }
    mClauseBump *= 1e-100;
  }
}

// 学習節のアクティビティを定率で減少させる．
void
YmSat::decay_clause_activity()
{
  mClauseBump /= mClauseDecay;
}

// 引数の位置にある要素を適当な位置まで沈めてゆく
void
YmSat::heap_move_down(ymuint pos)
{
  ymuint vindex_p = mHeap[pos];
  double val_p = mActivity[vindex_p];
  for ( ; ; ) {
    // ヒープ木の性質から親から子の位置がわかる
    ymuint pos_l = heap_left(pos);
    ymuint pos_r = pos_l + 1;
    if ( pos_r > mHeapNum ) {
      // 左右の子どもを持たない場合
      break;
    }
    ymuint pos_c = pos_l;
    ymuint vindex_c = mHeap[pos_c];
    double val_c = mActivity[vindex_c];
    if ( pos_r < mHeapNum ) {
      ymuint vindex_r = mHeap[pos_r];
      double val_r = mActivity[vindex_r];
      if ( vindex_c < vindex_r ) {
	pos_c = pos_r;
	vindex_c = vindex_r;
	val_c = val_r;
      }
    }
    if ( val_c <= val_p ) {
      break;
    }
    // 逆転
    heap_set(vindex_p, pos_c);
    heap_set(vindex_c, pos);
    pos = pos_c;
  }
}

// @brief 内容を出力する
void
YmSat::heap_dump(ostream& s) const
{
  s << "heap num = " << mHeapNum << endl;
  ymuint j = 0;
  ymuint nc = 1;
  const char* spc = "";
  for (ymuint i = 0; i < mHeapNum; ++ i) {
    ymuint vindex = mHeap[i];
    assert_cond(mHeapPos[vindex] == static_cast<ymint>(i),
		__FILE__, __LINE__);
    if ( i > 0 ) {
      ymint p = (i - 1) >> 1;
      assert_cond(mVal[mHeap[p]] >= mVal[vindex], __FILE__, __LINE__);
    }
    s << spc << vindex << "("
      << mActivity[vindex]
      << ")";
    ++ j;
    if ( j == nc ) {
      j = 0;
      nc <<= 1;
      s << endl;
      spc = "";
    }
    else {
      spc = " ";
    }
  }
  if ( j > 0 ) {
    s << endl;
  }
}

END_NAMESPACE_YM_SAT
