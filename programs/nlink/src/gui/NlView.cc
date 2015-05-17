
/// @file NlView.cc
/// @brief NlView の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlView.h"
#include "NlPoint.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include <QPainter>


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlView
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のウィジェット
// @param[in] flags ウィンドウフラグ
NlView::NlView(QWidget* parent,
	       Qt::WindowFlags flags) :
  QWidget(parent, flags)
{
  mGridSize = 50;

  // 適当なサイズを設定しておく．
  set_size(10, 10);
}

// @brief デストラクタ
NlView::~NlView()
{
}

// @brief 問題を表示する．
// @param[in] problem 問題
void
NlView::set_problem(const NlProblem& problem)
{
  set_size(problem.width(), problem.height());

  for (ymuint i = 0; i < problem.elem_num(); ++ i) {
    NlConnection con = problem.connection(i);
    mCellArray[point_to_index(con.start_point())] = - (i + 1);
    mCellArray[point_to_index(con.end_point())] = - (i + 1);
  }
}

// @brief 解を表示する．
// @param[in] solution 解
void
NlView::set_solution(const NlSolution& solution)
{
  set_size(solution.width(), solution.height());

  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      int val = solution.get(x, y);
      mCellArray[xy_to_index(x, y)] = val;
    }
  }
}

// @brief サイズヒントを返す．
QSize
NlView::sizeHint() const
{
  return QSize(mBanWidth, mBanHeight);
}

// @brief paint イベント
void
NlView::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  QColor frame_color(180, 150, 100);
  QColor ban_color(0x50, 0x50, 0x50);
  QColor line_color(0, 0, 0);
  QColor wire_color(0, 0, 200);

  // 盤
  painter.fillRect(0, 0, mBanWidth, mBanHeight, ban_color);

  // 枠線
  QPen pen1(line_color);
  pen1.setWidth(3);

  painter.save();
  painter.setPen(pen1);
  for (ymuint i = 0; i <= mHeight; ++ i) {
    int x0 = 0;
    int y0 = i * mGridSize;
    int x1 = x0 + mBanWidth;
    int y1 = y0;
    painter.drawLine(x0, y0, x1, y1);
  }
  for (ymuint i = 0; i <= mWidth; ++ i) {
    int x0 = i * mGridSize;
    int y0 = 0;
    int x1 = x0;
    int y1 = y0 + mBanHeight;
    painter.drawLine(x0, y0, x1, y1);
  }
  painter.restore();

  // 結線の描画
  QPen pen2(wire_color);
  pen2.setWidth(10);

  painter.save();
  painter.setPen(pen2);
  for (ymuint x = 0; x < mWidth; ++ x) {
    int x0 = x * mGridSize;
    int cx = x0 + mGridSize / 2;
    for (ymuint y = 0; y < mHeight; ++ y) {
      int y0 = y * mGridSize;
      int cy = y0 + mGridSize / 2;
      int val0 = mCellArray[xy_to_index(x, y)];
      if ( val0 < 0 ) {
	continue;
      }
      if ( x > 0 ) {
	int val1 = mCellArray[xy_to_index(x - 1, y)];
	if ( val1 < 0 ) {
	  val1 = - val1;
	}
	if ( val0 == val1 ) {
	  int x1 = cx - mGridSize / 2;
	  painter.drawLine(cx, cy, x1, cy);
	}
      }
      if ( x < mWidth - 1 ) {
	int val1 = mCellArray[xy_to_index(x + 1, y)];
	if ( val1 < 0 ) {
	  val1 = - val1;
	}
	if ( val0 == val1 ) {
	  int x1 = cx + mGridSize / 2;
	  painter.drawLine(cx, cy, x1, cy);
	}
      }
      if ( y > 0 ) {
	int val1 = mCellArray[xy_to_index(x, y - 1)];
	if ( val1 < 0 ) {
	  val1 = - val1;
	}
	if ( val0 == val1 ) {
	  int y1 = cy - mGridSize / 2;
	  painter.drawLine(cx, cy, cx, y1);
	}
      }
      if ( y < mHeight - 1 ) {
	int val1 = mCellArray[xy_to_index(x, y + 1)];
	if ( val1 < 0 ) {
	  val1 = - val1;
	}
	if ( val0 == val1 ) {
	  int y1 = cy + mGridSize / 2;
	  painter.drawLine(cx, cy, cx, y1);
	}
      }
    }
  }
  painter.restore();

  // 端点の番号
  QFont font1("Courier", 16, QFont::Bold);
  painter.save();
  painter.setFont(font1);
  for (ymuint x = 0; x < mWidth; ++ x) {
    int x0 = x * mGridSize;
    for (ymuint y = 0; y < mHeight; ++ y) {
      int y0 = y * mGridSize;
      int val = mCellArray[xy_to_index(x, y)];
      if ( val < 0 ) {
	char buff[3];
	sprintf(buff, "%2d", - val);
	painter.drawText(QRect(x0, y0, mGridSize, mGridSize),
			 Qt::AlignCenter, buff);
      }
    }
  }
  painter.restore();
}

// @brief 盤面のサイズを設定する．
void
NlView::set_size(ymuint width,
		 ymuint height)
{
  mWidth = width;
  mHeight = height;

  mBanWidth = width * mGridSize;
  mBanHeight = height * mGridSize;

  mCellArray.clear();
  mCellArray.resize(width * height);
  for (ymuint i = 0; i < mCellArray.size(); ++ i) {
    mCellArray[i] = 0;
  }
}

// @brief xy座標からインデックスを得る．
ymuint
NlView::xy_to_index(ymuint x,
		    ymuint y) const
{
  ASSERT_COND( x >= 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y >= 0 );
  ASSERT_COND( y < mHeight );

  return x * mHeight + y;
}

// @brief インデックスから xy 座標を得る．
void
NlView::index_to_xy(ymuint index,
		    ymuint& x,
		    ymuint& y) const
{
  ASSERT_COND( index >= 0 );
  ASSERT_COND( index < (mWidth * mHeight) );

  x = index / mHeight;
  y = index % mHeight;
}

// @brief NlPoint からインデックスを得る．
ymuint
NlView::point_to_index(const NlPoint& point) const
{
  return xy_to_index(point.x(), point.y());
}

// @brief インデックスから NlPoint を得る．
NlPoint
NlView::index_to_point(ymuint index) const
{
  ymuint x;
  ymuint y;
  index_to_xy(index, x, y);
  return NlPoint(x, y);
}

END_NAMESPACE_YM_NLINK
