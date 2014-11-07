
/// @file LR0State.cc
/// @brief LR0State の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LR0State.h"
#include "LR0Term.h"
#include "Rule.h"
#include "Token.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LR0State
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] terms 項集合
// @param[in] signature シグネチャ
LR0State::LR0State(ymuint id,
		   const vector<LR0Term>& terms,
		   const vector<ymuint64>& signature) :
  mSignature(signature)
{
  mId = id;

  HashSet<ymuint> token_set;
  for (vector<LR0Term>::const_iterator p = terms.begin();
       p != terms.end(); ++ p) {
    const LR0Term& term = *p;
    mTermList.push_back(term);
    const Token* token = term.next_token();
    if ( token == NULL ) {
      continue;
    }

    // mToken リストを更新する．
    // ただしすでに含まれていたらなにもしない．
    if ( token_set.check(token->id()) ) {
      continue;
    }
    mTokenList.push_back(token);
    token_set.add(token->id());
  }
}

// @brief デストラクタ
LR0State::~LR0State()
{
}

// @brief 状態番号を返す．
ymuint
LR0State::id() const
{
  return mId;
}

// @brief LR(0)項集合を返す．
const vector<LR0Term>&
LR0State::term_list() const
{
  return mTermList;
}

// @brief シグネチャを返す．
const vector<ymuint64>&
LR0State::signature() const
{
  return mSignature;
}

// @brief トークンによる遷移先を返す．
// @param[in] token トークン
// @return 遷移先の状態を返す．
//
// 遷移が定義されていなかったらNULLを返す．
LR0State*
LR0State::next_state(const Token* token) const
{
  LR0State* state;
  if ( mNextStates.find(token->id(), state) ) {
    return state;
  }
  else {
    return NULL;
  }
}

// @brief 遷移を引き起こすトークンのリストを返す．
const vector<const Token*>&
LR0State::token_list() const
{
  return mTokenList;
}

// @brief 遷移を追加する．
// @param[in] token トークン
// @param[in] next_state 遷移先の状態
void
LR0State::add_next_state(const Token* token,
			 LR0State* next_state)
{
  ASSERT_COND( !mNextStates.check(token->id()) );
  mNextStates.add(token->id(), next_state);
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
LR0State::print(ostream& s) const
{
  s << "State#" << mId << ":" << endl;
  for (vector<LR0Term>::const_iterator p = mTermList.begin();
       p != mTermList.end(); ++ p) {
    const Rule* rule = p->rule();
    ymuint pos = p->dot_pos();
    const Token* left = rule->left();
    s << "  " << left->str() << " ->";
    ymuint n = rule->right_size();
    for (ymuint i = 0; i < n; ++ i) {
      if ( i == pos ) {
	s << " .";
      }
      s << " " << rule->right(i)->str();
    }
    if ( pos == n ) {
      s << " .";
    }
    s << endl;
  }
  s << endl;
}

END_NAMESPACE_YM
