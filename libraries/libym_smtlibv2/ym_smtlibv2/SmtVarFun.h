#ifndef YM_SMTLIBV2_SMTVARFUN_H
#define YM_SMTLIBV2_SMTVARFUN_H

/// @file ym_smtlibv2/SmtVarFun.h
/// @brief SmtVarFun のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtVarFun SmtVarFun.h "ym_smtlibv2/SmtVarFun.h"
/// @brief 変数/関数を表すクラス
///
/// 変数は以下のプロパティをモツ．
///  - 名前 (SmtId を用いてユニークに管理される)
///  - 型 (未定義の場合もある)
///  - 種類 ( global | forall | exists )
///
/// 関数は以下のプロパティを持つ．
///  - 名前 (SmtId を用いてユニークに管理される)
///  - 入力の型
///  - 出力の型
///  - 本体の式 (オプショナル)
///  - 属性
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
/// 変数/関数はオーバーロードされないので，同名の関数は唯一しか存在しない．
/// なので fun->name()->id() で関数の一意性を表現できる．
/// ただし，let/forall/exists のスコープは別．
//////////////////////////////////////////////////////////////////////
class SmtVarFun
{
  friend class SmtNameMgr;

public:
  //////////////////////////////////////////////////////////////////////
  // 型定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 任意のオペランドをとる関数の属性
  enum tFunAttr {
    kNone,       // なし
    kRightAssoc, // 右方結合
    kLeftAssoc,  // 左方結合
    kChainable,  // chainable
    kPairwise    // pairwise
  };


protected:

  /// @brief デストラクタ
  virtual
  ~SmtVarFun() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const SmtId*
  name() const = 0;

  /// @brief 変数の場合に true を返す．
  virtual
  bool
  is_var() const = 0;

  /// @brief 変数の型を返す．
  /// @note 型がついていない場合は NULL を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  const SmtSort*
  var_sort() const = 0;

  /// @brief global 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_global() const = 0;

  /// @brief forall 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_forall() const = 0;

  /// @brief exists 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_exists() const = 0;

  /// @brief 関数の場合に true を返す．
  virtual
  bool
  is_fun() const = 0;

  /// @brief 入力数を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  ymuint
  input_num() const = 0;

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtSort*
  input_sort(ymuint pos) const = 0;

  /// @brief 入力変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note uninterpreted function の場合は NULL を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtVarFun*
  input_var(ymuint pos) const = 0;

  /// @brief 出力の型を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtSort*
  output_sort() const = 0;

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtTerm*
  fun_body() const = 0;

  /// @brief 関数の属性を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  tFunAttr
  fun_attr() const = 0;

  /// @brief パラメータの数を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  ymuint
  param_num() const = 0;

};


/// @relates SmtVarFun
/// @brief 内容を表す文字列を返す．
extern
string
varfun_str(const SmtVarFun* obj);

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTFUN_H
