#ifndef EITOPLEVEL_H
#define EITOPLEVEL_H

/// @file EiToplevel.h
/// @brief ElbFrameToplevel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/vl/VlNamedObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiToplevel EiToplevel.h "EiToplevel.h"
/// @brief elaboration 中の toplevel scope を表すクラス
//////////////////////////////////////////////////////////////////////
class EiToplevel :
  public VlNamedObj
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  EiToplevel();

  /// @brief デストラクタ
  virtual
  ~EiToplevel();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief このオブジェクトの属している module を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlModule*
  parent_module() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;

};

END_NAMESPACE_YM_VERILOG

#endif // EITOPLEVEL_H
