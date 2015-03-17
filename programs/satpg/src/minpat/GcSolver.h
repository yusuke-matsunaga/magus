#ifndef GCSOLVER_H
#define GCSOLVER_H

/// @file GcSolver.h
/// @brief GcSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

class GcNode;

//////////////////////////////////////////////////////////////////////
/// @class GcSolver GcSolver.h "GcSolver.h"
/// @brief 彩色問題を解くためのクラス
//////////////////////////////////////////////////////////////////////
class GcSolver
{
public:

  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  GcSolver(ymuint node_num);

  /// @brief デストラクタ
  ~GcSolver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  GcNode*
  node(ymuint id);

  /// @brief 彩色する．
  /// @param[in] color_group 同じ色のノード番号のリストの配列
  /// @return 彩色数を返す．
  ymuint
  coloring(vector<vector<ymuint> >& color_group);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mCountArray をクリアする．
  void
  clear_count();

  /// @brief ノードがカバーしている故障に印をつける．
  void
  cover_fault(GcNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // ノードの実体の配列
  GcNode* mNodeArray;

  // 現在の最大色番号
  ymuint32 mMaxColor;

  // sat degree 計算用の配列
  vector<bool> mCountArray;

  // 最大故障番号 + 1
  ymuint mMaxFaultId;

  // 故障のカバー状況を記録する配列
  vector<bool> mFaultMark;

};

END_NAMESPACE_YM_SATPG

#endif // GCSOLVER_H
