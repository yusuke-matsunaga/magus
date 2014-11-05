
/// @file Grammer.cc
/// @brief Grammer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Grammer.h"
#include "Rule.h"
#include "Token.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// @brief トークンのリストに追加する．
// @param[in] token_list 対象のトークンリスト
// @param[in] src_token 追加するトークン
// @retval true トークンが追加された．
// @retval false すでに含まれていた．
bool
add_to_tokenlist(vector<const Token*>& token_list,
		 const Token* token)
{
  for (vector<const Token*>::iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    const Token* token1 = *p;
    if ( token->id() == token1->id() ) {
      return false;
    }
    if ( token->id() < token1->id() ) {
      token_list.insert(p, token);
      return true;
    }
  }
  token_list.push_back(token);
  return true;
}

// @brief トークンのリストに追加する．
// @param[in] token_list 対象のトークンリスト
// @param[in] src_token_list 追加するトークン
// @retval true トークンが追加された．
// @retval false すでに含まれていた．
bool
add_to_tokenlist(vector<const Token*>& token_list,
		 const vector<const Token*>& src_token_list)
{
  vector<const Token*>::iterator p = token_list.begin();
  vector<const Token*>::iterator p_end = token_list.end();
  vector<const Token*>::const_iterator q = src_token_list.begin();
  vector<const Token*>::const_iterator q_end = src_token_list.end();
  bool update = false;
  while ( p != p_end && q != q_end ) {
    const Token* token1 = *p;
    const Token* src_token = *q;
    if ( src_token->id() == token1->id() ) {
      ++ p;
      ++ q;
    }
    else if ( src_token->id() < token1->id() ) {
      token_list.insert(p, src_token);
      ++ q;
      update = true;
    }
    else {
      ++ p;
    }
  }
  for ( ; q != q_end; ++ q) {
    token_list.push_back(*q);
    update = true;
  }
  return update;
}

struct TokenLt
{
  bool
  operator()(const Token* a,
	     const Token* b)
  {
    return a->id() < b->id();
  }
};

