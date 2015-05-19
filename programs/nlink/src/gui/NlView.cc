
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
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

  mFringeSize = 20;
  mGridSize = 50;
  mInnerMargin = 5;
  mWireWidth = 15;

  // 適当なサイズを設定しておく．
  set_size(10, 10);
}

// @brief コンストラクタ
// @param[in] problem 問題
// @param[in] parent 親のウィジェット
// @param[in] flags ウィンドウフラグ
NlView::NlView(const NlProblem& problem,
	       QWidget* parent,
	       Qt::WindowFlags flags)
{
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

  mFringeSize = 20;
  mGridSize = 50;
  mInnerMargin = 5;
  mWireWidth = 15;

  set_problem(problem);
}

// @brief コンストラクタ
// @param[in] solution 解
// @param[in] parent 親のウィジェット
// @param[in] flags ウィンドウフラグ
NlView::NlView(const NlSolution& solution,
	       QWidget* parent,
	       Qt::WindowFlags flags)
{
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

  mFringeSize = 20;
  mGridSize = 50;
  mInnerMargin = 5;
  mWireWidth = 15;

  set_solution(solution);
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

  painter.setWindow(0, 0, mBanWidth, mBanHeight);

  int w = width();
  int h = height();
  double w1_f = (mBanWidth * h) / static_cast<double>(mBanHeight);
  double h1_f = (mBanHeight * w) / static_cast<double>(mBanWidth);
  int w1 = static_cast<int>(w1_f);
  int h1 = static_cast<int>(h1_f);

  if ( w1 > w ) {
    ASSERT_COND( h1 <= h );
    painter.setViewport(0, (h - h1) / 2, w, h1);
  }
  else {
    painter.setViewport((w - w1) / 2, 0, w1, h);
  }

  QColor frame_color(180, 150, 100);
  QColor ban_color(0x50, 0x50, 0x50);
  QColor line_color(0, 0, 0);
  QColor wire_color(0, 0, 200);

  // 枠
  painter.fillRect(0, 0, mBanWidth, mBanHeight, frame_color);

  // 盤
  painter.fillRect(mFringeSize, mFringeSize,
		   mBanWidth - mFringeSize * 2, mBanHeight - mFringeSize * 2,
		   ban_color);

  // 区切り線
  QPen pen1(line_color);
  pen1.setWidth(3);

  painter.save();
  painter.setPen(pen1);
  for (ymuint i = 0; i <= mHeight; ++ i) {
    int x0 = mFringeSize;
    int y0 = i * mGridSize + mFringeSize;
    int x1 = x0 + mBanWidth - mFringeSize * 2;
    int y1 = y0;
    painter.drawLine(x0, y0, x1, y1);
  }
  for (ymuint i = 0; i <= mWidth; ++ i) {
    int x0 = i * mGridSize + mFringeSize;
    int y0 = mFringeSize;
    int x1 = x0;
    int y1 = y0 + mBanHeight - mFringeSize * 2;
    painter.drawLine(x0, y0, x1, y1);
  }
  painter.restore();

  // 結線の描画
  QPen pen2(wire_color);
  pen2.setWidth(mWireWidth);

  painter.save();
  painter.setPen(pen2);
  for (ymuint x = 0; x < mWidth - 1; ++ x) {
    int x0 = x * mGridSize + mFringeSize;
    for (ymuint y = 0; y < mHeight; ++ y) {
      int y0 = y * mGridSize + mFringeSize;

      int cx0 = x0 + mGridSize / 2;
      int cx1 = cx0 + mGridSize;
      int cy = y0 + mGridSize / 2;

      int val0 = mCellArray[xy_to_index(x, y)];
      int val1 = mCellArray[xy_to_index(x + 1, y)];

      if ( val0 < 0 ) {
	cx0 = x0 + mGridSize - mInnerMargin + mWireWidth / 2;
	val0 = -val0;
      }
      if ( val1 < 0 ) {
	cx1 = x0 + mGridSize + mInnerMargin - mWireWidth / 2;
	val1 = -val1;
      }
      if ( val0 == val1 ) {
	painter.drawLine(cx0, cy, cx1, cy);
      }
    }
  }
  for (ymuint y = 0; y < mHeight - 1; ++ y) {
    int y0 = y * mGridSize + mFringeSize;
    for (ymuint x = 0; x < mWidth; ++ x) {
      int x0 = x * mGridSize + mFringeSize;

      int cx = x0 + mGridSize / 2;
      int cy0 = y0 + mGridSize / 2;
      int cy1 = cy0 + mGridSize;

      int val0 = mCellArray[xy_to_index(x, y)];
      int val1 = mCellArray[xy_to_index(x, y + 1)];

      if ( val0 < 0 ) {
	cy0 = y0 + mGridSize - mInnerMargin + mWireWidth / 2;
	val0 = -val0;
      }
      if ( val1 < 0 ) {
	cy1 = y0 + mGridSize + mInnerMargin - mWireWidth / 2;
	val1 = -val1;
      }
      if ( val0 == val1 ) {
	painter.drawLine(cx, cy0, cx, cy1);
      }
    }
  }
  painter.restore();

  // 端点の番号
  QFont font1("Courier", 16, QFont::Bold);
  painter.save();
  painter.setPen(pen1);
  painter.setFont(font1);
  for (ymuint x = 0; x < mWidth; ++ x) {
    int x0 = x * mGridSize + mFringeSize;
    for (ymuint y = 0; y < mHeight; ++ y) {
      int y0 = y * mGridSize + mFringeSize;
      int val = mCellArray[xy_to_index(x, y)];
      if ( val >= 0 ) {
	continue;
      }

      painter.drawRect(x0 + mInnerMargin, y0 + mInnerMargin,
		       mGridSize - mInnerMargin * 2, mGridSize - mInnerMargin * 2);
      val = - val;
      char buff[3];
      if ( val < 10 ) {
	sprintf(buff, "%1d", val);
      }
      else {
	sprintf(buff, "%2d", val);
      }
      painter.drawText(QRect(x0, y0, mGridSize, mGridSize),
		       Qt::AlignCenter, buff);
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

  mBanWidth = width * mGridSize + mFringeSize * 2;
  mBanHeight = height * mGridSize + mFringeSize * 2;

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
