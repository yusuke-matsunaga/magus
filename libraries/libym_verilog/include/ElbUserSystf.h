#ifndef ELBUSERSYSTF_H
#define ELBUSERSYSTF_H

/// @file ElbUserSystf.h
/// @brief ElbUserSystf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/vl/VlUserSystf.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbUserSystf ElbUserSystf.h "ElbUserSystf.h"
/// @brief システムタスク/関数の定義を表すクラス
/// IEEE Std 1364-2001 26.6.14 UDP
//////////////////////////////////////////////////////////////////////
class ElbUserSystf :
  public VlUserSystf
{
public:

  /// @brief コンストラクタ
  ElbUserSystf() { }

  /// @brief デストラクタ
  virtual
  ~ElbUserSystf() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  string
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlUserSystf に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const char*
  _name() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBUSERSYSTF_H
