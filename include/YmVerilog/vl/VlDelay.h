#ifndef VERILOG_VL_VLDELAY_H
#define VERILOG_VL_VLDELAY_H

/// @file YmVerilog/vl/VlDelay.h
/// @brief VlDelay のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlExpr;

//////////////////////////////////////////////////////////////////////
/// @class VlDelay VlDelay.h "ym_YmVerilog/vl/VlDelay.h"
/// @brief 遅延式を表す基底クラス
/// 基本的には VlExpr の配列だが要素数は3以下である．
//////////////////////////////////////////////////////////////////////
class VlDelay :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlDelay() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlDelay の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  virtual
  ymuint
  elem_num() const = 0;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  virtual
  const VlExpr*
  expr(ymuint pos) const = 0;

  /// @brief Verilog-HDL の文字列を得る．
  virtual
  string
  decompile() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VL_VLDELAY_H
