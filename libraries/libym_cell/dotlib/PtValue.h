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
//////////////////////////////////////////////////////////////////////
class PtValue
{
  friend class PtMgr;

protected:

  /// @brief コンストラクタ
  PtValue();

  /// @brief デストラクタ
  virtual
  ~PtValue();


public:

  /// @brief 型を得る．
  virtual
  tTokenType
  type() const = 0;

  /// @brief 式全体のファイル上の位置を返す．
  virtual
  FileRegion
  loc() const = 0;

  /// @brief 整数値を返す．
  /// @note type() が INT_NUM の時のみ意味を持つ．
  virtual
  int
  int_value() const;

  /// @brief 数値を返す．
  /// @note type() が FLOAT_NUM の時のみ意味を持つ．
  virtual
  double
  float_value() const;

  /// @brief 文字列シンボルを返す．
  /// @note type() が SYMBOL の時のみ意味を持つ．
  virtual
  ShString
  string_value() const;

  /// @brief 第一オペランドを返す．
  /// @note type() が演算子の型の時のみ意味を持つ．
  virtual
  const PtValue*
  opr1() const;

  /// @brief 第二オペランドを返す．
  /// @note type() が演算子の型の時のみ意味を持つ．
  virtual
  const PtValue*
  opr2() const;

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