// @brief トークンのリストをソートする．
void
sort_tokenlist(vector<const Token*>& token_list)
{
  sort(token_list.begin(), token_list.end(), TokenLt());
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス Grammer
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Grammer::Grammer()
{
  mStart = add_token("_start_");
  ASSERT_COND( mStart->id() == kStart );

  mEnd = add_token("_end_");
  ASSERT_COND( mEnd->id() == kEnd );

  mEpsilon = add_token("_epsilon_");
  ASSERT_COND( mEpsilon->id() == kEpsilon );

  mNotExist = add_token("_not_exist_");
  ASSERT_COND( mNotExist->id() == kNotExist );

  mStartRule = NULL;
}

// @brief デストラクタ
Grammer::~Grammer()
{
  for (vector<Token*>::iterator p = mTokenList.begin();
       p != mTokenList.end(); ++ p) {
    delete *p;
  }
  mTokenList.clear();

  for (vector<Rule*>::iterator p = mRuleList.begin();
       p != mRuleList.end(); ++ p) {
    delete *p;
  }
  mRuleList.clear();
}

// @brief トークンを追加する．
// @param[in] str 記号を表す文字列
// @return トークンを返す．
Token*
Grammer::add_token(string str)
{
  ymuint id = mTokenList.size();
  Token* token = new Token(id, str);
  mTokenList.push_back(token);
  return token;
}

// @brief 文法規則を追加する．
// @param[in] left 左辺のトークン番号
// @param[in] right 右辺のトークンリスト
// @return 文法規則を返す．
Rule*
Grammer::add_rule(Token* left,
		  const vector<Token*>& right)
{
  ymuint id = mRuleList.size();
  Rule* rule = new Rule(id, left, right);
  mRuleList.push_back(rule);
  left->mRuleList.push_back(rule);
  return rule;
}

// @brief 開始記号を設定する．
// @param[in] start 開始記号
void
Grammer::set_start(Token* start)
{
  ASSERT_COND( mStartRule == NULL );
  mStartRule = add_rule(mStart, vector<Token*>(1, start));
}

// @brief 種々の解析を行う．
//
// 各トークンの FIRST/FOLLOW を計算しておく．
void
Grammer::analyze()
{
  // FIRST の計算

  // 終端節点は自分自身
  for (vector<Token*>::iterator p = mTokenList.begin();
       p != mTokenList.end(); ++ p) {
    Token* token = *p;
    if ( token->rule_list().empty() ) {
      token->mFirst.push_back(token);
    }
  }

  // 変更があるかぎり以下の処理を繰り返す．
  for (bool update = true; update; ) {
    update = false;
    for (vector<Rule*>::const_iterator p = mRuleList.begin();
	 p != mRuleList.end(); ++ p) {
      Rule* rule = *p;
      for (ymuint i = 0; i < rule->right_size(); ++ i) {
	const Token* token = rule->right(i);
	// const を外すための hack
	Token* left = mTokenList[rule->left()->id()];
	if ( add_to_tokenlist(left->mFirst, token) ) {
	  update = true;
	}
      }
    }
  }

  // mFirst リストをソートしておく．
  for (vector<Token*>::iterator p = mTokenList.begin();
       p != mTokenList.end(); ++ p) {
    Token* token = *p;
    sort_tokenlist(token->mFirst);
  }

  // FOLLOW の計算

  // 開始記号の FOLLOW は終了記号
  mStart->mFollow.push_back(mEnd);

  // 変更があるかぎり以下の処理を繰り返す．
  for (bool update = true; update; ) {
    update = false;
    for (vector<Rule*>::const_iterator p = mRuleList.begin();
	 p != mRuleList.end(); ++ p) {
      Rule* rule = *p;
      ymuint n = rule->right_size();
      for (ymuint i = 0; i < n - 1; ++ i) {
	bool has_epsilon = false;
	const Token* token = rule->right(i + 1);
	if ( token == mEpsilon ) {
	  has_epsilon = true;
	}
	else {
	  Token* right = mTokenList[rule->right(i)->id()];
	  if ( add_to_tokenlist(right->mFollow, token) ) {
	    update = true;
	  }
	}
	if ( has_epsilon ) {
	  Token* right = mTokenList[rule->right(i)->id()];
	  if ( add_to_tokenlist(right->mFollow, rule->left()->mFollow) ) {
	    update = true;
	  }
	}
      }
      Token* right = mTokenList[rule->right(n - 1)->id()];
      if ( add_to_tokenlist(right->mFollow, rule->left()->mFollow) ) {
	update = true;
      }
    }
  }

  // mFollow リストをソートしておく．
  for (vector<Token*>::iterator p = mTokenList.begin();
       p != mTokenList.end(); ++ p) {
    Token* token = *p;
    sort_tokenlist(token->mFollow);
  }

}

// @brief トークンを返す．
// @param[in] id トークンID
// @return トークンを返す．
Token*
Grammer::token(ymuint id) const
{
  ASSERT_COND( id < mTokenList.size() );
  return mTokenList[id];
}

// @brief 文法規則を返す．
// @param[in] id 文法規則ID
// @return 文法規則を返す．
Rule*
Grammer::rule(ymuint id) const
{
  ASSERT_COND( id < mRuleList.size() );
  return mRuleList[id];
}

// @brief トークンを表示する．
// @param[in] s 出力先のストリーム
void
Grammer::print_tokens(ostream& s) const
{
  s << "Tokens" << endl;
  for (vector<Token*>::const_iterator p = mTokenList.begin();
       p != mTokenList.end(); ++ p) {
    Token* token = *p;
    s << token->id() << ": " << token->str() << endl;
  }
  s << endl;
}

// @brief 文法規則を表示する．
// @param[in] s 出力先のストリーム
void
Grammer::print_rules(ostream& s) const
{
  s << "Rules" << endl;
  for (vector<Rule*>::const_iterator p = mRuleList.begin();
       p != mRuleList.end(); ++ p) {
    Rule* rule = *p;
    s << rule->id() << ": " << rule->left()->str() << " ::=";
    for (ymuint i = 0; i < rule->right_size(); ++ i) {
      const Token* token = rule->right(i);
      s << " " << token->str();
    }
    s << endl;
  }
  s << endl;
}

// @brief トークンのリストに対する FIRST を求める．
void
Grammer::first_of(const vector<const Token*>& token_list,
		  vector<const Token*>& first_list)
{
  first_list.clear();
  bool all_epsilon = true;
  for (vector<const Token*>::const_iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    bool has_epsilon = false;
    const Token* token = *p;
    const vector<const Token*>& flist = token->first();
    for (vector<const Token*>::const_iterator q = flist.begin();
	 q != flist.end(); ++ q) {
      const Token* token1 = *q;
      if ( token1 == mEpsilon ) {
	has_epsilon = true;
      }
      else {
	add_to_tokenlist(first_list, token1);
      }
    }
    if ( !has_epsilon ) {
      all_epsilon = false;
      break;
    }
  }
  if ( all_epsilon ) {
    first_list.push_back(mEpsilon);
  }
}

#if 0
void
LR0_next_state(const vector<LR0Term*>& input,
	       Token* token,
	       vector<LR0Term*>& output)
{
  vector<LR0Term*> tmp_terms;
  for (vector<LR0Term*>::const_iterator p = input.begin();
       p != input.end(); ++ p) {
    LR0Term* term = *p;
    Rule* rule = term->rule();
    ymuint pos = term->pos();
    if ( pos < rule->right_size() && rule->right(pos) == token ) {
      tmp_terms.push_back( new LR0Term(rule, pos + 1) );
    }
  }
  LR0_closure(tmp_terms, output);
}

// @brief LR(0)項のクロージャを求める．
// @param[in] input 入力の項集合
// @param[out] output 結果を納めるベクタ
void
Grammer::LR0_closure(const vector<LR0Term*>& input,
		     vector<LR0Term*>& output)
{
  vector<LR0Term*> new_list = input;
  output = input;
  while ( !new_list.empty() ) {
    vector<LR0Term*> cur_list = new_list;
    new_list.clear();
    for (vector<LR0Term*>::iterator p = cur_list.begin();
	 p != cur_list.end(); ++ p) {
      LR0Term* term = *p;
      Rule* rule = term->rule();
      ymuint pos = rule->pos();
      Token* left = rule->left();
      const vector<Token*>& right = rule->right();
      if ( pos < right.size() ) {
	Token* head = right[pos];
	const vector<Rule*>& rule_list = head->rule_list();
	for (vector<Rule*>::const_iterator p = rule_list.begin();
	     p != rule_list.end(); ++ p) {
	  Rule* rule1 = *p;
	  add_to_LR0term(new_list, rule1, 0);
	  add_to_LR0term(output, rule1, 0);
	}
      }
    }
  }
}
#endif

END_NAMESPACE_YM
