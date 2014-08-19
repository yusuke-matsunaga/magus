#ifndef DOTLIBNODEIMPL_H
#define DOTLIBNODEIMPL_H

/// @file DotlibNodeImpl.h
/// @brief DotlibNode の継承クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibNode.h"


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

  /// @brief ベクタ型(kVector)の時に true を返す．
  virtual
  bool
  is_vector() const;

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

  /// @brief ベクタの要素数を返す．
  /// @note is_vector() = true の時のみ意味を持つ．
  virtual
  ymuint
  vector_size() const;

  /// @brief ベクタの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < vector_size() )
  /// @note is_vector() = true の時のみ意味を持つ．
  virtual
  double
  vector_elem(ymuint pos) const;

  /// @brief ベクタの全体を取り出す．
  /// @param[out] vector 結果を格納する変数
  /// @note is_vector() = true の時のみ意味を持つ．
  virtual
  void
  get_vector(vector<double>& vector) const;

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

  /// @brief リストの要素数を返す．
  /// @note is_list() = true の時のみ意味を持つ．
  virtual
  ymuint
  list_size() const;

  /// @brief リストの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < list_size() )
  /// @note is_list() == true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  list_elem(ymuint pos) const;

  /// @brief グループの値を得る．
  /// @note is_group() = true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  group_value() const;

  /// @brief 先頭の属性を得る．
  /// @note is_group() = true の時のみ意味を持つ．
  virtual
  const DotlibAttr*
  attr_top() const;


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
  add_attr(DotlibAttr* attr);

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
/// @class DotlibVector DotlibNodeImpl.h "DotlibNodeImpl.h"
/// @brief ベクタを表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibVector :
  public DotlibNodeBase
{
  friend class DotlibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] value_list 値のリスト
  /// @param[in] loc ファイル上の位置
  DotlibVector(const vector<double>& value_list,
	       const FileRegion& loc);

  /// @brief デストラクタ
  ~DotlibVector();


public:

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief ベクタ型(kVector)の時に true を返す．
  virtual
  bool
  is_vector() const;

  /// @brief ベクタの要素数を返す．
  /// @note is_vector() = true の時のみ意味を持つ．
  virtual
  ymuint
  vector_size() const;

  /// @brief ベクタの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < vector_size() )
  /// @note is_vector() = true の時のみ意味を持つ．
  virtual
  double
  vector_elem(ymuint pos) const;

  /// @brief ベクタの全体を取り出す．
  /// @param[out] vector 結果を格納する変数
  /// @note is_vector() = true の時のみ意味を持つ．
  virtual
  void
  get_vector(vector<double>& vector) const;

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

  // 要素数
  ymuint32 mNum;

  // ベクタの本体
  double mBody[1];

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

  /// @brief リストの要素数を返す．
  /// @note is_list() = true の時のみ意味を持つ．
  virtual
  ymuint
  list_size() const;

  /// @brief リストの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < list_size() )
  /// @note is_list() == true の時のみ意味を持つ．
  virtual
  const DotlibNode*
  list_elem(ymuint pos) const;

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

  // 要素
  vector<const DotlibNodeImpl*> mBody;

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
  const DotlibAttr*
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
  add_attr(DotlibAttr* attr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  const DotlibNode* mValue;

  // 属性の先頭
  DotlibAttr* mAttrTop;

  // 属性の末尾
  DotlibAttr* mAttrTail;

};

END_NAMESPACE_YM_DOTLIB

#endif // DOTLIBNODEIMPL_H
