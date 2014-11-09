
/// @file LALR1Set.cc
/// @brief LALR1Set の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LALR1Set.h"
#include "Grammer.h"
#include "LR0State.h"
#include "LR0Term.h"
#include "LR1Term.h"
#include "Rule.h"
#include "Token.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

void
LR1_closure(Grammer* grammer,
	    const vector<LR1Term>& input,
	    vector<LR1Term>& output)
{
  for (vector<LR1Term>::const_iterator p = input.begin();
       p != input.end(); ++ p) {
    output.push_back(*p);
  }
  for (ymuint rpos = 0; rpos < output.size(); ++ rpos) {
    const LR1Term& term = output[rpos];
    const Rule* rule = term.rule();
    ymuint pos = term.dot_pos();
    const Token* token = term.token();
    const Token* next_token = term.next_token();
    if ( next_token == NULL ) {
      continue;
    }
    vector<const Token*> rest;
    for (ymuint i = pos + 1; i < rule->right_size(); ++ i) {
      rest.push_back(rule->right(i));
    }
    rest.push_back(token);
    vector<const Token*> flist;
    grammer->first_of(rest, flist);
    const vector<const Rule*>& rule_list = next_token->rule_list();
    for (vector<const Rule*>::const_iterator q = rule_list.begin();
	 q != rule_list.end(); ++ q) {
      const Rule* rule2 = *q;
      for (vector<const Token*>::const_iterator r = flist.begin();
	   r != flist.end(); ++ r) {
	output.push_back(LR1Term(rule2, 0, *r));
      }
    }
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス LALR1Set
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] grammer 元となる文法
LALR1Set::LALR1Set(Grammer* grammer) :
  LR0Set(grammer)
{
  mTermNum = 0;
  for (vector<LR0State*>::const_iterator p = state_list().begin();
       p != state_list().end(); ++ p) {
    LR0State* state = *p;
    ASSERT_COND( state->id() == mTermIdTop.size() );
    mTermIdTop.push_back(mTermNum);
    mTermNum += state->term_list().size();
  }
  for (ymuint i = 0; i < mTermNum; ++ i) {
    mTokenList.push_back(vector<const Token*>(0));
  }

  // 先読みの計算をする．
  vector<pair<ymuint, const Token*> > gen_list;
  vector<vector<ymuint> > prop_list(mTermNum, vector<ymuint>(0));
  const Rule* start_rule = grammer->start_rule();
  const Token* dummy = grammer->token(Grammer::kNotExist);
  for (vector<LR0State*>::const_iterator p = state_list().begin();
       p != state_list().end(); ++ p) {
    LR0State* state = *p;
    const vector<LR0Term>& term_list = state->term_list();
    ymuint n = term_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      const LR0Term& term = term_list[i];
      const Rule* rule = term.rule();
      ymuint pos = term.dot_pos();
      if ( rule != start_rule && pos == 0 ) {
	// 非カーネル項は除外する．
	continue;
      }
      vector<LR1Term> tmp_list;
      LR1_closure(grammer, vector<LR1Term>(1, LR1Term(rule, pos, dummy)), tmp_list);
      for (vector<LR1Term>::iterator q = tmp_list.begin();
	   q != tmp_list.end(); ++ q) {
	const Rule* rule1 = q->rule();
	ymuint pos1 = q->dot_pos();
	const Token* token1 = q->token();
	const Token* next_token = q->next_token();
	if ( next_token == NULL ) {
	  continue;
	}
	LR0State* state2 = state->next_state(next_token);
	ASSERT_COND( state2 != NULL );
	const vector<LR0Term>& term_list2 = state2->term_list();
	ymuint n2 = term_list2.size();
	if ( token1 == dummy ) {
	  // 先読みの伝搬
	  for (ymuint i2 = 0; i2 < n2; ++ i2) {
	    const LR0Term& term2 = term_list2[i2];
	    if ( term2.rule() == rule1 && term2.dot_pos() == pos1 + 1 ) {
	      ymuint src_id = calc_term_id(state->id(), i);
	      ymuint dst_id = calc_term_id(state2->id(), i2);
	      prop_list[src_id].push_back(dst_id);
	      break;
	    }
	  }
	}
	else {
	  // 先読みの生成
	  for (ymuint i2 = 0; i2 < n2; ++ i2) {
	    const LR0Term& term2 = term_list2[i2];
	    if ( term2.rule() == rule1 && term2.dot_pos() == pos1 + 1 ) {
	      ymuint dst_id = calc_term_id(state2->id(), i2);
	      gen_list.push_back(make_pair(dst_id, token1));
	      break;
	    }
	  }
	}
      }
    }
  }

  // S' -> . S, $ という先読みを追加する．
  ymuint start_id = calc_term_id(start_state()->id(), 0);
  const Token* end = grammer->token(Grammer::kEnd);
  gen_list.push_back(make_pair(start_id, end));

  // 処理するトークンがなくなるまで以下の処理を繰り返す．
  for (ymuint rpos = 0; rpos < gen_list.size(); ++ rpos) {
    ymuint term_id = gen_list[rpos].first;
    const Token* token = gen_list[rpos].second;
    for (vector<ymuint>::const_iterator p = prop_list[term_id].begin();
	 p != prop_list[term_id].end(); ++ p) {
      ymuint dst_id = *p;
      // gen_list に (dst_id, token) が含まれているか調べる．
      bool found = false;
      for (vector<pair<ymuint, const Token*> >::iterator q = gen_list.begin();
	   q != gen_list.end(); ++ q) {
	if ( q->first == dst_id && q->second == token ) {
	  found = true;
	  break;
	}
      }
      if ( found ) {
	continue;
      }
      gen_list.push_back(make_pair(dst_id, token));
    }
  }
  // gen_list の結果を記録する．
  for (vector<pair<ymuint, const Token*> >::iterator q = gen_list.begin();
       q != gen_list.end(); ++ q) {
    ymuint term_id = q->first;
    const Token* token = q->second;
    mTokenList[term_id].push_back(token);
  }
}

