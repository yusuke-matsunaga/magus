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
/// @class PtValueBase PtValueImple.h "PtValueImpl.h"
/// @brief 直接値を表すクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class PtValueBase :
  public PtValue
{
protected:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  PtValueBase(const FileRegion& loc);

  /// @brief デストラクタ
  ~PtValueBase();


public:

  /// @brief ファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
/// @class PtInt PtValueImple.h "PtValueImpl.h"
/// @brief 整数値を表すクラス
//////////////////////////////////////////////////////////////////////
class PtInt :
  public PtValueBase
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
  tType
  type() const;

  /// @brief 整数値を返す．
  /// @note type() が kInt の時のみ意味を持つ．
  virtual
  int
  int_value() const;

  /// @brief 数値を返す．
  /// @note type() が kFloat の時のみ意味を持つ．
  /// @note 実は kInt でもOK
  virtual
  double
  float_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  int mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class PtFloat PtValueImpl.h "PtValueImpl.h"
/// @brief 実数値を表すクラス
//////////////////////////////////////////////////////////////////////
class PtFloat :
  public PtValueBase
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
  tType
  type() const;

  /// @brief 数値を返す．
  /// @note type() が kFloat の時のみ意味を持つ．
  virtual
  double
  float_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class PtString PtValueImpl.h "PtValueImpl.h"
/// @brief 文字列を表すクラス
//////////////////////////////////////////////////////////////////////
class PtString :
  public PtValueBase
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtString(ShString value,
	   const FileRegion& loc);

  /// @brief デストラクタ
  ~PtString();


public:

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief 定数シンボルを返す．
  /// @note type() が kString の時のみ意味を持つ．
  virtual
  ShString
  string_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ShString mValue;

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

  /// @brief コンストラクタ
  /// @param[in] type 演算子の型
  /// @param[in] opr1, opr2 オペランド
  PtOpr(tType type,
	PtValue* opr1,
	PtValue* opr2);

  /// @brief デストラクタ
  virtual
  ~PtOpr();


public:

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief ファイル上の位置を返す．
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
  tType mType;

  // 第一オペランド
  const PtValue* mOpr1;

  // 第二オペランド
  const PtValue* mOpr2;

};


//////////////////////////////////////////////////////////////////////
/// @class PtList PtValueImpl.h "PtValueImpl.h"
/// @brief リストを表すクラス
//////////////////////////////////////////////////////////////////////
class PtList :
  public PtValue
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] top 先頭要素
  PtList(PtValue* top);

  /// @brief デストラクタ
  virtual
  ~PtList();


public:

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief ファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

  /// @brief リストの先頭の要素を返す．
  /// @note type() が kList の時のみ意味をもつ．
  virtual
  const PtValue*
  top() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭要素
  const PtValue* mTop;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTVALUEIMPLE_H
