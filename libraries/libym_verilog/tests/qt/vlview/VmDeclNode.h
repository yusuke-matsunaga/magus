#ifndef VMDECLNODE_H
#define VMDECLNODE_H

/// @file VmDeclNode.h
/// @brief VmDeclNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "YmVerilog/verilog.h"
#include "YmVerilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmIODeclListNode VmDeclNode.h
/// @brief IO宣言要素のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmIODeclListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] io_array IO宣言の配列
  VmIODeclListNode(const vector<const VlIODecl*>& io_array);

  /// @brief デストラクタ
  virtual
  ~VmIODeclListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // IO宣言の配列
  vector<const VlIODecl*> mIOArray;

};


//////////////////////////////////////////////////////////////////////
/// @class VmIODeclNode VmDeclNode.h
/// @brief IO宣言要素を表すノード
//////////////////////////////////////////////////////////////////////
class VmIODeclNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] io IO宣言要素
  VmIODeclNode(const VlIODecl* io);

  /// @brief デストラクタ
  virtual
  ~VmIODeclNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // IO宣言要素
  const VlIODecl* mIODecl;

};


//////////////////////////////////////////////////////////////////////
/// @class VmDeclListNode VmDeclNode.h
/// @brief Decl宣言要素のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmDeclListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] decl_array 宣言要素の配列
  VmDeclListNode(const QString& label,
		 const vector<const VlDecl*>& decl_array);

  /// @brief デストラクタ
  virtual
  ~VmDeclListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラベル
  QString mLabel;

  // 宣言の配列
  vector<const VlDecl*> mDeclArray;

};


//////////////////////////////////////////////////////////////////////
/// @class VmDeclNode VmDeclNode.h
/// @brief 宣言要素を表すノード
//////////////////////////////////////////////////////////////////////
class VmDeclNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] declitem 宣言要素
  VmDeclNode(const QString& label,
	     const VlDecl* declitem);

  /// @brief デストラクタ
  virtual
  ~VmDeclNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラベル
  QString mLabel;

  // 宣言要素
  const VlDecl* mDecl;

};


//////////////////////////////////////////////////////////////////////
/// @class VmDeclArrayListNode VmDeclNode.h
/// @brief Decl宣言要素のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmDeclArrayListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] decl_array 宣言要素の配列
  VmDeclArrayListNode(const QString& label,
		      const vector<const VlDeclArray*>& decl_array);

  /// @brief デストラクタ
  virtual
  ~VmDeclArrayListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラベル
  QString mLabel;

  // 宣言の配列
  vector<const VlDeclArray*> mDeclArray;

};


//////////////////////////////////////////////////////////////////////
/// @class VmDeclArrayNode VmDeclNode.h
/// @brief 宣言要素を表すノード
//////////////////////////////////////////////////////////////////////
class VmDeclArrayNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] declitem 宣言要素
  VmDeclArrayNode(const QString& label,
		  const VlDeclArray* declitem);

  /// @brief デストラクタ
  virtual
  ~VmDeclArrayNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラベル
  QString mLabel;

  // 宣言要素
  const VlDeclArray* mDecl;

};


//////////////////////////////////////////////////////////////////////
/// @class VmRangeListNode VmDeclNode.h
/// @brief 配列の範囲のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmRangeListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] decl 親の宣言要素
  VmRangeListNode(const VlDecl* decl);

  /// @brief デストラクタ
  ~VmRangeListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の宣言要素
  const VlDecl* mDecl;

};


//////////////////////////////////////////////////////////////////////
/// @class VmRangeNode VmDeclNode.h
/// @brief 配列の範囲を表すノード
//////////////////////////////////////////////////////////////////////
class VmRangeNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] range 範囲
  VmRangeNode(const VlRange* range);

  /// @brief デストラクタ
  ~VmRangeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  const VlRange* mRange;

};

END_NAMESPACE_YM_VERILOG

#endif // VMDECLNODE_H
