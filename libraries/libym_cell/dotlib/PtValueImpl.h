#ifndef LIBYM_CELL_DOTLIB_PTVALUEIMPLE_H
#define LIBYM_CELL_DOTLIB_PTVALUEIMPLE_H

/// @file libym_cell/dotlib/PtValueImpl.h
/// @brief PtValue の継承クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtValue.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtInt PtValueImple.h "PtValueImpl.h"
/// @brief 整数値を表すクラス
//////////////////////////////////////////////////////////////////////
class PtInt :
  public PtValue
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtInt(int value,
	const FileRegion& loc);

  /// @brief デストラクタ
  ~PtInt();


public:

  /// @brief 型を得る．
  virtual
  tTokenType
  type() const;

  /// @brief 式全体のファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  int mValue;

  // ファイル上の位置
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
/// @class PtFloat PtValueImpl.h "PtValueImpl.h"
/// @brief 実数値を表すクラス
//////////////////////////////////////////////////////////////////////
class PtFloat :
  public PtValue
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtFloat(double value,
	  const FileRegion& loc);

  /// @brief デストラクタ
  ~PtFloat();


public:

  /// @brief 型を得る．
  virtual
  tTokenType
  type() const;

  /// @brief 式全体のファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

  /// @brief 数値を返す．
  /// @note type() が FLOAT_NUM の時のみ意味を持つ．
  virtual
  double
  float_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

  // ファイル上の位置
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
/// @class PtString PtValueImpl.h "PtValueImpl.h"
/// @brief 文字列を表すクラス
//////////////////////////////////////////////////////////////////////
class PtString :
  public PtValue
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtString(const string& value,
	   const FileRegion& loc);

  /// @brief デストラクタ
  ~PtString();


public:

  /// @brief 型を得る．
  virtual
  tTokenType
  type() const;

  /// @brief 式全体のファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

  /// @brief 定数シンボルを返す．
  /// @note type() が SYMBOL の時のみ意味を持つ．
  virtual
  string
  string_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  string mValue;

  // ファイル上の位置
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
/// @class PtOpr PtValueImpl.h "PtValueImpl.h"
/// @brief 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class PtOpr :
  public PtValue
{
  friend class PtMgr;

private:

  /// @brief 表すコンストラクタ
  /// @param[in] type 演算子の型
  /// @param[in] opr1, opr2 オペランド
  PtOpr(tTokenType type,
	PtValue* opr1,
	PtValue* opr2);

  /// @brief デストラクタ
  virtual
  ~PtOpr();


public:

  /// @brief 型を得る．
  virtual
  tTokenType
  type() const;

  /// @brief 式全体のファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  tTokenType mType;

  // 第一オペランド
  const PtValue* mOpr1;

  // 第二オペランド
  const PtValue* mOpr2;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTVALUEIMPLE_H
