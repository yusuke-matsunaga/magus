#ifndef YM_DOTLIB_PTNODE_H
#define YM_DOTLIB_PTNODE_H

/// @file ym_dotlib/PtNode.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/dotlib_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtNode PtNode.h "ym_dotlib/PtNode.h"
/// @brief パース木の構造を表すための基底クラス
//////////////////////////////////////////////////////////////////////
class PtNode
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
    /// @brief + 演算子
    kPlus,
    /// @brief - 演算子
    kMinus,
    /// @brief * 演算子
    kMult,
    /// @brief / 演算子
    kDiv,
    /// @brief リスト
    kList,
    /// @brief グループ
    kGroup
  };

protected:

  /// @brief デストラクタ
  virtual
  ~PtNode() { }


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

  /// @brief 第一オペランドを返す．
  /// @note is_opr() = true の時のみ意味を持つ．
  virtual
  const PtNode*
  opr1() const = 0;

  /// @brief 第二オペランドを返す．
  /// @note is_opr() = true の時のみ意味を持つ．
  virtual
  const PtNode*
  opr2() const = 0;

  /// @brief リストの先頭の要素を返す．
  /// @note is_list() = true の時のみ意味を持つ．
  virtual
  const PtNode*
  top() const = 0;

  /// @brief リストの要素数を返す．
  /// @note is_list() = true の時のみ意味を持つ．
  virtual
  ymuint
  list_size() const = 0;

  /// @brief リストの次の要素を得る．
  /// @note これはすべての型で意味を持つ．
  virtual
  const PtNode*
  next() const = 0;

  /// @brief 値を得る．
  /// @note is_group() = true の時のみ意味を持つ．
  virtual
  const PtNode*
  value() const = 0;

  /// @brief 先頭の属性を得る．
  /// @note is_group() = true の時のみ意味を持つ．
  virtual
  const PtAttr*
  attr_top() const = 0;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_PTNODE_H
