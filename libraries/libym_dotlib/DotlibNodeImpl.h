#ifndef LIBYM_DOTLIB_DOTLIBNODEIMPL_H
#define LIBYM_DOTLIB_DOTLIBNODEIMPL_H

/// @file libym_dotlib/DotlibNodeImpl.h
/// @brief DotlibNode の継承クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibNodeImpl DotlibNodeImple.h "DotlibNodeImpl.h"
/// @brief DotlibNode の実装クラス
//////////////////////////////////////////////////////////////////////
class DotlibNodeImpl :
  public DotlibNode
{
  friend class DotlibList;
  friend class DotlibGroup;

protected:

  /// @brief コンストラクタ
  DotlibNodeImpl();

  /// @brief デストラクタ
  virtual
  ~DotlibNodeImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 整数型(kInt)の時に true を返す．
  virtual
  bool
  is_int() const;

  /// @brief 数値型(kInt か kFloat)の時に true を返す．
  virtual
  bool
  is_float() const;

  /// @brief 文字列型(kString)の時に true を返す．
  virtual
  bool
  is_string() const;

  /// @brief 演算子型(kPlus, kMinsu, kMult, kDiv)の時に true を返す．
  virtual
  bool
  is_opr() const;

  /// @brief リスト型(kList)の時に true を返す．
  virtual
  bool
  is_list() const;

  /// @brief グループ型(kGroup)の時に true を返す．
  virtual
  bool
  is_group() const;

  /// @brief 属性型(kAttr)の時に true を返す．
  virtual
  bool
  is_attr() const;

  /// @brief 整数値を返す．
  /// @note is_int() = true の時のみ意味を持つ．
  virtual
  int
  int_value() const;

  /// @brief 数値を返す．
  /// @note is_float() = true の時のみ意味を持つ．
  virtual
  double
  float_value() const;

  /// @brief 文字列シンボルを返す．
  /// @note is_string() = true の時のみ意味を持つ．
  virtual
  ShString
  string_value() const;

  /// @brief 第一オペランドを返す．
  /// @note is_opr() = true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  opr1() const;

  /// @brief 第二オペランドを返す．
  /// @note is_opr() = true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  opr2() const;

  /// @brief リストの先頭の要素を返す．
  /// @note is_list() = true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  top() const;

  /// @brief リストの要素数を返す．
  /// @note is_list() = true の時のみ意味を持つ．
  virtual
  ymuint
  list_size() const;

  /// @brief グループの値を得る．
  /// @note is_group() = true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  group_value() const;

  /// @brief 先頭の属性を得る．
  /// @note is_group() = true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  attr_top() const;

  /// @brief 属性名を得る．
  /// @note is_attr() = true の時のみ意味を持つ．
  virtual
  ShString
  attr_name() const;

  /// @brief 属性の値を得る．
  /// @note is_attr() = true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  attr_value() const;

  /// @brief リストの次の要素を得る．
  /// @note これはすべての型で意味を持つ．
  virtual
  const DotlibNode*
  next() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を追加する．
  /// @param[in] node 追加する要素
  /// @note is_list() = true の時のみ意味を持つ．
  virtual
  void
  add_node(DotlibNodeImpl* node);

  /// @brief attribute を設定する．
  /// @param[in] attr 属性
  /// @note is_group() = true の時のみ意味を持つ．
  virtual
  void
  add_attr(DotlibNodeImpl* attr);

  /// @brief 次の要素を設定する．
  /// @param[in] next 次の要素
  void
  set_next(DotlibNodeImpl* next);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンクポインタ
  DotlibNodeImpl* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class DotlibNodeBase DotlibNodeImpl.h "DotlibNodeImpl.h"
/// @brief 直接値を表すクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class DotlibNodeBase :
  public DotlibNodeImpl
{
protected:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  DotlibNodeBase(const FileRegion& loc);

  /// @brief デストラクタ
  ~DotlibNodeBase();


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
/// @class DotlibInt DotlibNodeImpl.h "DotlibNodeImpl.h"
/// @brief 整数値を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibInt :
  public DotlibNodeBase
{
  friend class DotlibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  DotlibInt(int value,
	    const FileRegion& loc);

  /// @brief デストラクタ
  ~DotlibInt();


public:

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief 整数型(kInt)の時に true を返す．
  virtual
  bool
  is_int() const;

  /// @brief 数値型(kInt か kFloat)の時に true を返す．
  virtual
  bool
  is_float() const;

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

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  virtual
  void
  dump(ostream& s,
       ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  int mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class DotlibFloat DotlibNodeImpl.h "DotlibNodeImpl.h"
/// @brief 実数値を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibFloat :
  public DotlibNodeBase
{
  friend class DotlibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  DotlibFloat(double value,
	      const FileRegion& loc);

  /// @brief デストラクタ
  ~DotlibFloat();


public:

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief 数値型(kInt か kFloat)の時に true を返す．
  virtual
  bool
  is_float() const;

  /// @brief 数値を返す．
  /// @note type() が kFloat の時のみ意味を持つ．
  virtual
  double
  float_value() const;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  virtual
  void
  dump(ostream& s,
       ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class DotlibString DotlibNodeImpl.h "DotlibNodeImpl.h"
/// @brief 文字列を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibString :
  public DotlibNodeBase
{
  friend class DotlibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  DotlibString(ShString value,
	       const FileRegion& loc);

  /// @brief デストラクタ
  ~DotlibString();


public:

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief 文字列型(kString)の時に true を返す．
  virtual
  bool
  is_string() const;

  /// @brief 定数シンボルを返す．
  /// @note type() が kString の時のみ意味を持つ．
  virtual
  ShString
  string_value() const;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  virtual
  void
  dump(ostream& s,
       ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ShString mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class DotlibNot DotlibNodeImpl.h "DotlibNodeImpl.h"
/// @brief NOT 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibNot :
  public DotlibNodeBase
{
  friend class DotlibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] opr オペランド
  /// @param[in] loc ファイル上の位置
  DotlibNot(const DotlibNode* opr,
	    const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~DotlibNot();


public:

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief 演算子型(kPlus, kMinsu, kMult, kDiv)の時に true を返す．
  virtual
  bool
  is_opr() const;

  /// @brief 第一オペランドを返す．
  /// @note type() が演算子の型の時のみ意味を持つ．
  virtual
  const DotlibNode*
  opr1() const;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  virtual
  void
  dump(ostream& s,
       ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 第一オペランド
  const DotlibNode* mOpr1;

};


//////////////////////////////////////////////////////////////////////
/// @class DotlibOpr DotlibNodeImpl.h "DotlibNodeImpl.h"
/// @brief 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibOpr :
  public DotlibNodeImpl
{
  friend class DotlibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] type 演算子の型
  /// @param[in] opr1, opr2 オペランド
  DotlibOpr(tType type,
	    const DotlibNode* opr1,
	    const DotlibNode* opr2);

  /// @brief デストラクタ
  virtual
  ~DotlibOpr();


public:

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief 演算子型(kPlus, kMinsu, kMult, kDiv)の時に true を返す．
  virtual
  bool
  is_opr() const;

  /// @brief ファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

  /// @brief 第一オペランドを返す．
  /// @note type() が演算子の型の時のみ意味を持つ．
  virtual
  const DotlibNode*
  opr1() const;

  /// @brief 第二オペランドを返す．
  /// @note type() が演算子の型の時のみ意味を持つ．
  virtual
  const DotlibNode*
  opr2() const;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  virtual
  void
  dump(ostream& s,
       ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  tType mType;

  // 第一オペランド
  const DotlibNode* mOpr1;

  // 第二オペランド
  const DotlibNode* mOpr2;

};


//////////////////////////////////////////////////////////////////////
/// @class DotlibList DotlibNodeImpl.h "DotlibNodeImpl.h"
/// @brief リストを表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibList :
  public DotlibNodeImpl
{
  friend class DotlibMgrImpl;

private:

  /// @brief コンストラクタ
  DotlibList();

  /// @brief デストラクタ
  virtual
  ~DotlibList();


public:

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief リスト型(kList)の時に true を返す．
  virtual
  bool
  is_list() const;

  /// @brief ファイル上の位置を返す．
  virtual
  FileRegion
  loc() const;

  /// @brief リストの先頭の要素を返す．
  /// @note type() が kList の時のみ意味をもつ．
  virtual
  const DotlibNode*
  top() const;

  /// @brief リストの要素数を返す．
  /// @note is_list() = true の時のみ意味を持つ．
  virtual
  ymuint
  list_size() const;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  virtual
  void
  dump(ostream& s,
       ymuint indent = 0) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を追加する．
  /// @param[in] node 追加する要素
  /// @note type() が kList の時のみ意味を持つ．
  virtual
  void
  add_node(DotlibNodeImpl* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  const DotlibNodeImpl* mTop;

  // 末尾の要素
  DotlibNodeImpl* mTail;

};


//////////////////////////////////////////////////////////////////////
/// @class DotlibGroup DotlibNodeImpl.h "DotlibNodeImpl.h"
/// @brief group statement を表す DotlibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class DotlibGroup :
  public DotlibNodeBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  DotlibGroup(const DotlibNode* value,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~DotlibGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief グループ型(kGroup)の時に true を返す．
  virtual
  bool
  is_group() const;

  /// @brief グループの値を得る．
  /// @note type() が kGroup の時のみ意味を持つ．
  virtual
  const DotlibNode*
  group_value() const;

  /// @brief 先頭の属性を得る．
  /// @note type() が kGroup の時のみ意味を持つ．
  virtual
  const DotlibNode*
  attr_top() const;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  virtual
  void
  dump(ostream& s,
       ymuint indent = 0) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute を設定する．
  /// @param[in] attr 属性
  /// @note type() が kGroup の時のみ意味を持つ．
  virtual
  void
  add_attr(DotlibNodeImpl* attr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  const DotlibNode* mValue;

  // 属性の先頭
  DotlibNodeImpl* mAttrTop;

  // 属性の末尾
  DotlibNodeImpl* mAttrTail;

};


//////////////////////////////////////////////////////////////////////
/// @class DotlibAttr DotlibNodeImpl.h "DotlibNodeImpl.h"
/// @brief 属性を表す DotlibNode の継承クラス
//////////////////////////////////////////////////////////////////////
class DotlibAttr :
  public DotlibNodeBase
{
  friend class DotlibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  DotlibAttr(const ShString& attr_name,
	     const DotlibNode* value,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~DotlibAttr();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief 属性型(kAttr)の時に true を返す．
  virtual
  bool
  is_attr() const;

  /// @brief 属性名を得る．
  virtual
  ShString
  attr_name() const;

  /// @brief 属性の値を得る．
  virtual
  const DotlibNode*
  attr_value() const;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  virtual
  void
  dump(ostream& s,
       ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性名
  ShString mAttrName;

  // 値
  const DotlibNode* mValue;

};

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_DOTLIBNODEIMPL_H
