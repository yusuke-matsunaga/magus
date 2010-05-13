#ifndef YM_VERILOG_VL_VLSCOPE_H
#define YM_VERILOG_VL_VLSCOPE_H

/// @file ym_verilog/vl/VlScope.h
/// @brief VlScope のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlScope.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vl/VlNamedObj.h>
#include <ym_verilog/vl/VlFwd.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlScope VlScope.h <ym_verilog/vl/VlScope.h>
/// @brief elaboration 中の scope を表すクラス
/// IEEE Std 1364-2001 26.6.3 Scope
//////////////////////////////////////////////////////////////////////
class VlScope
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlScope() { }
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // VlScope の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 対象の named object (実際には下のいづれか) を返す．
  virtual
  const VlNamedObj*
  obj() = 0;
  
  /// @brief module scope の場合，対象の module を返す．
  /// @note それ以外のクラスでは NULL を返す．
  virtual
  const VlModule*
  module() = 0;

  /// @brief task scope の場合，対象の task を返す．
  virtual
  const VlTask*
  task() = 0;

  /// @brief function scope の場合，対象の function を返す．
  virtual
  const VlFunction*
  function() = 0;

  /// @brief named block scope の場合，対象の statement を返す．
  virtual
  const VlStmt*
  stmt() = 0;
  
#if 0
  /// @brief generate block scope の場合，対象の generate block を返す．
  virtual
  const VlGenBlock*
  genblock() = 0;
#endif
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLSCOPE_H
