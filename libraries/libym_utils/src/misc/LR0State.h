#ifndef LR0STATE_H
#define LR0STATE_H

/// @file LR0State.h
/// @brief LR0State のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmUtils/HashMap.h"


BEGIN_NAMESPACE_YM

class Token;
class LR0Term;

//////////////////////////////////////////////////////////////////////
/// @class LR0State LR0State.h "LR0State.h"
/// @brief LR(0)正準集の状態を表すクラス
//////////////////////////////////////////////////////////////////////
class LR0State
{
  friend class LR0Set;
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] terms 項集合
  LR0State(ymuint id,
	   const vector<LR0Term>& terms);

  /// @brief デストラクタ
  ~LR0State();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態番号を返す．
  ymuint
  id() const;

  /// @brief LR(0)項集合を返す．
  const vector<LR0Term>&
  term_list() const;

  /// @brief トークンによる遷移先を返す．
  /// @param[in] token トークン
  /// @return 遷移先の状態を返す．
  ///
  /// 遷移が定義されていなかったらNULLを返す．
  LR0State*
  next_state(const Token* token) const;

  /// @brief 遷移を引き起こすトークンのリストを返す．
  const vector<const Token*>&
  token_list() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遷移を追加する．
  /// @param[in] token トークン
  /// @param[in] next_state 遷移先の状態
  void
  add_next_state(const Token* token,
		 LR0State* next_state);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 状態番号
  ymuint mId;

  // LR(0)項の集合
  vector<LR0Term> mTermList;

  // トークン番号をキーにした連想配列
  // 遷移先の状態を保持する．
  HashMap<ymuint, LR0State*> mNextStates;

  // トークンのリスト
  // mNextStates に登録されている要素のキーのリストと一致する．
  vector<const Token*> mTokenList;

};

END_NAMESPACE_YM


#endif // LR0STATE_H
