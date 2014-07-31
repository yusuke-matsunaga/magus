#ifndef VERILOG_VL_VLCONTROL_H
#define VERILOG_VL_VLCONTROL_H

/// @file YmVerilog/vl/VlControl.h
/// @brief VlControl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.31 Repeat control


#include "YmVerilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlControl VlControl.h "ym_YmVerilog/vl/VlControl.h"
/// @brief delay/event/repeat control statement を表すクラス
/// IEEE Std 1364-2001 26.6.29 Delay control
/// IEEE Std 1364-2001 26.6.30 Event control
/// IEEE Std 1364-2001 26.6.31 Repeat control
/// IEEE1364-2001 では procedural timing control
/// とこれを混同している．
/// 正確には delay control はステートメントではない．
///
/// コントロールには，
/// - delay control
/// - event control
/// - repeat control
/// の3種類がある．
/// このうち，delay control の場合には，delay() のみが意味のある値を返す．
/// event control の場合には event_num() と event() のみが意味のある値を
/// 返す．
/// repeat control の場合には，expr(), event_num(), event() のみが意味の
/// ある値を返す．
//////////////////////////////////////////////////////////////////////
class VlControl :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlControl() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式を返す．
  virtual
  const VlExpr*
  delay() const = 0;

  /// @brief 繰り返し式を返す．
  virtual
  const VlExpr*
  expr() const = 0;

  /// @brief イベント条件式の数を返す．
  virtual
  ymuint
  event_num() const = 0;

  /// @brief イベント条件式を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < event_num() )
  virtual
  const VlExpr*
  event(ymuint pos) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VL_VLCONTROL_H
