#ifndef YM_DOTLIB_DOTLIBNODE_H
#define YM_DOTLIB_DOTLIBNODE_H

/// @file DotlibNode.h
/// @brief DotlibNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibNode DotlibNode.h "DotlibNode.h"
/// @brief パース木の構造を表すための基底クラス
//////////////////////////////////////////////////////////////////////
class DotlibNode
{
public:

  /// @brief 型の定義
  enum tType {
    /// @brief 整数型の定数
    kInt,
    /// @brief 浮動小数点型の定数
    kFloat,
    /// @brief 文字列型の定数
    kString,
    /// @brief 浮動小数点数ベクタ型の定数
    kVector,
    /// @brief + 演算子
    kPlus,
    /// @brief - 演算子
    kMinus,
    /// @brief * 演算子
    kMult,
    /// @brief / 演算子
    kDiv,
    /// @brief NOT 演算子
    kNot,
    /// @brief AND 演算子
    kAnd,
    /// @brief OR 演算子
    kOr,
    /// @brief XOR 演算子
    kXor,
    /// @brief リスト
    kList,
    /// @brief グループ
    kGroup
  };

protected:

  /// @brief デストラクタ
  virtual
  ~DotlibNode() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  tType
  type() const = 0;

  /// @brief 整数型(kInt)の時に true を返す．
  virtual
  bool
  is_int() const = 0;

  /// @brief 数値型(kInt か kFloat)の時に true を返す．
  virtual
  bool
  is_float() const = 0;

  /// @brief 文字列型(kString)の時に true を返す．
  virtual
  bool
  is_string() const = 0;

  /// @brief ベクタ型(kVector)の時に true を返す．
  virtual
  bool
  is_vector() const = 0;

  /// @brief 演算子型(kPlus, kMinsu, kMult, kDiv)の時に true を返す．
  virtual
  bool
  is_opr() const = 0;

  /// @brief リスト型(kList)の時に true を返す．
  virtual
  bool
  is_list() const = 0;

  /// @brief グループ型(kGroup)の時に true を返す．
  virtual
  bool
  is_group() const = 0;

  /// @brief ファイル上の位置を返す．
  virtual
  FileRegion
  loc() const = 0;

  /// @brief 整数値を返す．
  /// @note is_int() = true の時のみ意味を持つ．
  virtual
  int
  int_value() const = 0;

  /// @brief 数値を返す．
  /// @note is_float() = true の時のみ意味を持つ．
  virtual
  double
  float_value() const = 0;

  /// @brief 文字列シンボルを返す．
  /// @note is_string() = true の時のみ意味を持つ．
  virtual
  ShString
  string_value() const = 0;

  /// @brief ベクタの要素数を返す．
  /// @note is_vector() = true の時のみ意味を持つ．
  virtual
  ymuint
  vector_size() const = 0;

  /// @brief ベクタの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < vector_size() )
  /// @note is_vector() = true の時のみ意味を持つ．
  virtual
  double
  vector_elem(ymuint pos) const = 0;

  /// @brief ベクタの全体を取り出す．
  /// @param[out] vector 結果を格納する変数
  /// @note is_vector() = true の時のみ意味を持つ．
  virtual
  void
  get_vector(vector<double>& vector) const = 0;

  /// @brief 第一オペランドを返す．
  /// @note is_opr() = true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  opr1() const = 0;

  /// @brief 第二オペランドを返す．
  /// @note is_opr() = true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  opr2() const = 0;

  /// @brief リストの要素数を返す．
  /// @note is_list() = true の時のみ意味を持つ．
  virtual
  ymuint
  list_size() const = 0;

  /// @brief リストの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < list_size() )
  /// @note is_list() == true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  list_elem(ymuint pos) const = 0;

  /// @brief グループの値を得る．
  /// @note is_group() = true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  group_value() const = 0;

  /// @brief 先頭の属性を得る．
  /// @note is_group() = true の時のみ意味を持つ．
  /// @note 返り値のノードの型は kAttr
  virtual
  const DotlibAttr*
  attr_top() const = 0;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  virtual
  void
  dump(ostream& s,
       ymuint indent = 0) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // パーズ用の便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1つの文字列からなるリストの場合に文字列を返す．
  /// @note 仮定が外れたらアボートする．
  ShString
  get_string_from_value_list() const;

  /// @brief 2つの文字列からなるリストの内容を取り出す．
  /// @param[out] str1 1つ目の文字列を格納する変数
  /// @param[out] str2 2つ目の文字列を格納する変数
  /// @retval true 正しく読み込めた．
  /// @retval false エラーが起こった．
  /// @note エラーは MsgMgr に出力する．
  bool
  get_string_pair(ShString& str1,
		  ShString& str2) const;

  /// @brief float 値を取り出す．
  /// @note 型が違ったらエラーを MsgMgr に出力する．
  bool
  get_float(double& value) const;

  /// @brief 文字列を取り出す．
  /// @note 型が違ったらエラーを MsgMgr に出力する．
  bool
  get_string(ShString& value) const;

};

/// @brief ストリーム出力演算子
inline
ostream&
operator<<(ostream& s,
	   const DotlibNode* node)
{
  node->dump(s, 0);
  return s;
}

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBNODE_H
