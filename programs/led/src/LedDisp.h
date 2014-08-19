#ifndef LEDDISP_H
#define LEDDISP_H

/// @file LedDisp.h
/// @brief LedDisp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2013 Yusuke Matsunaga
/// All rights reserved.

#include "led_nsdef.h"
#include <QWidget.h>


namespace nsYm {
namespace nsLed {

//////////////////////////////////////////////////////////////////////
/// @class LedDisp LedDisp.h "LedDisp.h"
/// @brief LED の描画領域を表すクラス
//////////////////////////////////////////////////////////////////////
class LedDisp :
  public QWidget
{
  Q_OBJECT

public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のウィジェット
  /// @param[in] flags ウィンドウフラグ
  LedDisp(QWidget* parent = NULL,
	  Qt::WindowFlags flags = 0);

  /// @brief デストラクタ
  virtual
  ~LedDisp();


#if 0
public:

  /// @brief ウィンドウ全体を無効化する．
  void
  invalidate();

  /// @brief ウィンドウを更新する．
  void
  update();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
  /// @note on_realize() 中で呼ばれる．
  /// @note gl_begin() と gl_end() で挟まれている．
  virtual
  void
  initialize();

  /// @brief ウィンドウのサイズが変わったときの処理
  /// @note on_configure_event() 中で呼ばれる．
  /// @note gl_begin() と gl_end() で挟まれている．
  virtual
  void
  resize();

  /// @brief 描画を行う．
  /// @note on_expose_event() 中で呼ばれる．
  /// @note gl_begin() と gl_end() で挟まれている．
  /// @note ただし gl_end() の前に swap_buffers() か glFlush()
  /// を行う．
  virtual
  void
  draw();
#endif


protected:
  //////////////////////////////////////////////////////////////////////
  // QWidget のイベントコールバック関数の再定義
  //////////////////////////////////////////////////////////////////////

  /// @brief paint イベント
  virtual
  void
  paintEvent(QPaintEvent* event);

  /// @brief Motion notify イベント
  virtual
  void
  mouseMoveEvent(QMouseEvent* event);

  /// @brief Leave notify イベント
  virtual
  void
  leaveEvent(QEvent* event);

  /// @brief Button Press イベント
  virtual
  void
  mousePressEvent(QMouseEvent* event);

  /// @brief Key Press イベント
  virtual
  void
  keyPressEvent(QKeyEvent* event);

#if 0
protected:
  //////////////////////////////////////////////////////////////////////
  // シグナルハンドラ
  //////////////////////////////////////////////////////////////////////

  /// @brief "realize" イベントハンドラ
  virtual
  void
  on_realize();

  /// @brief "configure" イベントハンドラ
  virtual
  bool
  on_configure_event(GdkEventConfigure* event);

  /// @brief "expose" イベントハンドラ
  virtual
  bool
  on_expose_event(GdkEventExpose* event);

  /// @brief "map" イベントハンドラ
  virtual
  bool
  on_map_event(GdkEventAny* event);

  /// @brief "unmap" イベントハンドラ
  virtual
  bool
  on_unmap_event(GdkEventAny* event);

  /// @brief "visibility notify" イベントハンドラ
  virtual
  bool
  on_visibility_notify_event(GdkEventVisibility* event);
#endif

};


#if 0
//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ウィンドウ全体を無効化する．
inline
void
GlvScene::invalidate()
{
  get_window()->invalidate_rect(get_allocation(), false);
}

// @brief ウィンドウを更新する．
inline
void
GlvScene::update()
{
  get_window()->process_updates(false);
}
#endif

} // namespace nsLed
} // namespace nsYm

#endif // LEDDISP_H
