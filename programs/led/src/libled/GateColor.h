#ifndef GATECOLOR_H
#define GATECOLOR_H

/// @file GateColor.h
/// @brief GateColor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "led_nsdef.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
/// @class GateColor GateColor.h "GateColor.h"
/// @brief ゲートの描画用の設定
//////////////////////////////////////////////////////////////////////
class GateColor
{
public:

  /// @brief コンストラクタ
  GateColor();

  /// @brief デストラクタ
  ~GateColor();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 輪郭用のペンを得る．
  const QPen&
  frame_pen() const;

  /// @brief 枠線用のペンを得る．
  const QPen&
  box_pen() const;

  /// @brief 塗りつぶし用のブラシを得る．
  const QBrush&
  gate_brush() const;

  /// @brief 輪郭用のペンを設定する．
  /// @param[in] pen 設定するペン
  void
  set_frame_pen(const QPen& pen);

  /// @brief 枠線用のペンを設定する．
  /// @param[in] pen 設定するペン
  void
  set_box_pen(const QPen& pen);

  /// @brief 塗りつぶし用のブラシを設定する．
  void
  set_gate_brush(const QBrush& brush);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 輪郭用のペン
  QPen mFramePen;

  // 枠線用のペン
  QPen mBoxPen;

  // 塗りつぶし用のブラシ
  QBrush mGateBrush;

};

END_NAMESPACE_YM_LED

#endif // GATECOLOR_H
