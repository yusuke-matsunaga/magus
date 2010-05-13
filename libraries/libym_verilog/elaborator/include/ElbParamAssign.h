#ifndef LIBYM_VERILOG_ELB_ELBPARAMASSIGN_H
#define LIBYM_VERILOG_ELB_ELBPARAMASSIGN_H

/// @file libym_verilog/elaborator/include/ElbParamAssign.h
/// @brief ElbParamAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbParamAssign.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vl/VlParamAssign.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbParamAssign ElbParamAssign.h "ElbParamAssign.h"
/// @brief parameter 代入文を表すクラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class ElbParamAssign :
  public VlParamAssign
{
protected:
  
  /// @brief コンストラクタ
  ElbParamAssign();
  
  /// @brief デストラクタ
  virtual
  ~ElbParamAssign();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbParamAssign* mNext;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbDefParam ElbParamAssign.h "ElbParamAssign.h"
/// @brief defparam 文を表すクラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class ElbDefParam :
  public VlDefParam
{
protected:
  
  /// @brief コンストラクタ
  ElbDefParam();
  
  /// @brief デストラクタ
  virtual
  ~ElbDefParam();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbDefParam* mNext;
  
};


END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBPARAMASSIGN_H
