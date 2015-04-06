#ifndef GCSOLVER2_H
#define GCSOLVER2_H

/// @file GcSolver.h
/// @brief GcSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "NodeValList.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

class GcNode;

//////////////////////////////////////////////////////////////////////
/// @class GcSolver2 GcSolver2.h "GcSolver2.h"
/// @brief 彩色問題を解くためのクラス
//////////////////////////////////////////////////////////////////////
class GcSolver2
{
public:

  /// @brief コンストラクタ
  /// @param[in] fault_list 故障リスト
  /// @param[in] max_id ノード番号の最大値
  GcSolver2(const vector<TpgFault*>& fault_list,
	    ymuint max_id);

  /// @brief デストラクタ
  ~GcSolver2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝を追加する．
  /// @param[in] id1, id2 故障番号 ( 0 <= id1, id2 < node_num() )
  void
  connect(ymuint id1,
	  ymuint id2);

  /// @brief 彩色する．
  /// @return 彩色数を返す．
  ymuint
  coloring();

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

    // 故障ごとの十分割当リスト
    vector<NodeValList> mFaultSufList;

    // 十分割当リスト
    NodeValList mSufList;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色する．
  /// @return 彩色数を返す．
  ymuint
  coloring1(RandGen& rg,
	    const vector<TpgFault*>& fault_list,
	    vector<ColInfo*>& col_list);

  /// @brief 新しい色を割り当てる．
  void
  new_color(TpgFault* fault,
	    vector<ColInfo*>& col_list);


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

#endif // GCSOLVER2_H
