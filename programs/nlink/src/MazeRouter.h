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
  MazeRouter(const NlProblem& problem);

  /// @brief デストラクタ
  ~MazeRouter();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベル付けを行う．
  /// @param[in] idx 線分番号
  /// @return 最短経路長を返す．
  ymuint
  labeling(ymuint idx);

  /// @brief 直前のラベル付けの結果を得る．
  /// @param[in] label ラベル値
  /// @param[out] cell_list ラベル値を持つセルのリスト
  void
  get_cell_list(ymuint label,
		vector<NlPoint>& cell_list) const;


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

  // 問題
  const NlProblem& mProblem;

  // 幅
  ymuint mWidth;

  // 高さ
  ymuint mHeight;

  // セルの配列
  vector<int> mCellArray;

  // キュー
  vector<ymuint> mQueue;

};

END_NAMESPACE_YM_NLINK

#endif // MAZEROUTER_H
