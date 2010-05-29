#ifndef LIBYM_VERILOG_ELB_ELBSCOPEHANDLE_H
#define LIBYM_VERILOG_ELB_ELBSCOPEHANDLE_H

/// @file libym_verilog/elb/ElbScopeHandle.h
/// @brief ElbScopeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbScopeHandle.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlScope.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbScopeHandle ElbScopeHandle.h "ElbScopeHandle.h"
/// @brief scope を保持するためのクラス
//////////////////////////////////////////////////////////////////////
class ElbScopeHandle :
  public VlScope
{
protected:
  
  /// @brief コンストラクタ
  ElbScopeHandle() { }
  
  /// @brief デストラクタ
  virtual
  ~ElbScopeHandle() { }


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  ElbScopeHandle* mNext;
  
};

END_NAMESPACE_YM_VERILOG

#endif // LiBYM_VERILOG_ELB_ELBSCOPEHANDLE_H
