#ifndef VERILOG_VL_VLUSERSYSTF_H
#define VERILOG_VL_VLUSERSYSTF_H

/// @file YmVerilog/vl/VlUserSystf.h
/// @brief VlUserSystf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlUserSystf VlUserSystf.h "YmVerilog/vl/VlUserSystf.h"
/// @brief システムタスク/関数の定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class VlUserSystf :
  public VlObj
{
public:

  /// @brief デストラクタ
  virtual
  ~VlUserSystf() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  string
  name() const = 0;

  /// @brief system task の時 true を返す．
  virtual
  bool
  system_task() const = 0;

  /// @brief system function の時 true を返す．
  virtual
  bool
  system_function() const = 0;

  /// @brief system function の型を返す．
  virtual
  tVpiFuncType
  function_type() const = 0;

  /// @brief compile 時のコールバック関数
  virtual
  ymuint
  on_compile() = 0;

  /// @brief 実行時のコールバック関数
  virtual
  ymuint
  on_call() = 0;

  /// @brief SizedFunc の場合にサイズを返す．
  virtual
  ymuint
  size() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VL_VLUSERSYSTF_H
