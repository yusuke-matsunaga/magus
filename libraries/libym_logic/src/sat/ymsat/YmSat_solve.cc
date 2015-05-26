
/// @file YmSat_solve.cc
/// @brief YmSat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmSat.h"
#include "YmLogic/SatStats.h"
#include "YmLogic/SatMsgHandler.h"
#include "SatAnalyzer.h"
#include "SatClause.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// YmSat
//////////////////////////////////////////////////////////////////////

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
    cout << " VarNum: " << mVarNum << endl;
  }

  model.clear();
  model.resize(mVarNum, kB3X);

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

  // 最終的な結果を納める変数
  Bool3 sat_stat = kB3X;

  ASSERT_COND( decision_level() == 0 );

  // 自明な簡単化を行う．
  reduce_CNF();
  if ( !mSane ) {
    // その時点で充足不可能なら終わる．
    sat_stat = kB3False;
    goto end;
  }

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

    if ( stat ) {
      // 今の割当に基づく含意を行う．
      SatReason reason = implication();
      if ( reason != kNullSatReason ) {
	// 矛盾が起こった．
	stat = false;
      }
    }

    if ( !stat ) {
      // 矛盾が起こった．
      backtrack(0);
      sat_stat = kB3False;
      goto end;
    }
  }

  // 充足した節があれば取り除く．
  reduce_CNF();

  // 以降，現在のレベルが基底レベルとなる．
  mRootLevel = decision_level();
  if ( debug & (debug_assign | debug_decision) ) {
    cout << "RootLevel = " << mRootLevel << endl;
  }

  for ( ; ; ) {
    // search() の制限値の設定
    confl_limit = static_cast<ymuint64>(luby(restart_inc, mRestart)) * 100;
    mLearntLimit = static_cast<ymuint64>(learnt_limit);

    ++ mRestart;

    // 探索の本体
    sat_stat = search(confl_limit);

    // get_stats() で用いる．
    mConflictLimit = static_cast<ymuint64>(confl_limit);

    // メッセージ出力を行う．
    {
      SatStats stats;
      get_stats(stats);
      for (list<SatMsgHandler*>::iterator p = mMsgHandlerList.begin();
	   p != mMsgHandlerList.end(); ++ p) {
	SatMsgHandler& handler = *(*p);
	handler.print_message(stats);
      }
    }

    if ( sat_stat != kB3X ) {
      // 結果が求められた．
      break;
    }

    if ( mConflictNum == mMaxConflict ) {
      // 制限値に達した．(アボート)
      break;
    }

    if ( debug & debug_assign ) {
      cout << "restart" << endl;
    }

    // 判定できなかったのでパラメータを更新して次のラウンドへ
#if 0
    confl_limit = confl_limit * 1.5;
#endif
    learnt_limit = learnt_limit + 100;
  }

  if ( sat_stat == kB3True ) {
    // SAT ならモデル(充足させる変数割り当てのリスト)を作る．
    for (ymuint i = 0; i < mVarNum; ++ i) {
      Bool3 val = cur_val(mVal[i]);
      ASSERT_COND(val != kB3X );
      model[i] = val;
    }
  }
  // 最初の状態に戻す．
  backtrack(0);

  if ( mTimerOn ) {
    mTimer.stop();
  }

 end:

  // 終了メッセージを出力させる．
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
    default: ASSERT_NOT_REACHED;
    }
  }

  return sat_stat;
}

