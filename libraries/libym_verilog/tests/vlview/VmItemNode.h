#ifndef VMITEMNODE_H
#define VMITEMNODE_H

/// @file libym_verilog/tests/vlview/VmItemNode.h
/// @brief VmItemNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmItemNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmPrimitiveListNode VmItemNode.h
/// @brief プリミティブのリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimitiveListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] prim_list プリミティブのリスト
  VmPrimitiveListNode(const vector<const VlPrimitive*>& prim_list);

  /// @brief デストラクタ
  virtual
  ~VmPrimitiveListNode();


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
  
  // プリミティブの配列
  vector<const VlPrimitive*> mPrimArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmPrimitiveNode VmItemNode.h
/// @brief プリミティブを表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimitiveNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] primitive プリミティブ
  VmPrimitiveNode(const VlPrimitive* primitive);

  /// @brief デストラクタ
  virtual
  ~VmPrimitiveNode();


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
  
  // プリミティブ
  const VlPrimitive* mPrimitive;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmPrimTermListNode VItemNode.h
/// @brief PrimTerm のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimTermListNode :
  public VmNode
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] primitive 親のプリミティブ
  VmPrimTermListNode(const VlPrimitive* primitive);

  /// @brief デストラクタ
  virtual
  ~VmPrimTermListNode();


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
  
  // 親のプリミティブ
  const VlPrimitive* mPrimitive;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmPrimTermNode VmItermNode
/// @brief PrimTerm を表すノード
//////////////////////////////////////////////////////////////////////
class VmPrimTermNode :
  public VmNode
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] prim_term プリミティブの端子
  VmPrimTermNode(const VlPrimTerm* prim_term);

  /// @brief デストラクタ
  virtual
  ~VmPrimTermNode();


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
  
  // プリミティブの端子
  const VlPrimTerm* mPrimTerm;

};


//////////////////////////////////////////////////////////////////////
/// @class VmContAssignListNode VmItemNode.h
/// @brief continuous assign 文のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmContAssignListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] ca_list 継続的代入文のリスト
  VmContAssignListNode(const vector<const VlContAssign*>& ca_list);

  /// @brief デストラクタ
  virtual
  ~VmContAssignListNode();


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
  
  // continuous assign 文の配列
  vector<const VlContAssign*> mContAssignArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmContAssignNode VmItemNode.h
/// @brief continuous assign 文を表すノード
//////////////////////////////////////////////////////////////////////
class VmContAssignNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] cont_assign 継続的代入文
  VmContAssignNode(const VlContAssign* cont_assign);

  /// @brief デストラクタ
  virtual
  ~VmContAssignNode();


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
  
  // continuous assign 文
  const VlContAssign* mContAssign;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VMITEMNODE_H