// @brief デストラクタ
LALR1Set::~LALR1Set()
{
}

// @brief 先読みトークンのリストを返す．
// @param[in] state_id 状態番号
// @param[in] local_term_id 状態中の項番号
const vector<const Token*>&
LALR1Set::token_list(ymuint state_id,
		     ymuint local_term_id) const
{
  ymuint term_id = calc_term_id(state_id, local_term_id);
  return mTokenList[term_id];
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
LALR1Set::print(ostream& s) const
{
  for (vector<LR0State*>::const_iterator p = state_list().begin();
       p != state_list().end(); ++ p) {
    LR0State* state = *p;
    s << "State#" << state->id() << ":" << endl;
    const vector<LR0Term>& term_list = state->term_list();
    ymuint n = term_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      const Rule* rule = term_list[i].rule();
      ymuint pos = term_list[i].dot_pos();
      const Token* left = rule->left();
      s << "  " << left->str() << " ->";
      ymuint nr = rule->right_size();
      for (ymuint j = 0; j < nr; ++ j) {
	if ( j == pos ) {
	  s << " .";
	}
	s << " " << rule->right(j)->str();
      }
      if ( pos == nr ) {
	s << " .";
      }
      s << " ,";
      const vector<const Token*>& tlist = token_list(state->id(), i);
      const char* slash = "";
      for (vector<const Token*>::const_iterator q = tlist.begin();
	   q != tlist.end(); ++ q) {
	s << slash << (*q)->str();
	slash = "/";
      }
      s << endl;
    }
  }
  s << endl;
}

// @brief 状態番号とローカルな項番号から項番号を得る．
// @param[in] state_id 状態番号
// @param[in] local_term_id 状態中の項番号
ymuint
LALR1Set::calc_term_id(ymuint state_id,
		       ymuint local_term_id) const
{
  ASSERT_COND( state_id < state_list().size() );
  LR0State* state = state_list()[state_id];
  ASSERT_COND( local_term_id < state->term_list().size() );

  ymuint term_id = mTermIdTop[state_id] + local_term_id;
  return term_id;
}

END_NAMESPACE_YM
