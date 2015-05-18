#ifndef MAZEROUTER_H
#define MAZEROUTER_H

/// @file MazeRouter.h
/// @brief MazeRouter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class MazeRouter MazeRouter.h "MazeRouter.h"
/// @brief number link 用の迷路法
//////////////////////////////////////////////////////////////////////
class MazeRouter
{
public:

  /// @brief コンストラクタ
  MazeRouter();

  /// @brief デストラクタ
  ~MazeRouter();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題をセットする．
  void
  set_problem(const NlProblem& problem);

  /// @brief 幅を得る．
  ymuint
  width() const;

  /// @brief 高さを得る．
  ymuint
  height() const;

  /// @brief 結線数を得る．
  ymuint
  num() const;

  /// @brief 最短経路を求める．
  /// @param[in] idx 線分番号
  /// @param[out] point_list 経路
  /// @return 迂回長を返す．
  ymuint
  find_route(ymuint idx,
	     vector<NlPoint>& point_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベル付けの基本処理
  void
  label1(ymuint index,
	 ymuint label);

  /// @brief xy 座標からインデックスを計算する．
  ymuint
  xy_to_index(ymuint x,
	      ymuint y) const;

  /// @brief インデックスから xy座標を計算する．
  void
  index_to_xy(ymuint index,
	      ymuint& x,
	      ymuint& y) const;

  /// @brief NlPoint からインデックスを計算する．
  ymuint
  point_to_index(const NlPoint& point) const;

  /// @brief インデックスから NlPoint を求める．
  NlPoint
  index_to_point(ymuint index) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 幅
  ymuint mWidth;

  // 高さ
  ymuint mHeight;

  // 結線数
  ymuint mNum;

  // 結線の配列
  vector<NlConnection> mConList;

  // セルの配列
  vector<int> mCellArray;

  // キュー
  vector<ymuint> mQueue;

};

END_NAMESPACE_YM_NLINK

#endif // MAZEROUTER_H
