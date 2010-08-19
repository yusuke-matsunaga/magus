#ifndef VMMODULEMODE_H
#define VMMODULEMODE_H

/// @file libym_verilog/tests/vlview/VlPtNode_module.h
/// @brief VlPtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_module.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include <ym_verilog/verilog.h>
#include <ym_verilog/vl/VlModule.h>
#include <ym_verilog/vl/VlPort.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmModuleNode VmModuleNode.h "VmModuleNode.h"
/// @brief Module を表すノード
//////////////////////////////////////////////////////////////////////
class VmModuleNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] module Module の構造を表すオブジェクト
  VmModuleNode(const VlModule* module);

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
/// @class VmModulePortListNode VmModuleNodeImpl.h
/// @brief Module Port のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmModulePortListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] module Module の構造を表すオブジェクト
  VmModulePortListNode(const VlModule* module);

  /// @brief デストラクタ
  virtual
  ~VmModulePortListNode();


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

END_NAMESPACE_YM_VERILOG

#endif // VMMODULEMODE_H
