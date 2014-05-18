
/// @file YmSat.cc
/// @brief YmSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmSat.h"
#include "logic/SatStats.h"
#include "logic/SatMsgHandler.h"
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

BEGIN_NONAMESPACE

// Luby restart strategy
double
luby(double y,
     int x)
{
  // なんのこっちゃわかんないコード
  int size;
  int seq;
  for (size = 1, seq = 0; size < x + 1; ++ seq, size = size * 2 + 1) ;

  while ( size - 1 != x ) {
    size = (size - 1) >> 1;
    -- seq;
    x = x % size;
  }

  return pow(y, seq);
}

END_NONAMESPACE

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

  // メッセージハンドラにヘッダの出力を行わせる．
  for (list<SatMsgHandler*>::iterator p = mMsgHandlerList.begin();
       p != mMsgHandlerList.end(); ++ p) {
    SatMsgHandler& handler = *(*p);
    handler.print_header();
  }

  if ( mTimerOn ) {
    mTimer.stop();
    mTimer.reset();
    mTimer.start();
  }

  // 変数領域の確保を行う．
  alloc_var();

  // パラメータの初期化
  double confl_limit = 100;
  double restart_inc = 2;
  double learnt_limit = clause_num() / 3.0;
  mVarHeap.set_decay(mParams.mVarDecay);
  mClauseDecay = mParams.mClauseDecay;

  Bool3 sat_stat = kB3X;

  // 自明な簡単化を行う．
  sweep_clause();
  if ( !mSane ) {
    // その時点で充足不可能なら終わる．
    sat_stat = kB3False;
    goto end;
  }

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

      sat_stat = kB3False;
      goto end;
    }
  }

  // 以降，現在のレベルが基底レベルとなる．
  mRootLevel = decision_level();
  if ( debug & (debug_assign | debug_decision) ) {
    cout << "RootLevel = " << mRootLevel << endl;
  }

  for ( ; ; ) {
    // 実際の探索を行う．
    mConflictLimit = static_cast<ymuint64>(confl_limit);
    mConflictLimit = static_cast<ymuint64>(luby(restart_inc, mRestart)) * 100;
    if ( mConflictLimit > mMaxConflict ) {
      mConflictLimit = mMaxConflict;
    }
    mLearntLimit = static_cast<ymuint64>(learnt_limit);

    ++ mRestart;
    sat_stat = search(mConflictLimit);

    // メッセージ出力を行う．
    SatStats stats;
    get_stats(stats);
    for (list<SatMsgHandler*>::iterator p = mMsgHandlerList.begin();
	 p != mMsgHandlerList.end(); ++ p) {
      SatMsgHandler& handler = *(*p);
      handler.print_message(stats);
    }

    if ( sat_stat != kB3X ) {
      // 結果が求められた．
      break;
    }
    if ( mConflictLimit == mMaxConflict ) {
      // 制限値に達した．(アボート)
      break;
    }

    // 判定できなかったのでパラメータを更新して次のラウンドへ
    confl_limit = confl_limit * 1.5;
    learnt_limit = learnt_limit + 100;
  }
  if ( sat_stat == kB3True ) {
    // SAT ならモデル(充足させる変数割り当てのリスト)を作る．
    model.resize(mVarNum);
    for (ymuint i = 0; i < mVarNum; ++ i) {
      Bool3 val = cur_val(mVal[i]);
      assert_cond(val != kB3X, __FILE__, __LINE__);
      model[i] = val;
    }
  }
  backtrack(0);

  if ( mTimerOn ) {
    mTimer.stop();
  }

 end:

  // メッセージハンドラに終了メッセージを出力させる．
  {
    SatStats stats;
    get_stats(stats);
    for (list<SatMsgHandler*>::iterator p = mMsgHandlerList.begin();
	 p != mMsgHandlerList.end(); ++ p) {
      SatMsgHandler& handler = *(*p);
      handler.print_footer(stats);
    }
  }

  if ( debug & debug_solve ) {
    switch ( sat_stat ) {
    case kB3True:  cout << "SAT" << endl; break;
    case kB3False: cout << "UNSAT" << endl; break;
    case kB3X:     cout << "UNKNOWN" << endl; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
  }

  return sat_stat;
}

