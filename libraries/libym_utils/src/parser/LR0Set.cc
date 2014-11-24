
/// @file LR0Set.cc
/// @brief LR0Set の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LR0Set.h"
#include "Grammer.h"
#include "LR0State.h"
#include "LR0Term.h"
#include "Rule.h"
#include "Token.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const int debug = 0;

// LR0Term の比較関数
struct LR0TermLt
{
  bool
  operator()(const LR0Term& left,
	     const LR0Term& right)
  {
    ymuint l_id = left.rule()->id();
    ymuint r_id = right.rule()->id();
    if ( l_id < r_id ) {
      return true;
    }
    if ( l_id > r_id ) {
      return false;
    }
    // l_id == r_id

    ymuint l_pos = left.dot_pos();
    ymuint r_pos = right.dot_pos();
    if ( l_pos < r_pos ) {
      return true;
    }
    return false;
  }
};

// @brief 項集合の閉包を求める．
// @param[in] input 入力の項集合
// @param[out] output 閉包集合
void
closure(const vector<LR0Term>& input,
	vector<LR0Term>& output)
{
  if ( debug ) {
    cout << "LR(0) closure:" << endl;
    for (vector<LR0Term>::const_iterator p = input.begin();
	 p != input.end(); ++ p) {
      cout << *p << endl;
    }
    cout << endl;
  }

  // 入力をコピーする．
  ymuint n = input.size();
  for (ymuint i = 0; i < n; ++ i) {
    output.push_back(input[i]);
  }

  // output は次に処理すべき項を入れるキューとしても機能している．
  // rpos が次に処理する項の位置を示す．
  // これが末尾に達したら新たに加わった項がないということ．

  // 処理したトークン番号を入れるハッシュ表
  HashSet<ymuint> token_set;
  for (ymuint rpos = 0; rpos < output.size(); ++ rpos) {
    const LR0Term& term = output[rpos];
    const Token* next_token = term.next_token();
    if ( next_token == NULL ) {
      continue;
    }
    if ( token_set.check(next_token->id()) ) {
      continue;
    }
    token_set.add(next_token->id());

    // dot の次のトークンを左辺に持つ規則を列挙する．
    const vector<const Rule*>& rule_list = next_token->rule_list();
    for (vector<const Rule*>::const_iterator p = rule_list.begin();
	 p != rule_list.end(); ++ p) {
      const Rule* rule1 = *p;
      // rule1 を output に加える．
      output.push_back(LR0Term(rule1, 0));
    }
  }

  // ソートしておく．
  sort(output.begin(), output.end(), LR0TermLt());

  if ( debug ) {
    cout << "LR(0) closure end:" << endl;
    for (vector<LR0Term>::const_iterator p = output.begin();
	 p != output.end(); ++ p) {
      cout << *p << endl;
    }
    cout << endl;
  }
}

// @brief 遷移先の項集合を求める．
// @param[in] cur_state 現在の状態
// @param[in] token 次のトークン
// @param[out] next_terms 遷移先の状態を表す項集合
void
next_state(LR0State* cur_state,
	   const Token* token,
	   vector<LR0Term>& next_terms)
{
  // 次のトークンが token に等しい項の dot を進めた項を tmp_terms に入れる．
  vector<LR0Term> tmp_terms;
  const vector<LR0Term>& cur_terms = cur_state->term_list();
  for (vector<LR0Term>::const_iterator p = cur_terms.begin();
       p != cur_terms.end(); ++ p) {
    const LR0Term& term = *p;
    if ( term.next_token() == token ) {
      const Rule* rule = term.rule();
      ymuint pos = term.dot_pos();
      tmp_terms.push_back(LR0Term(rule, pos + 1));
    }
  }
  // その閉包を求める．
  closure(tmp_terms, next_terms);
}

END_NONAMESPACE

