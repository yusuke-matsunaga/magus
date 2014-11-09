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
/// @brief 結合性を表す列挙型
//////////////////////////////////////////////////////////////////////
enum AssocType {
  /// @brief 指定なし
  kNotDefined,
  /// @brief 左結合性
  kLeftAssoc,
  /// @brief 右結合性
  kRightAssoc,
  /// @brief 結合しない
  kNonAssoc
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

  /// @brief トークン数
  ymuint
  token_num() const;

  /// @brief トークンを返す．
  /// @param[in] id トークンID ( 0 <= id < token_num() )
  /// @return トークンを返す．
  Token*
  token(ymuint id) const;

  /// @brief 文法規則の数
  ymuint
  rule_num() const;

  /// @brief 文法規則を返す．
  /// @param[in] id 文法規則ID ( 0 <= id < rule_num() )
  /// @return 文法規則を返す．
  Rule*
  rule(ymuint id) const;

  /// @brief 開始規則を返す．
  Rule*
  start_rule() const;

  /// @brief 項番号を返す．
  /// @param[in] rule_id 文法規則ID
  /// @param[in] dot_pos dot の位置
  ymuint
  term_id(ymuint rule_id,
	  ymuint dot_pos) const;

  /// @brief 項の総数を返す．
  ymuint
  term_size() const;

  /// @brief トークンのリストに対する FIRST を求める．
  void
  first_of(const vector<const Token*>& token_list,
	   vector<const Token*>& first_list);

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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークンリスト
  // Token::mIdをキーにした配列
  vector<Token*> mTokenList;

  // 文法規則のリスト
  // Rule::mId をキーにした配列
  vector<Rule*> mRuleList;

  // 各文法規則の項番号の先頭を納める配列
  vector<ymuint> mTermIdList;

  // 現在の項番号
  ymuint mNextTermId;

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
