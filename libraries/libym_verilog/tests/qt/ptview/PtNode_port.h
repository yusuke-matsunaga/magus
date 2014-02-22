#ifndef PTNODE_PORT_H
#define PTNODE_PORT_H

/// @file PtNode_port.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "verilog/verilog.h"
#include "verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PortNode PtNodeImpl.h
/// @brief ポートを表すノード
//////////////////////////////////////////////////////////////////////
class PortNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] port ポートを表すパース木
  PortNode(const PtPort* port);

  /// @brief デストラクタ
  virtual
  ~PortNode();


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
  const PtPort* mPort;

};

END_NAMESPACE_YM_VERILOG

#endif // PTNODE_PORT_H
