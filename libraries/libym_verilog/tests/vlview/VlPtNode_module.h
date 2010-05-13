#ifndef VLPTNODE_MODULE_H
#define VLPTNODE_MODULE_H

/// @file libym_verilog/tests/vlview/VlPtNode_module.h
/// @brief VlPtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_module.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VlPtNode.h"
#include <ym_verilog/verilog.h>
#include <ym_verilog/pt/PtP.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ModuleNode VlPtNodeImpl.h
/// @brief Module を表すノード
//////////////////////////////////////////////////////////////////////
class ModuleNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] module Module を表すパース木
  ModuleNode(const PtModule* module);

  /// @brief デストラクタ
  virtual
  ~ModuleNode();


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
  const PtModule* mModule;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ModulePortListNode VlPtNodeImpl.h
/// @brief Module Port のリストを表すノード
//////////////////////////////////////////////////////////////////////
class ModulePortListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] module Module を表すパース木
  ModulePortListNode(const PtModule* module);

  /// @brief デストラクタ
  virtual
  ~ModulePortListNode();


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
  const PtModule* mModule;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VLPARSETREEMODEL_H
