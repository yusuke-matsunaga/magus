#ifndef VMPORTNODE_H
#define VMPORTNODE_H

/// @file VmPortNode.h
/// @brief VmPortNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "YmVerilog/verilog.h"
#include "YmVerilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

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

#endif // VMPORTNODE_H
