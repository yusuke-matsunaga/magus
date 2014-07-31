#ifndef ELBDELAY_H
#define ELBDELAY_H

/// @file ElbDelay.h
/// @brief ElbDelay のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/vl/VlDelay.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbDelay ElbDelay.h "ym_YmVerilog/vl/VlDelay.h"
/// @brief エラボレーション中の expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class ElbDelay :
  public VlDelay
{
protected:

  /// @brief コンストラクタ
  ElbDelay() { }

  /// @brief デストラクタ
  virtual
  ~ElbDelay() { }

};

END_NAMESPACE_YM_VERILOG

#endif // ELBDELAY_H
