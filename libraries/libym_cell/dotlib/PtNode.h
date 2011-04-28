#ifndef LIBYM_CELL_DOTLIB_PTNODE_H
#define LIBYM_CELL_DOTLIB_PTNODE_H

/// @file libym_cell/dotlib/PtNode.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtNode PtNode.h "PtNode.h"
/// @brief パース木の構造を表すための基底クラス
//////////////////////////////////////////////////////////////////////
class PtNode
{
  friend class PtMgr;
  friend class PtList;

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

  /// @brief コンストラクタ
  PtNode();

  /// @brief デストラクタ
  virtual
  ~PtNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  tType
  type() const = 0;

  /// @brief ファイル上の位置を返す．
  virtual
  FileRegion
  loc() const = 0;

  /// @brief 整数値を返す．
  /// @note type() が kInt の時のみ意味を持つ．
  virtual
  int
  int_value() const;

  /// @brief 数値を返す．
  /// @note type() が kFloat の時のみ意味を持つ．
  virtual
  double
  float_value() const;

  /// @brief 文字列シンボルを返す．
  /// @note type() が kString の時のみ意味を持つ．
  virtual
  ShString
  string_value() const;

  /// @brief 第一オペランドを返す．
  /// @note type() が演算子の型(kPlus, kMinus, kMult, kDiv)の時のみ意味を持つ．
  virtual
  const PtNode*
  opr1() const;

  /// @brief 第二オペランドを返す．
  /// @note type() が演算子の型(kPlus, kMinus, kMult, kDiv)の時のみ意味を持つ．
  virtual
  const PtNode*
  opr2() const;

  /// @brief リストの先頭の要素を返す．
  /// @note type() が kList の時のみ意味を持つ．
  virtual
  const PtNode*
  top() const;

  /// @brief リストの要素数を返す．
  /// @note type() が kList の時のみ意味を持つ．
  virtual
  ymuint
  list_size() const;

  /// @brief リストの次の要素を得る．
  /// @note これはすべての型で意味を持つ．
  const PtNode*
  next() const;

  /// @brief 値を得る．
  /// @note type() が kGroup の時のみ意味を持つ．
  virtual
  const PtNode*
  value() const;

  /// @brief 先頭の属性を得る．
  /// @note type() が kGroup の時のみ意味を持つ．
  virtual
  const PtAttr*
  attr_top() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を追加する．
  /// @param[in] node 追加する要素
  /// @note type() が kList の時のみ意味を持つ．
  virtual
  void
  add_node(PtNode* node);

  /// @brief attribute を設定する．
  /// @param[in] attr 属性
  /// @note type() が kGroup の時のみ意味を持つ．
  virtual
  void
  add_attr(PtAttr* attr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンクポインタ
  PtNode* mNext;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTNODE_H
