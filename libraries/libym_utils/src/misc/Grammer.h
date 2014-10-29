#ifndef GRAMMER_H
#define GRAMMER_H

/// @file Grammer.h
/// @brief Grammer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

class Token;
class Rule;

//////////////////////////////////////////////////////////////////////
/// @class Token Grammer.h "Grammer.h"
/// @brief トークンを表すクラス
//////////////////////////////////////////////////////////////////////
class Token
{
  friend class Grammer;

public:

  /// @brief コンストラクタ
  /// @param[in] id トークンID
  /// @param[in] str 文字列
  Token(ymuint id,
	string str);

  /// @brief デストラクタ
  ~Token();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 文字列を返す．
  string
  str() const;

  /// @brief 文法規則のリストを返す．
  const vector<Rule*>&
  rule_list() const;

  /// @brief FIRST を返す．
  const vector<Token*>&
  first() const;

  /// @brief FOLLOW を返す．
  const vector<Token*>&
  follow() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // 文字列
  string mStr;

  // このトークンを左辺に持つ文法規則のリスト
  vector<Rule*> mRuleList;

  // FIRST リスト
  vector<Token*> mFirst;

  // FOLLOW リスト
  vector<Token*> mFollow;

};


//////////////////////////////////////////////////////////////////////
/// @class Rule Grammer.h "Grammer.h"
/// @brief 文法規則を表すクラス
//////////////////////////////////////////////////////////////////////
class Rule
{
public:

  /// @brief コンストラクタ
  Rule(ymuint id,
       Token* left,
       const vector<Token*>& right);

  /// @brief デストラクタ
  ~Rule();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 左辺のトークンを返す．
  Token*
  left() const;

  /// @brief 右辺のトークンのリストを返す．
  const vector<Token*>&
  right() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // 左辺のトークン番号
  Token* mLeft;

  // 右辺のトークンリスト
  vector<Token*> mRight;

};


//////////////////////////////////////////////////////////////////////
/// @class Grammer Grammer.h "Grammer.h"
/// @brief 文脈自由文法を表すクラス
///
/// - トークン: 終端記号か非終端記号
/// - 文法規則: 左辺は非終端記号，右辺はトークンの列
/// - 文法: 文法規則の集合＋開始記号
///
/// 通常のトークン以外に特殊記号がある．
/// - ダミーの開始記号 kStart, S': S' -> S という規則も追加する．
/// - 文末を表す記号   kEnd, $
/// - 空記号           kEpsilon, ε
/// - 実際の文法規則
///   に現れない記号   kNotExist, #
//////////////////////////////////////////////////////////////////////
class Grammer
{
public:

  /// @brief ダミーの開始記号
  static
  const ymuint kStart = 0;

  /// @brief 文末を表す記号
  static
  const ymuint kEnd = 1;

  /// @brief 空記号
  static
  const ymuint kEpsilon = 2;

  /// @brief 実際の文法規則に使われない記号
  static
  const ymuint kNotExist = 3;


public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  ///
  /// 上の特殊記号はすでに定義済み
  Grammer();

  /// @brief デストラクタ
  ~Grammer();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンを追加する．
  /// @param[in] str 記号を表す文字列
  /// @return トークンを返す．
  Token*
  add_token(string str);

  /// @brief 文法規則を追加する．
  /// @param[in] left 左辺のトークン番号
  /// @param[in] right 右辺のトークンリスト
  /// @return 文法規則を返す．
  Rule*
  add_rule(Token* left,
	   const vector<Token*>& right);

  /// @brief 開始記号を設定する．
  /// @param[in] start 開始記号
  void
  set_start(Token* start);

  /// @brief 種々の解析を行う．
  ///
  /// 各トークンの FIRST/FOLLOW を計算しておく．
  void
  analyze();

  /// @brief トークンを返す．
  /// @param[in] id トークンID
  /// @return トークンを返す．
  Token*
  token(ymuint id) const;

  /// @brief 文法規則を返す．
  /// @param[in] id 文法規則ID
  /// @return 文法規則を返す．
  Rule*
  rule(ymuint id) const;

  /// @brief トークンを表示する．
  /// @param[in] s 出力先のストリーム
  void
  print_tokens(ostream& s) const;

  /// @brief 文法規則を表示する．
  /// @param[in] s 出力先のストリーム
  void
  print_rules(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンのリストに対する FIRST を求める．
  void
  first_of(const vector<Token*>& token_list,
	   vector<Token*>& first_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークンリスト
  // トークン番号をキーにした配列
  vector<Token*> mTokenList;

  // 文法規則のリスト
  // RUle::mId をキーにした配列
  vector<Rule*> mRuleList;

  // ダミーの開始記号
  Token* mStart;

  // 文末記号
  Token* mEnd;

  // 空記号
  Token* mEpsilon;

  // ダミーの終端記号
  Token* mNotExist;

  // ダミーの開始規則
  Rule* mStartRule;

};

END_NAMESPACE_YM

#endif // GRAMMER_H
