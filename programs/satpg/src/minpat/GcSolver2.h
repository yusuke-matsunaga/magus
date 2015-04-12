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
#include "FaultInfo.h"


BEGIN_NAMESPACE_YM_SATPG

class GcNode2;

//////////////////////////////////////////////////////////////////////
/// @class GcSolver2 GcSolver2.h "GcSolver2.h"
/// @brief 彩色問題を解くためのクラス
//////////////////////////////////////////////////////////////////////
class GcSolver2
{
public:

  /// @brief コンストラクタ
  GcSolver2();

  /// @brief デストラクタ
  ~GcSolver2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色する．
  /// @return 彩色数を返す．
  ymuint
  coloring(const vector<TpgFault*>& fault_list,
	   const vector<FaultInfo>& fault_info_array,
	   const vector<vector<ymuint> >& input_list_array,
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

    // 両立故障リスト
    vector<GcNode2*> mCompatList;

    // 十分割当リスト
    NodeValList mSufList;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 新しい色を割り当てる．
  ymuint
  new_color(TpgFault* fault);

  /// @brief 故障が両立するか調べる．
  bool
  check_compat(ColInfo* cip,
	       GcNode2* node,
	       const vector<FaultInfo>& fault_info_array,
	       const vector<vector<ymuint> >& input_list_array);

  /// @brief 両立チェックの本体
  bool
  _check_compat(GcNode2* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // TpgNode のノード番号の最大値
  ymuint mMaxId;

  // 色のリスト
  vector<ColInfo*> mColList;

};

END_NAMESPACE_YM_SATPG

#endif // GCSOLVER2_H
