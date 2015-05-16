
/// @file MazeRouter.cc
/// @brief MazeRouter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MazeRouter.h"
#include "NlProblem.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス MazeRouter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MazeRouter::MazeRouter(const NlProblem& problem) :
  mProblem(problem),
  mWidth(problem.width()),
  mHeight(problem.height()),
  mCellArray((mWidth + 1) * (mHeight + 1))
{
}

// @brief デストラクタ
MazeRouter::~MazeRouter()
{
}

// @brief ラベル付けを行う．
// @param[in] idx 線分番号
// @return 最短経路長を返す．
ymuint
MazeRouter::labeling(ymuint idx)
{
  ASSERT_COND( idx < mProblem.elem_num() );

  ymuint h = mProblem.height();
  NlConnection con = mProblem.connection(idx);
  NlPoint start_point = con.start_point();
  NlPoint end_point = con.end_point();

  for (ymuint i = 0; i < mCellArray.size(); ++ i) {
    mCellArray[i] = -2;
  }
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      ymuint index = xy_to_index(x, y);
      mCellArray[index] = -1;
    }
  }
  for (ymuint i = 0; i < mProblem.elem_num(); ++ i) {
    if ( i == idx ) {
      continue;
    }
    NlConnection con = mProblem.connection(i);
    NlPoint start_point = con.start_point();
    NlPoint end_point = con.end_point();
    mCellArray[point_to_index(start_point)] = -2;
    mCellArray[point_to_index(end_point)] = -2;
  }

  ymuint dx = mHeight + 1;
  ymuint dy = 1;
  mQueue.clear();
  mQueue.reserve(mCellArray.size());
  ymuint rpos = 0;
  ymuint index0 = point_to_index(start_point);
  label1(index0, 0);
  ymuint index1 = point_to_index(end_point);
  for (ymuint rpos = 0; rpos < mQueue.size(); ++ rpos) {
    ymuint index = mQueue[rpos];
    ymuint label = mCellArray[index];
    if ( index == index1 ) {
      return label;
    }
    label1(index - dy, label + 1);
    label1(index + dy, label + 1);
    label1(index - dx, label + 1);
    label1(index + dx, label + 1);
  }

  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 直前のラベル付けの結果を得る．
// @param[in] label ラベル値
// @param[out] cell_list ラベル値を持つセルのリスト
void
MazeRouter::get_cell_list(ymuint label,
			  vector<NlPoint>& cell_list) const
{
  cell_list.clear();
  for (ymuint i = 0; i < mCellArray.size(); ++ i) {
    if ( mCellArray[i] == label ) {
      cell_list.push_back(index_to_point(i));
    }
  }
}

// @brief ラベル付けの基本処理
void
MazeRouter::label1(ymuint index,
		   ymuint label)
{
  if ( mCellArray[index] == -1 ) {
    mCellArray[index] = label;
    mQueue.push_back(index);
  }
}

// @brief xy 座標からインデックスを計算する．
ymuint
MazeRouter::xy_to_index(ymuint x,
			ymuint y) const
{
  return (x + 1) * (mHeight + 1) + (y + 1);
}

// @brief インデックスから xy座標を計算する．
void
MazeRouter::index_to_xy(ymuint index,
			ymuint& x,
			ymuint& y) const
{
  x = index / (mHeight + 1) - 1;
  y = (index % (mHeight + 1)) - 1;
}

// @brief NlPoint からインデックスを計算する．
ymuint
MazeRouter::point_to_index(const NlPoint& point) const
{
  return xy_to_index(point.x(), point.y());
}

// @brief インデックスから NlPoint を求める．
NlPoint
MazeRouter::index_to_point(ymuint index) const
{
  ymuint x, y;
  index_to_xy(index, x, y);
  return NlPoint(x, y);
}

END_NAMESPACE_YM_NLINK