// @brief 学習節の整理を行なう．
void
YmSat::reduce_learnt_clause()
{
  cut_down();
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
  return mConstrClause.size() + mConstrBinNum;
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

// 探索を行う本体の関数
Bool3
YmSat::search(ymuint confl_limit)
{
  ymuint confl_num0 = mConflictNum;
  for ( ; ; ) {
    if ( (mConflictNum - confl_num0) > confl_limit ) {
      // 矛盾の回数が制限値を越えた．
      backtrack(mRootLevel);
      return kB3X;
    }

    // キューにつまれている割り当てから含意される値の割り当てを行う．
    SatReason conflict = implication();
    if ( conflict != kNullSatReason ) {
      // 矛盾が生じた．
      ++ mConflictNum;
      if ( decision_level() == mRootLevel ) {
	// トップレベルで矛盾が起きたら充足不可能
	return kB3False;
      }

      // 今の矛盾の解消に必要な条件を「学習」する．
      int bt_level = mAnalyzer->analyze(conflict, mLearntLits);

      if ( debug & debug_analyze ) {
	cout << endl
	     << "analyze for " << conflict << endl
	     << "learnt clause is ";
	const char* plus = "";
	for (ymuint i = 0; i < mLearntLits.size(); ++ i) {
	  Literal l = mLearntLits[i];
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
      add_learnt_clause();

      decay_var_activity();
      decay_clause_activity();
    }
    else {
      if ( decision_level() == 0 ) {
	// 一見，無意味に思えるが，学習節を追加した結果，真偽値が確定する節が
	// あるかもしれないのでそれを取り除く．
	sweep_clause();
      }
      if ( mLearntClause.size() > mLearntLimit ) {
	// 学習節の数が制限値を超えたら整理する．
	cut_down();
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

      if ( debug & (debug_assign | debug_decision) ) {
	cout << endl
	     << "choose " << lit << " :"
	     << mVarHeap.activity(lit.varid()) << endl;
      }
      // 未割り当ての変数を選んでいるのでエラーになるはずはない．
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
	SatClause* c = w.clause();
	Literal l0 = c->wl0();
	if ( l0 == nl ) {
	  // nl を 1番めのリテラルにする．
	  c->xchange_wl();
	  // 新しい wl0 を得る．
	  l0 = c->wl0();
	}
	else { // l1 == nl
	  if ( debug & debug_implication ) {
	    // この assert は重いのでデバッグ時にしかオンにしない．
	    // ※ debug と debug_implication が const なので結果が0の
	    // ときにはコンパイル時に消されることに注意
	    assert_cond(c->wl1() == nl, __FILE__, __LINE__);
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
	ymuint n = c->lit_num();
	for (ymuint i = 2; i < n; ++ i) {
	  Literal l2 = c->lit(i);
	  Bool3 v = eval(l2);
	  if ( v != kB3False ) {
	    // l2 を 1番めの watch literal にする．
	    c->xchange_wl1(i);
	    if ( debug & debug_implication ) {
	      cout << "\t\t\tsecond watching literal becomes "
		   << l2 << endl;
	    }
	    // l の watcher list から取り除く
	    -- wpos;
	    // ~l2 の watcher list に追加する．
	    add_watcher(~l2, w);

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

	  if ( mParams.mUseLbd ) {
	    ymuint lbd = calc_lbd(c) + 1;
	    if ( c->lbd() > lbd ) {
	      c->set_lbd(lbd);
	    }
	  }
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
      mVal[vindex] = (mVal[vindex] << 2) | conv_from_Bool3(kB3X);
      mVarHeap.push(varid);
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
  // 一定確率でランダムな変数を選ぶ．
  if ( mRandGen.real1() < mParams.mVarFreq && !mVarHeap.empty() ) {
    ymuint pos = mRandGen.int32() % mVarNum;
    VarId vid(pos);
    if ( eval(VarId(vid)) == kB3X ) {
      bool inv = mRandGen.real1() < 0.5;
      return Literal(vid, inv);
    }
  }

  while ( !mVarHeap.empty() ) {
    // activity の高い変数を取り出す．
    ymuint vindex = mVarHeap.pop_top();
    ymuint8 x = mVal[vindex];
    if ( (x & 3U) != conv_from_Bool3(kB3X) ) {
      // すでに確定していたらスキップする．
      // もちろん，ヒープからも取り除く．
      continue;
    }

    bool inv = false;
    ymuint8 old_val = (x >> 2) & 3U;
    if ( mParams.mPhaseCache && old_val != conv_from_Bool3(kB3X) ) {
      // 以前割り当てた極性を選ぶ
      if ( old_val == conv_from_Bool3(kB3False) ) {
	inv = true;
      }
    }
    else {
      ymuint v2 = vindex * 2;
      if ( mParams.mWlPosi ) {
	// Watcher の多い方の極性を(わざと)選ぶ
	if ( mWatcherList[v2 + 1].num() >= mWatcherList[v2 + 0].num() ) {
	  inv = true;
	}
      }
      else if ( mParams.mWlNega ) {
	// Watcher の少ない方の極性を選ぶ
	if ( mWatcherList[v2 + 1].num() < mWatcherList[v2 + 0].num() ) {
	  inv = true;
	}
      }
      else {
	// mWlPosi/mWlNega が指定されていなかったらランダムに選ぶ．
	inv = mRandGen.real1() < 0.5;
      }
#if 0
      //cout << mWeightArray[v2 + 0] << " : " << mWeightArray[v2 + 1] << endl;
      if ( mWeightArray[v2 + 1] > mWeightArray[v2 + 0] ) {
	inv = true;
      }
#else
      inv = false; // 意味はない．
#endif
    }
    return Literal(VarId(vindex), inv);
  }
  return kLiteralX;
}

// CNF を簡単化する．
void
YmSat::sweep_clause()
{
  if ( !mSane ) {
    return;
  }
  assert_cond(decision_level() == 0, __FILE__, __LINE__);

  if ( implication() != kNullSatReason ) {
    mSane = false;
    return;
  }

  ymuint n = mLearntClause.size();
  ymuint wpos = 0;
  for (ymuint rpos = 0; rpos < n; ++ rpos) {
    SatClause* c = mLearntClause[rpos];
    ymuint nl = c->lit_num();
    bool satisfied = false;
    for (ymuint i = 0; i < nl; ++ i) {
      if ( eval(c->lit(i)) == kB3True ) {
	satisfied = true;
	break;
      }
    }
    if ( satisfied ) {
      // c を削除する．
      delete_clause(c);
    }
    else {
      if ( wpos != rpos ) {
	mLearntClause[wpos] = c;
      }
      ++ wpos;
    }
  }
  if ( wpos != n ) {
    mLearntClause.erase(mLearntClause.begin() + wpos, mLearntClause.end());
  }

  if( true ) {
    ymuint n = mConstrClause.size();
    ymuint wpos = 0;
    for (ymuint rpos = 0; rpos < n; ++ rpos) {
      SatClause* c = mConstrClause[rpos];
      ymuint nl = c->lit_num();
      bool satisfied = false;
      for (ymuint i = 0; i < nl; ++ i) {
	if ( eval(c->lit(i)) == kB3True ) {
	  satisfied = true;
	  break;
	}
      }
      if ( satisfied ) {
	// c を削除する．
	delete_clause(c);
      }
      else {
	if ( wpos != rpos ) {
	  mConstrClause[wpos] = c;
	}
	++ wpos;
      }
    }
    if ( wpos != n ) {
      mConstrClause.erase(mConstrClause.begin() + wpos, mConstrClause.end());
    }
  }
}

BEGIN_NONAMESPACE
// cut_down で用いる SatClause の比較関数
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
YmSat::cut_down()
{
  ymuint n = mLearntClause.size();
  ymuint n2 = n / 2;

  // 足切りのための制限値
  double abs_limit = mClauseBump / n;

  sort(mLearntClause.begin(), mLearntClause.end(), SatClauseLess());

  vector<SatClause*>::iterator wpos = mLearntClause.begin();
  for (ymuint i = 0; i < n2; ++ i) {
    SatClause* clause = mLearntClause[i];
    if ( clause->lit_num() > 2 && clause->lbd() > 2 && !is_locked(clause) ) {
      delete_clause(clause);
    }
    else {
      *wpos = clause;
      ++ wpos;
    }
  }
  for (ymuint i = n2; i < n; ++ i) {
    SatClause* clause = mLearntClause[i];
    if ( clause->lit_num() > 2 && clause->lbd() > 2 && !is_locked(clause) &&
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

  for (ymuint i = 0; i < lit_num; ++ i) {
    Literal l = mTmpLits[i];
    ymuint index = l.index();
    mWeightArray[index] += 1.0 / static_cast<double>(lit_num);
  }

  Literal l1 = mTmpLits[1];

  if ( lit_num == 2 ) {
    // watcher-list の設定
    add_watcher(~l0, SatReason(l1));
    add_watcher(~l1, SatReason(l0));

    // binary clause は watcher-list に登録するだけで実体はない．
    ++ mConstrBinNum;
  }
  else {
    // 節の生成
    SatClause* clause = new_clause(lit_num);
    mConstrClause.push_back(clause);

    // watcher-list の設定
    add_watcher(~l0, SatReason(clause));
    add_watcher(~l1, SatReason(clause));
  }
}

// 学習節を追加する．
void
YmSat::add_learnt_clause()
{
  // 学習節の内容は mLeartLits に格納されている．
  // 結果，0 番目のリテラルに値の割り当てが生じる．
  // 残りのリテラルがその割り当ての原因となる．

  ymuint n = mLearntLits.size();
  mLearntLitNum += n;

  if ( n == 0 ) {
    // empty clause があったら unsat
    mSane = false;
    return;
  }

  Literal l0 = mLearntLits[0];
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
  Literal l1 = mLearntLits[1];
  if ( n == 2 ) {
    // binary-clause の場合
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
      mTmpLits[i] = mLearntLits[i];
    }
    SatClause* clause = new_clause(n, true);

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
  assert_cond(eval(l0) != kB3False, __FILE__, __LINE__);
  if ( debug & debug_assign ) {
    cout << "\tassign " << l0 << " @" << decision_level()
	 << " from " << reason << endl;
  }

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
  assert_cond( wpos < n, __FILE__, __LINE__);
  -- n;
  for ( ; wpos < n; ++ wpos) {
    Watcher w = wlist.elem(wpos + 1);
    wlist.set_elem(wpos, w);
  }
  wlist.erase(n);
}

// @brief LBD を計算する．
ymuint
YmSat::calc_lbd(const SatClause* clause)
{
  ymuint max_level = decision_level() + 1;
  ymuint32 old_size = mLbdTmpSize;
  while ( mLbdTmpSize < max_level ) {
    mLbdTmpSize <<= 1;
  }
  if ( mLbdTmpSize != old_size ) {
    delete [] mLbdTmp;
    mLbdTmp = new bool[mLbdTmpSize];
  }

  ymuint n = clause->lit_num();

  // mLbdTmp をクリア
  for (ymuint i = 0; i < n; ++ i) {
    Literal l = clause->lit(i);
    VarId v = l.varid();
    ymuint level = decision_level(v);
    mLbdTmp[level] = false;
  }

  // 異なる決定レベルの個数を数える．
  ymuint c = 0;
  for (ymuint i = 0; i < n; ++ i) {
    Literal l = clause->lit(i);
    VarId v = l.varid();
    ymuint level = decision_level(v);
    if ( !mLbdTmp[level] ) {
      mLbdTmp[level] = true;
      ++ c;
    }
  }

  return c;
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

END_NAMESPACE_YM_SAT