// 探索を行う本体の関数
Bool3
YmSat::search(ymuint confl_limit)
{
  ymuint cur_confl_num = 0;
  for ( ; ; ) {
    // キューにつまれている割り当てから含意される値の割り当てを行う．
    SatReason conflict = implication();
    if ( conflict != kNullSatReason ) {
      // 矛盾が生じた．
      ++ mConflictNum;
      ++ cur_confl_num;
      if ( decision_level() == mRootLevel ) {
	// トップレベルで矛盾が起きたら充足不可能
	return kB3False;
      }

      // 今の矛盾の解消に必要な条件を「学習」する．
      vector<Literal> learnt_lits;
      int bt_level = mAnalyzer->analyze(conflict, learnt_lits);

      if ( debug & debug_analyze ) {
	cout << endl
	     << "analyze for " << conflict << endl
	     << endl
	     << "learnt clause is ";
	const char* plus = "";
	for (ymuint i = 0; i < learnt_lits.size(); ++ i) {
	  Literal l = learnt_lits[i];
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
      add_learnt_clause(learnt_lits);

      decay_var_activity();
      decay_clause_activity();

      continue;
    }

    if ( cur_confl_num >= confl_limit ) {
      // 矛盾の回数が制限値を越えた．
      backtrack(mRootLevel);
      return kB3X;
    }

    if ( decision_level() == 0 ) {
      // 一見，無意味に思えるが，学習節を追加した結果，真偽値が確定する節が
      // あるかもしれないのでそれを取り除く．
      reduce_CNF();
    }

    if ( mLearntClause.size() >  mAssignList.size() + mLearntLimit ) {
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

    if ( debug & debug_assign ) {
      cout << "\tassign " << lit << " @" << decision_level() << endl;
    }

    // 選ばれたリテラルに基づいた割当を行う．
    // 未割り当ての変数を選んでいるのでエラーになるはずはない．
    assign(lit);
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
    -- mSweep_props;

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
	if ( val0 == kB3True ) {
	  // すでに充足していた．
	  continue;
	}
	if ( debug & debug_assign ) {
	  cout << "\tassign " << l0 << " @" << decision_level()
	       << " from (" << l0
	       << " + " << ~l << "): " << l << endl;
	}
	if ( val0 == kB3X ) {
	  assign(l0, SatReason(nl));
	}
	else { // val0 == kB3False
	  // 矛盾がおこった．
	  if ( debug & debug_assign ) {
	    cout << "\t--> conflict(#" << mConflictNum << ") with previous assignment" << endl
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
	  if ( eval(c->wl1()) == kB3True ) {
	    continue;
	  }
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
	    ASSERT_COND(c->wl1() == nl );
	  }
	}

	Bool3 val0 = eval(l0);
	if ( val0 == kB3True ) {
	  // すでに充足していた．
	  continue;
	}

	if ( debug & debug_implication ) {
	  cout << "\t\texamining watcher clause " << (*c) << endl;
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
	if ( debug & debug_assign ) {
	  cout << "\tassign " << l0 << " @" << decision_level()
	       << " from " << w << ": " << l << endl;
	}
	if ( val0 == kB3X ) {
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
	    cout << "\t--> conflict(#" << mConflictNum << ") with previous assignment" << endl
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
    ASSERT_COND( vindex < mVarNum );
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
      inv = true; // 意味はない．
#endif
    }
    return Literal(VarId(vindex), inv);
  }
  return kLiteralX;
}

// CNF を簡単化する．
void
YmSat::reduce_CNF()
{
  if ( !mSane ) {
    return;
  }
  ASSERT_COND(decision_level() == 0 );

  if ( implication() != kNullSatReason ) {
    mSane = false;
    return;
  }

  if ( mAssignList.size() == mSweep_assigns /*|| mSweep_props > 0*/ ) {
    // 前回から変化がなかったらスキップする．
    return;
  }

  // 制約節をスキャンする
  sweep_clause(mConstrClause);

  // 学習節をスキャンする．
  sweep_clause(mLearntClause);

  // 変数ヒープを再構成する．
  vector<VarId> var_list;
  var_list.reserve(mVarNum);
  for (ymuint i = 0; i < mVarNum; ++ i) {
    VarId var(i);
    if ( eval(var) == kB3X ) {
      var_list.push_back(VarId(i));
    }
    else {
      del_satisfied_watcher(Literal(var, false));
      del_satisfied_watcher(Literal(var, true));
    }
  }
  mVarHeap.build(var_list);

  // 現在の状況を記録しておく．
  mSweep_assigns = mAssignList.size();
  mSweep_props = mConstrLitNum + mLearntLitNum;
}

// @brief 充足している節を取り除く
// @param[in] clause_list 節のリスト
void
YmSat::sweep_clause(vector<SatClause*>& clause_list)
{
  ymuint n = clause_list.size();
  ymuint wpos = 0;
  for (ymuint rpos = 0; rpos < n; ++ rpos) {
    SatClause* c = clause_list[rpos];
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
	clause_list[wpos] = c;
      }
      ++ wpos;
    }
  }
  if ( wpos != n ) {
    clause_list.erase(clause_list.begin() + wpos, clause_list.end());
  }
}

// @brief 学習節の整理を行なう．
void
YmSat::reduce_learnt_clause()
{
  cut_down();
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

  // SatClauseLess を用いて学習節をソートする．
  sort(mLearntClause.begin(), mLearntClause.end(), SatClauseLess());

  vector<SatClause*>::iterator wpos = mLearntClause.begin();

  // 前半の節は基本削除する．
  // 残す節は，
  // - binary clause (今の実装では SatClause の形にはなっていない)
  // - LBD が2以下の節
  // - 現在の割当の理由となっている節
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

  // 残りの節はアクティビティが規定値以下の節を削除する．
  // ただし，上と同じ例外はある．
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

  // vector を切り詰める．
  if ( wpos != mLearntClause.end() ) {
    mLearntClause.erase(wpos, mLearntClause.end());
  }
}

// @brief 学習節をすべて削除する．
void
YmSat::forget_learnt_clause()
{
  // こんな乱暴なことができるは初期状態だけ．
  ASSERT_COND( decision_level() == 0 );

  // 学習節を本当に削除する．
  for (vector<SatClause*>::iterator p = mLearntClause.begin();
       p != mLearntClause.end(); ++ p) {
    SatClause* clause = *p;
    delete_clause(clause);
  }
  mLearntClause.clear();

  // 変数ヒープも再構成する．
  // どうじに変数の履歴もリセットする．
  mVarHeap.reset_activity();
  vector<VarId> var_list;
  var_list.reserve(mVarSize);
  for (ymuint i = 0; i < mVarSize; ++ i) {
    var_list.push_back(VarId(i));
    mVal[i] = conv_from_Bool3(kB3X) | (conv_from_Bool3(kB3X) << 2);
  }
  mVarHeap.build(var_list);
}

// @brief LBD を計算する．
// @param[in] clause 対象の節
ymuint
YmSat::calc_lbd(const SatClause* clause)
{
  // 割当レベルの最大値 + 1 だけ mLbdTmp を確保する．
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
  // ただし， clause に現れるリテラルのレベルだけでよい．
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
      // はじめてこのレベルの変数が現れた．
      mLbdTmp[level] = true;
      ++ c;
    }
  }

  return c;
}

// 学習節のアクティビティを増加させる．
// @param[in] clause 対象の節
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

END_NAMESPACE_YM_SAT
