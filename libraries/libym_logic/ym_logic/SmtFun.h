#ifndef YM_LOGIC_SMTFUN_H
#define YM_LOGIC_SMTFUN_H


/// @file ym_logic/SmtFun.h
/// @brief SmtFun のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/smt_nsdef.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtFun SmtFun.h "ym_logic/SmtFun.h"
/// @brief 関数を表すクラス
///
/// 関数は以下のプロパティを持つ．
///  - 入力の型
///  - 出力の型
///  - 本体の式 (オプショナル)
///  - 属性
///  - パラメータ数
///
/// 関数の属性は以下のとおり
///  - right assoc: 右方結合性．マルチオペランドのときに右から対にする．
///      cf) (R a b c d e) は (R a (R b (R c (R d e)))) と等価
///  - left assoc: 左方結合性．マルチオペランドのときに左から対にする．
///      cf) (L a b c d e) は (L (L (L (L a b) c) d) e) と等価
///  - chainable: マルチオペランドのときに各オペランドの間に演算子を挿入する．
///      cf) (C a b c d e) は (and (C a b) (C b c) (C c d) (C d e)) と等価
///          ちなに and は left assoc
///  - pairwise: マルチオペランドのときに全ての対を考える．
///      cf) (P a b c d e) は (and (P a b) (P a c) (P a d) (P a e)
///                                (P b c) (P b d) (P b e)
///                                (P c d) (P c e)
///                                (P d e)
///           と等価
///
/// 入力及び出力の型がパラメータ型になっている場合がある．
/// param_num() は全パラメータ数を返す．
//////////////////////////////////////////////////////////////////////
class SmtFun
{
  friend class SmtFunMgr;

public:
  //////////////////////////////////////////////////////////////////////
  // 型定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 任意のオペランドをとる関数の属性
  enum tAttr {
    kNone,       // なし
    kRightAssoc, // 右方結合
    kLeftAssoc,  // 左方結合
    kChainable,  // chainable
    kPairwise    // pairwise
  };


protected:

  /// @brief デストラクタ
  virtual
  ~SmtFun() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_num() const = 0;

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtSort*
  input_sort(ymuint pos) const = 0;

  /// @brief 入力変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtVar*
  input_var(ymuint pos) const = 0;

  /// @brief 出力の型を返す．
  virtual
  const SmtSort*
  output_sort() const = 0;

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtTerm*
  body() const = 0;

  /// @brief 属性を返す．
  virtual
  tAttr
  attr() const = 0;

  /// @brief パラメータの数を返す．
  virtual
  ymuint
  param_num() const = 0;

};


/// @relates SmtFun
/// @brief 内容を表す文字列を返す．
extern
string
fun_str(const SmtFun* fun);

END_NAMESPACE_YM_SMT

#endif // YM_LOGIC_SMTFUN_H
