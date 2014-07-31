#ifndef PTIEXPR_H
#define PTIEXPR_H

/// @file PtiExpr.h
/// @brief PtiExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtiExpr PtiExpr.h "PtiExpr.h"
/// @brief PtExpr の実装クラス
//////////////////////////////////////////////////////////////////////
class PtiExpr :
  public PtExpr
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式を表す文字列表現の取得
  virtual
  string
  decompile() const;

};

END_NAMESPACE_YM_VERILOG

#endif // PTIEXPR_H
