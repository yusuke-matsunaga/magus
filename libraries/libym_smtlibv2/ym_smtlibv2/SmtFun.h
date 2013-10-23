#ifndef YM_SMTLIBV2_SMTFUN_H
#define YM_SMTLIBV2_SMTFUN_H

#error "obsolete"

/// @file ym_smtlibv2/SmtFun.h
/// @brief SmtFun のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtFun SmtFun.h "ym_smtlibv2/SmtFun.h"
/// @brief 関数を表すクラス
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
/// 関数はオーバーロードされないので，同名の関数は唯一しか存在しない．
/// なので fun->name()->id() で関数の一意性を表現できる．
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

  /// @brief 名前を返す．
  virtual
  const SmtId*
  name() const = 0;

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
  const SmtId*
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

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTFUN_H
