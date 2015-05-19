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
private:

  enum Dir {
    DirUp,
    DirDown,
    DirLeft,
    DirRight
  };

public:

  /// @brief コンストラクタ
  MazeRouter();

  /// @brief デストラクタ
  ~MazeRouter();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベル付を行う．
  /// @param[in] graph グラフ
  /// @param[in] idx 線分番号
  /// @param[in] dir 向き
  /// @param[out] edge_list ラベル付された枝番号のリスト
  /// @param[out] index_list 各ラベルごとの末尾のインデックスリスト
  /// @return 最短経路長を返す．
  ymuint
  labeling(const NlGraph& graph,
	   ymuint idx,
	   bool dir,
	   vector<ymuint>& edge_list,
	   vector<ymuint>& index_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベル付けの基本処理
  void
  label1(const NlGraph& graph,
	 const NlNode* node,
	 Dir dir,
	 ymuint label,
	 vector<const NlNode*>& cell_list,
	 vector<ymuint>& edge_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルの配列
  vector<int> mCellArray;

};

END_NAMESPACE_YM_NLINK

#endif // MAZEROUTER_H