// シグネチャのハッシュ関数
template<>
struct
HashFunc<vector<ymuint64> >
{
  ymuint
  operator()(const vector<ymuint64>& key) const
  {
    ymuint ans = 0;
    for (vector<ymuint64>::const_iterator p = key.begin();
	 p != key.end(); ++ p) {
      ymuint64 tmp = *p;
      ans += (ans * 1023);
      ans += static_cast<ymuint>(tmp & 0xFFFFFFFFU);
      ans += static_cast<ymuint>(tmp >> 32) * 511;
    }
    return ans;
  }
};


//////////////////////////////////////////////////////////////////////
// クラス LR0Set
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] grammer 元となる文法
LR0Set::LR0Set(Grammer* grammer)
{
  // 初期状態は明示的に作る．
  // start_state = closure( {S'-> . S} )
  vector<LR0Term> start_terms;
  const vector<const Rule*>& rule_list = grammer->token(0)->rule_list();
  ASSERT_COND ( rule_list.size() == 1 );
  start_terms.push_back(LR0Term(rule_list[0], 0));
  vector<LR0Term> tmp_terms;
  closure(start_terms, tmp_terms);

  HashMap<vector<ymuint64>, LR0State*> state_hash;
  mStartState = new_state(grammer, state_hash, tmp_terms);

  // mStateList に未処理の状態が残っている限り以下の処理を繰り返す．
  for (ymuint rpos = 0; rpos < mStateList.size(); ++ rpos) {
    LR0State* cur_state = mStateList[rpos];
    if ( debug ) {
      cur_state->print(cout);
      cout << endl;
    }
    // cur_state に関係するトークンを取り出す．
    const vector<const Token*>& token_list = cur_state->token_list();
    for (vector<const Token*>::const_iterator p = token_list.begin();
	 p != token_list.end(); ++ p) {
      const Token* token = *p;
      // token に対する次状態を求める．
      vector<LR0Term> tmp_terms;
      next_state(cur_state, token, tmp_terms);
      // tmp_terms に対応する状態を作る．
      // 場合によっては既存の状態を再利用する．
      LR0State* state1 = new_state(grammer, state_hash, tmp_terms);
      // それを cur_state の遷移先に設定する．
      cur_state->add_next_state(token, state1);
    }
  }
}

// @brief デストラクタ
LR0Set::~LR0Set()
{
  for (vector<LR0State*>::iterator p = mStateList.begin();
       p != mStateList.end(); ++ p) {
    delete *p;
  }
}

// @brief 状態のリストを返す．
const vector<LR0State*>&
LR0Set::state_list() const
{
  return mStateList;
}

// @brief 初期状態を返す．
LR0State*
LR0Set::start_state() const
{
  return mStartState;
}

// @brief 状態を追加する．
// @param[in] grammer 元となる文法
// @param[in] terms 状態を表す項集合
// @return 対応する状態を返す．
//
// すでに等価は状態が存在したらその状態を返す．
LR0State*
LR0Set::new_state(Grammer* grammer,
		  HashMap<vector<ymuint64>, LR0State*>& state_map,
		  const vector<LR0Term>& terms)
{
  // シグネチャを作る．
  ymuint n = grammer->term_size();
  ymuint ns = (n + 63) / 64;
  vector<ymuint64> sig(ns, 0UL);
  for (vector<LR0Term>::const_iterator p = terms.begin();
       p != terms.end(); ++ p) {
    ymuint term_id = grammer->term_id(p->rule()->id(), p->dot_pos());
    ymuint blk = term_id / 64;
    ymuint sft = term_id % 64;
    sig[blk] |= (1UL << sft);
  }

  // ハッシュ表に存在するか調べる．
  LR0State* state;
  if ( state_map.find(sig, state) ) {
    // 見つかった．
    return state;
  }

  // なかったので新たに作る．
  ymuint id = mStateList.size();
  state = new LR0State(id, terms);
  mStateList.push_back(state);
  state_map.add(sig, state);
  return state;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
LR0Set::print(ostream& s) const
{
  for (vector<LR0State*>::const_iterator p = mStateList.begin();
       p != mStateList.end(); ++ p) {
    LR0State* state = *p;
    state->print(s);
  }
}

END_NAMESPACE_YM
