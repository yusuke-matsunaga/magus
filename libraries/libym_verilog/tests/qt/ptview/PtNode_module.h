#ifndef PTNODE_MODULE_H
#define PTNODE_MODULE_H

/// @file PtNode_module.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "YmVerilog/verilog.h"
#include "YmVerilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ModuleNode PtNodeImpl.h
/// @brief Module を表すノード
//////////////////////////////////////////////////////////////////////
class ModuleNode :
  public PtNode
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
/// @class ModulePortListNode PtNodeImpl.h
/// @brief Module Port のリストを表すノード
//////////////////////////////////////////////////////////////////////
class ModulePortListNode :
  public PtNode
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

#endif // PTNODE_MODLE_H
