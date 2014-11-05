
/// @file LR0State.cc
/// @brief LR0State の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LR0State.h"
#include "LR0Term.h"
#include "Token.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LR0State
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] terms 項集合
LR0State::LR0State(ymuint id,
		   const vector<LR0Term>& terms)
{
  mId = id;

  for (vector<LR0Term>::const_iterator p = terms.begin();
       p != terms.end(); ++ p) {
    const LR0Term& term = *p;
    mTermList.push_back(term);
    const Token* token = term.next_token();
    if ( token != NULL ) {
      // mToken リストを更新する．
      // ただしすでに含まれていたらなにもしない．
      bool found = false;
      for (vector<const Token*>::iterator q = mTokenList.begin();
	   q != mTokenList.end(); ++ q) {
	const Token* token1 = *q;
	if ( token1 == token ) {
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	mTokenList.push_back(token);
      }
    }
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
  mTokenList.push_back(token);
}

END_NAMESPACE_YM
