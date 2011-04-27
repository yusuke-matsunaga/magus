#ifndef LIBYM_CELL_DOTLIB_PTVALUE_H
#define LIBYM_CELL_DOTLIB_PTVALUE_H

/// @file libym_cell/dotlib/PtValue.h
/// @brief PtValue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtValue PtValue.h "PtValue.h"
/// @brief 値を表すためのクラス
///
/// 型としては以下の種類がある．
/// - 整数
/// - 実数
/// - 文字列
/// - 式(演算子)
/// - リスト
//////////////////////////////////////////////////////////////////////
class PtValue
{
  friend class PtMgr;
  friend class PtNode;
  friend class PtValueList;

public:

  /// @brief 型の定義
  enum tType {
    kInt,
    kFloat,
    kString,
    kPlus,
    kMinus,
    kMult,
    kDiv,
    kList
  };


protected:

  /// @brief コンストラクタ
  PtValue();

  /// @brief デストラクタ
  virtual
  ~PtValue();


public:

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
  const PtValue*
  opr1() const;

  /// @brief 第二オペランドを返す．
  /// @note type() が演算子の型(kPlus, kMinus, kMult, kDiv)の時のみ意味を持つ．
  virtual
  const PtValue*
  opr2() const;

  /// @brief リストの先頭の要素を返す．
  /// @note type() が kList の時のみ意味をもつ．
  virtual
  const PtValue*
  top() const;

  /// @brief リストの次の要素を得る．
  /// @note これはすべての型で意味を持つ．
  const PtValue*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  PtValue* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class PtValueList PtValue.h "PtValue.h"
/// @brief PtValue のリストを作るためのクラス
//////////////////////////////////////////////////////////////////////
class PtValueList
{
public:

  /// @brief コンストラクタ
  /// @note 空のリストとなる．
  PtValueList();

  /// @brief デストラクタ
  ~PtValueList();


public:

  /// @brief 要素を追加する．
  void
  add(PtValue* value);

  /// @brief 先頭の要素を取り出す．
  PtValue*
  top();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  PtValue* mTop;

  // 末尾の要素
  PtValue* mTail;

};


/// @relates PtValue
/// @brief PtValue の内容をストリームに出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] value 値
ostream&
operator<<(ostream& s,
	   const PtValue* value);

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTVALUE_H
