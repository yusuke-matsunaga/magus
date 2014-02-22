#ifndef ELBCONTROL_H
#define ELBCONTROL_H

/// @file ElbControl.h
/// @brief ElbControl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control


#include "verilog/vl/VlControl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbControl ElbControl.h "ElbControl.h"
/// @brief delay/event/repeat control statement を表すクラス
/// IEEE Std 1364-2001 26.6.29 Delay control
/// IEEE Std 1364-2001 26.6.30 Event control
/// IEEE Std 1364-2001 26.6.31 Repeat control
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には delay control はステートメントではない．
//////////////////////////////////////////////////////////////////////
class ElbControl :
  public VlControl
{
protected:

  /// @brief コンストラクタ
  ElbControl() { }

  /// @brief デストラクタ
  virtual
  ~ElbControl() { }

};

END_NAMESPACE_YM_VERILOG

#endif // ELBCONTROL_H
