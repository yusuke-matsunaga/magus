#ifndef VMMODULEMODE_H
#define VMMODULEMODE_H

/// @file libym_verilog/tests/vlview/VmModuleNode.h
/// @brief VmModuleNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_module.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmModuleListNode VmRootNode.h
/// @brief VmModule のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmModuleListNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  VmModuleListNode(const VlMgr& vl_mgr);

  /// @brief デストラクタ
  virtual
  ~VmModuleListNode();


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
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmModuleNode VmModuleNode.h "VmModuleNode.h"
/// @brief Module を表すノード
//////////////////////////////////////////////////////////////////////
class VmModuleNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  /// @param[in] module Module の構造を表すオブジェクト
  VmModuleNode(const VlMgr& vl_mgr,
	       const VlModule* module);

  /// @brief デストラクタ
  virtual
  ~VmModuleNode();


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

  // 対応する Module
  const VlModule* mModule;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VmPortListNode VmModuleNodeImpl.h
/// @brief Module Port のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmPortListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] module Module の構造を表すオブジェクト
  VmPortListNode(const VlModule* module);

  /// @brief デストラクタ
  virtual
  ~VmPortListNode();


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

  // 対応する Module
  const VlModule* mModule;
  
};

//////////////////////////////////////////////////////////////////////
/// @class VmPortNode VmPortNode.h
/// @brief ポートを表すノード
//////////////////////////////////////////////////////////////////////
class VmPortNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] port ポートを表すオブジェクト
  VmPortNode(const VlPort* port);

  /// @brief デストラクタ
  virtual
  ~VmPortNode();


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

  // 対応するポート
  const VlPort* mPort;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VMMODULEMODE_H
