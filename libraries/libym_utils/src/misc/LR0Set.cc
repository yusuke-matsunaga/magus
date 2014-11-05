
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


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// 項集合の閉包を求める．
void
closure(const vector<LR0Term>& input,
	vector<LR0Term>& output)
{
  ymuint n = input.size();
  for (ymuint i = 0; i < n; ++ i) {
    output.push_back(input[i]);
  }

  for (ymuint rpos = 0; rpos < output.size(); ++ rpos) {
    const LR0Term& term = output[rpos];
    const Rule* rule = term.rule();
    ymuint pos = term.pos();
    if ( pos < rule->right_size() ) {
      const Token* head = rule->right(pos);
      const vector<Rule*>& rule_list = head->rule_list();
      for (vector<Rule*>::const_iterator p = rule_list.begin();
	   p != rule_list.end(); ++ p) {
	Rule* rule1 = *p;
	bool found = false;
	for (ymuint i = 0; i < output.size(); ++ i) {
	  if ( output[i] == LR0Term(rule1, 0) ) {
	    found = true;
	    break;
	  }
	}
	if ( !found ) {
	  output.push_back(LR0Term(rule1, 0));
	}
      }
    }
  }
}

// 遷移先の項集合を求める．
void
next_state(LR0State* cur_state,
	   const Token* token,
	   vector<LR0Term> next_terms)
{
  vector<LR0Term> tmp_terms;
  const vector<LR0Term>& cur_terms = cur_state->term_list();
  for (vector<LR0Term>::const_iterator p = cur_terms.begin();
       p != cur_terms.end(); ++ p) {
    const LR0Term& term = *p;
    const Rule* rule = term.rule();
    ymuint pos = term.pos();
    if ( pos < rule->right_size() && rule->right(pos) == token ) {
      tmp_terms.push_back(LR0Term(rule, pos + 1));
    }
    closure(tmp_terms, next_terms);
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス LR0Set
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] grammer 元となる文法
LR0Set::LR0Set(Grammer* grammer)
{
  vector<LR0Term> start_terms;
  const vector<Rule*>& rule_list = grammer->token(0)->rule_list();
  ASSERT_COND ( rule_list.size() == 1 );
  start_terms.push_back(LR0Term(rule_list[0], 0));
  vector<LR0Term> tmp_terms;
  closure(start_terms, tmp_terms);
  mStartState = new_state(tmp_terms);
	  for (ymuint rpos = 0; rpos < mStateList.size(); ++ rpos) {
    LR0State* cur_state = mStateList[rpos];
    const vector<const Token*>& token_list = cur_state->token_list();
    for (vector<const Token*>::const_iterator p = token_list.begin();
	 p != token_list.end(); ++ p) {
      const Token* token = *p;
      vector<LR0Term> tmp_terms;
      next_state(cur_state, token, tmp_terms);
      LR0State* state1 = new_state(tmp_terms);
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
// @param[in] terms 状態を表す項集合
// @return 対応する状態を返す．
//
// すでに等価は状態が存在したらその状態を返す．
LR0State*
LR0Set::new_state(const vector<LR0Term>& terms)
{
  for (ymuint i = 0; i < mStateList.size(); ++ i) {
    LR0State* state1 = mStateList[i];
    if ( state1->term_list() == terms ) {
      // すでに同じ状態が存在した．
      return state1;
    }
  }
  LR0State* state = new LR0State(terms);
  mStateList.push_back(state);
  return state;
}

END_NAMESPACE_YM
