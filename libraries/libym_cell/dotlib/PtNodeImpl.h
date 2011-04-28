#ifndef LIBYM_CELL_DOTLIB_PTNODEIMPL_H
#define LIBYM_CELL_DOTLIB_PTNODEIMPL_H

/// @file libym_cell/dotlib/PtNodeImpl.h
/// @brief PtNode の継承クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtNodeBase PtNodeImple.h "PtNodeImpl.h"
/// @brief 直接値を表すクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class PtNodeBase :
  public PtNode
{
protected:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  PtNodeBase(const FileRegion& loc);

  /// @brief デストラクタ
  ~PtNodeBase();


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
/// @class PtInt PtNodeImple.h "PtNodeImpl.h"
/// @brief 整数値を表すクラス
//////////////////////////////////////////////////////////////////////
class PtInt :
  public PtNodeBase
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
/// @class PtFloat PtNodeImpl.h "PtNodeImpl.h"
/// @brief 実数値を表すクラス
//////////////////////////////////////////////////////////////////////
class PtFloat :
  public PtNodeBase
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
/// @class PtString PtNodeImpl.h "PtNodeImpl.h"
/// @brief 文字列を表すクラス
//////////////////////////////////////////////////////////////////////
class PtString :
  public PtNodeBase
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
/// @class PtOpr PtNodeImpl.h "PtNodeImpl.h"
/// @brief 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class PtOpr :
  public PtNode
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] type 演算子の型
  /// @param[in] opr1, opr2 オペランド
  PtOpr(tType type,
	PtNode* opr1,
	PtNode* opr2);

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
  const PtNode*
  opr1() const;

  /// @brief 第二オペランドを返す．
  /// @note type() が演算子の型の時のみ意味を持つ．
  virtual
  const PtNode*
  opr2() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  tType mType;

  // 第一オペランド
  const PtNode* mOpr1;

  // 第二オペランド
  const PtNode* mOpr2;

};


//////////////////////////////////////////////////////////////////////
/// @class PtList PtNodeImpl.h "PtNodeImpl.h"
/// @brief リストを表すクラス
//////////////////////////////////////////////////////////////////////
class PtList :
  public PtNode
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  PtList();

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
  const PtNode*
  top() const;


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  const PtNode* mTop;

  // 末尾の要素
  PtNode* mTail;

};


//////////////////////////////////////////////////////////////////////
/// @class PtGroup PtNodeImpl.h "PtNodeImpl.h"
/// @brief group statement を表す PtNode の継承クラス
//////////////////////////////////////////////////////////////////////
class PtGroup :
  public PtNodeBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtGroup(const PtNode* value,
	  const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~PtGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief 値を得る．
  /// @note type() が kGroup の時のみ意味を持つ．
  const PtNode*
  value() const;

  /// @brief 先頭の属性を得る．
  /// @note type() が kGroup の時のみ意味を持つ．
  const PtAttr*
  attr_top() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

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

  // 値
  const PtNode* mValue;

  // 属性の先頭
  PtAttr* mAttrTop;

  // 属性の末尾
  PtAttr* mAttrTail;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTNODE_H
