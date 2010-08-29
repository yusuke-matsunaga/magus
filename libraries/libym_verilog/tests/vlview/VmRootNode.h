#ifndef VMROOTNODE_H
#define VMROOTNODE_H

/// @file libym_verilog/tests/vlview/VlPtNodeImpl.h
/// @brief VlPtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_root.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include <ym_verilog/verilog.h>


BEGIN_NAMESPACE_YM_VERILOG

class VlMgr;

//////////////////////////////////////////////////////////////////////
/// @class VmRootNode VmRootNode.h
/// @brief 根のノード
//////////////////////////////////////////////////////////////////////
class VmRootNode :
  public VmNode
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  VmRootNode(const VlMgr& vl_mgr);

  /// @brief デストラクタ
  virtual
  ~VmRootNode();
  
  
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
/// @class VmUdpListNode VmRootNode.h
/// @brief UDP のリストを表すノード
//////////////////////////////////////////////////////////////////////
class VmUdpListNode :
  public VmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  VmUdpListNode(const VlMgr& vl_mgr);

  /// @brief デストラクタ
  virtual
  ~VmUdpListNode();


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

END_NAMESPACE_YM_VERILOG

#endif // VMROOTNODE_H
