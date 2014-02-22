#ifndef PTNODE_ROOT_H
#define PTNODE_ROOT_H

/// @file PtNode_root.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "verilog/verilog.h"
#include "verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlMgr;

//////////////////////////////////////////////////////////////////////
/// @class RootNode PtNode_root.h
/// @brief 根のノード
//////////////////////////////////////////////////////////////////////
class RootNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] pt_mgr VlMgr
  RootNode(const VlMgr& pt_mgr);

  /// @brief デストラクタ
  virtual
  ~RootNode();


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

  // VlMgr
  const VlMgr& mVlMgr;

};


//////////////////////////////////////////////////////////////////////
/// @class UdpListNode PtNode_root.h
/// @brief UDP のリストを表すノード
//////////////////////////////////////////////////////////////////////
class UdpListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] pt_mgr VlMgr
  UdpListNode(const VlMgr& pt_mgr);

  /// @brief デストラクタ
  virtual
  ~UdpListNode();


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

  // VlMgr
  const VlMgr& mVlMgr;

};


//////////////////////////////////////////////////////////////////////
/// @class ModuleListNode PtNode_root.h
/// @brief Module のリストを表すノード
//////////////////////////////////////////////////////////////////////
class ModuleListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] pt_mgr VlMgr
  ModuleListNode(const VlMgr& pt_mgr);

  /// @brief デストラクタ
  virtual
  ~ModuleListNode();


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

  // VlMgr
  const VlMgr& mVlMgr;

};

END_NAMESPACE_YM_VERILOG

#endif // PTNODE_ROOT_H
