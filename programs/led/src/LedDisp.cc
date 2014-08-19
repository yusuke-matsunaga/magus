
/// @file LedDisp.cc
/// @brief LedDisp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "LedDisp.h"


BEGIN_NAMESPACE_LED

// @brief コンストラクタ
// @param[in] parent 親のウィジェット
// @param[in] flags ウィンドウフラグ
LedDisp::LedDisp(QWidget* parent,
		 Qt::WindowFlags flags) :
  QWidget(parent, flags)
{
}

// @brief デストラクタ
LedDisp::~LedDisp()
{
}

#if 0
// @brief 初期化
// @note on_realize() 中で呼ばれる．
// @note gl_begin() と gl_end() で挟まれている．
void
GlvScene::initialize()
{
}

// @brief ウィンドウのサイズが変わったときの処理
// @note on_configure_event() 中で呼ばれる．
// @note gl_begin() と gl_end() で挟まれている．
void
GlvScene::resize()
{
}

// @brief 描画を行う．
// @note on_expose_event() 中で呼ばれる．
// @note gl_begin() と gl_end() で挟まれている．
// @note ただし gl_end() の前に swap_buffers() か glFlush()
// を行う．
void
GlvScene::draw()
{
}

// "realize" イベントハンドラ
void
GlvScene::on_realize()
{
  // We need to call the base on_realize()
  Gtk::DrawingArea::on_realize();

  // Get GL::Drawable.
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  // GL calls.
  // *** OpenGL BEGIN ***
  if ( !gldrawable->gl_begin(get_gl_context()) ) {
    return;
  }

  initialize();

  gldrawable->gl_end();
  // *** OpenGL END ***
}

// "configure" イベントハンドラ
bool
GlvScene::on_configure_event(GdkEventConfigure* event)
{
  // Get GL::Drawable.
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  // GL calls.
  // *** OpenGL BEGIN ***
  if ( !gldrawable->gl_begin(get_gl_context()) ) {
    return false;
  }

  resize();

  gldrawable->gl_end();
  // *** OpenGL END ***

  return true;
}

// "expose" イベントハンドラ
bool
GlvScene::on_expose_event(GdkEventExpose* event)
{
  // Get GL::Drawable.
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  // GL calls.
  // *** OpenGL BEGIN ***
  if ( !gldrawable->gl_begin(get_gl_context()) ) {
    return false;
  }

  draw();

  // Swap buffers.
  if ( gldrawable->is_double_buffered() ) {
    gldrawable->swap_buffers();
  }
  else {
    glFlush();
  }

  gldrawable->gl_end();
  // *** OpenGL END ***

  return true;
}

/// @brief "map" イベントハンドラ
bool
GlvScene::on_map_event(GdkEventAny* event)
{
  return true;
}

/// @brief "unmap" イベントハンドラ
bool
GlvScene::on_unmap_event(GdkEventAny* event)
{
  return true;
}

/// @brief "visibility notify" イベントハンドラ
bool
GlvScene::on_visibility_notify_event(GdkEventVisibility* event)
{
  return true;
}
#endif


//////////////////////////////////////////////////////////////////////
// QWidget のイベントコールバック関数の再定義
//////////////////////////////////////////////////////////////////////

// paint イベント
void
LedDisp::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  QRect rect = event->rect();

#if 0
  // 盤面の背景
  mDispMgr->draw_ban(painter, rect);

  // 駒
  for (const Pos* ppos = Pos::all_list(); *ppos != kPos_NO; ++ ppos) {
    Pos pos(*ppos);
    ymuint8 pat_id = koma(pos).cur_pat();
    mDispMgr->draw_koma(painter, pos, pat_id, rect);
    if ( koma(pos).marker() ) {
      // マーカーの描画
      mDispMgr->draw_marker(painter, pos, rect);
    }
  }
  Pos cpos = cursor_pos();
  if ( check_cursor_disp() && cpos != kPos_NO ) {
    // カーソルの描画
    mDispMgr->draw_cursor(painter, cpos, rect);
  }
#endif
}

// Motion notify イベント
void
LedDisp::mouseMoveEvent(QMouseEvent* event)
{
  ymuint x = event->x();
  ymuint y = event->y();

#if 0
  Pos pos = search(x, y, mCursorPos);
  if ( pos != mCursorPos ) {
    // 位置が変わった．
    if ( !loose_cursor() || pos != kPos_NO ) {
      set_cursor_pos(pos);
    }
  }
#endif
}

// Leave notify イベント
void
LedDisp::leaveEvent(QEvent* event)
{
#if 0
  if ( !loose_cursor() ) {
    set_cursor_pos(kPos_NO);
  }
#endif
}

// Button Press イベント
void
LedDisp::mousePressEvent(QMouseEvent* event)
{
  ymuint x = event->x();
  ymuint y = event->y();
#if 0
  Pos pos = search(x, y, mCursorPos);
  if ( pos != kPos_NO ) {
    emit button_pressed(pos, event);
  }
#endif
}

// @brief Key Press イベント
void
LedDisp::keyPressEvent(QKeyEvent* event)
{
#if 0
  ymuint x;
  ymuint y;
  if ( mCursorPos.is_pass() ) {
    x = 1;
    y = 1;
  }
  else {
    x = mCursorPos.x();
    y = mCursorPos.y();
  }
  switch ( event->key() ) {
  case Qt::Key_Left:  if ( x > 1 ) -- x; break;
  case Qt::Key_Up:    if ( y > 1 ) -- y; break;
  case Qt::Key_Right: if ( x < 8 ) ++ x; break;
  case Qt::Key_Down:  if ( y < 8 ) ++ y; break;
  case Qt::Key_A:                  x = 1; break;
  case Qt::Key_B:                  x = 2; break;
  case Qt::Key_C:                  x = 3; break;
  case Qt::Key_D:                  x = 4; break;
  case Qt::Key_E:                  x = 5; break;
  case Qt::Key_F:                  x = 6; break;
  case Qt::Key_G:                  x = 7; break;
  case Qt::Key_H:                  x = 8; break;
  case Qt::Key_1:                  y = 1; break;
  case Qt::Key_2:                  y = 2; break;
  case Qt::Key_3:                  y = 3; break;
  case Qt::Key_4:                  y = 4; break;
  case Qt::Key_5:                  y = 5; break;
  case Qt::Key_6:                  y = 6; break;
  case Qt::Key_7:                  y = 7; break;
  case Qt::Key_8:                  y = 8; break;
  default: QWidget::keyPressEvent(event); return;
  }
  set_cursor_pos(Pos(x, y));
#endif
}

END_NAMESPACE_LED
