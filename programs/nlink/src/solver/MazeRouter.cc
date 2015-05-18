
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
MazeRouter::MazeRouter()
{
}

// @brief デストラクタ
MazeRouter::~MazeRouter()
{
}

// @brief 問題をセットする．
void
MazeRouter::set_problem(const NlProblem& problem)
{
  mWidth = problem.width();
  mHeight = problem.height();
  mNum = problem.elem_num();
  mCellArray.clear();
  mCellArray.resize((mWidth + 1) * (mHeight + 1) + 1);
  for (ymuint i = 0; i < mCellArray.size(); ++ i) {
    mCellArray[i] = -2; // 範囲外
  }
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      mCellArray[xy_to_index(x, y)] = 0;
    }
  }

  mConList.clear();
  mConList.reserve(mNum);
  for (ymuint i = 0; i < mNum; ++ i) {
    NlConnection con = problem.connection(i);
    mConList.push_back(con);
    mCellArray[point_to_index(con.start_point())] = -1;
    mCellArray[point_to_index(con.end_point())] = -1;
  }
}

// @brief 幅を得る．
ymuint
MazeRouter::width() const
{
  return mWidth;
}

// @brief 高さを得る．
ymuint
MazeRouter::height() const
{
  return mHeight;
}

// @brief 結線数を得る．
ymuint
MazeRouter::num() const
{
  return mNum;
}

// @brief 最短経路を求める．
// @param[in] idx 線分番号
// @param[out] point_list 経路
// @return 迂回長を返す．
ymuint
MazeRouter::find_route(ymuint idx,
		       vector<NlPoint>& point_list)
{
  ASSERT_COND( idx < mNum );

  NlConnection con = mProblem.connection(idx);
  NlPoint start_point = con.start_point();
  NlPoint end_point = con.end_point();

  ymuint dx = mHeight + 1;
  ymuint dy = 1;
  mQueue.clear();
  mQueue.reserve(mCellArray.size());
  ymuint index0 = point_to_index(start_point);
  mCellArray[index0] = 0;
  mQueue.push_back(index0);
  ymuint index1 = point_to_index(end_point);
  ymuint rpos = 0;
  bool reached = false;
  for (ymuint rpos = 0; rpos < mQueue.size(); ++ rpos) {
    ymuint index = mQueue[rpos];
    ymuint label = mCellArray[index];
    if ( index == index1 ) {
      reached = true;
      break;
    }
    label1(index - dy, label + 1);
    label1(index + dy, label + 1);
    label1(index - dx, label + 1);
    label1(index + dx, label + 1);
  }

  if ( !reached ) {
    return MAX_UINT;
  }


  return 0;
}

// @brief ラベル付けの基本処理
void
MazeRouter::label1(ymuint index,
		   ymuint label)
{
  if ( mCellArray[index] == 0 ) {
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
