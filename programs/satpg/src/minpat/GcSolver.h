#ifndef GCSOLVER_H
#define GCSOLVER_H

/// @file GcSolver.h
/// @brief GcSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "FaultInfo.h"
#include "NodeValList.h"


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
  GcSolver();

  /// @brief デストラクタ
  ~GcSolver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色する．
  /// @return 彩色数を返す．
  ymuint
  coloring(const vector<TpgFault*>& fault_list,
	   const vector<FaultInfo>& fault_info_array,
	   const vector<pair<ymuint, ymuint> >& edge_list,
	   ymuint max_node_id);

  /// @brief 故障リストを返す．
  /// @param[in] col 色番号(1が最初)
  const vector<TpgFault*>&
  fault_list(ymuint col) const;

  /// @brief 十分割当リストを返す．
  /// @param[in] col 色番号(1が最初)
  const NodeValList&
  suf_list(ymuint col) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 「色」に対応した情報を収める構造体
  struct ColInfo
  {
    // 色番号(1が最初)
    ymuint mColId;

    // 故障リスト
    vector<TpgFault*> mFaultList;

    // 十分割当リスト
    NodeValList mSufList;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 新しい色を割り当てる．
  void
  new_color(TpgFault* fault);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障リスト
  vector<TpgFault*> mFaultList;

  // 枝のリスト
  vector<pair<ymuint, ymuint> > mEdgeList;

  // TpgNode のノード番号の最大値
  ymuint mMaxId;

  // 色のリスト
  vector<ColInfo*> mColList;

};

END_NAMESPACE_YM_SATPG

#endif // GCSOLVER_H
