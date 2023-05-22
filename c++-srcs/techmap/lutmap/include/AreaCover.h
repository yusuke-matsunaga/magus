#ifndef AREACOVER_H
#define AREACOVER_H

/// @file AreaCover.h
/// @brief AreCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DagCover.h"


BEGIN_NAMESPACE_LUTMAP

class Cut;

//////////////////////////////////////////////////////////////////////
/// @class AreaCover AreaCover.h "AreaCover.h"
/// @brief 面積モードの DAG covering のヒューリスティック
//////////////////////////////////////////////////////////////////////
class AreaCover :
  public DagCover
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  AreaCover(
    bool fanout_mode ///< [in] ファンアウトモード
  );

  /// @brief デストラクタ
  ~AreaCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  void
  record_cuts(
    const SbjGraph& sbjgraph,    ///< [in] サブジェクトグラフ
    const CutHolder& cut_holder, ///< [in] 各ノードのカットを保持するオブジェクト
    MapRecord& maprec            ///< [out] マッピング結果を記録するオブジェクト
  ) override;

  /// @brief best cut の記録を行う．
  void
  record_cuts(
    const SbjGraph& sbjgraph,                    ///< [in] サブジェクトグラフ
    const CutHolder& cut_holder,                 ///< [in] 各ノードのカットを保持するオブジェクト
    const vector<const SbjNode*>& boundary_list, ///< [in] 境界ノードのリスト
    MapRecord& maprec                            ///< [out] マッピング結果を記録するオブジェクト
  );

  /// @brief best cut の記録を行う．
  void
  record_cuts(
    const SbjGraph& sbjgraph,                    ///< [in] サブジェクトグラフ
    const CutHolder& cut_holder,                 ///< [in] 各ノードのカットを保持するオブジェクト
    const vector<const SbjNode*>& boundary_list, ///< [in] 境界ノードのリスト
    const vector<const SbjNode*>& dupnode_list,  ///< [out] 重複ノードのリスト
    MapRecord& maprec                            ///< [out] マッピング結果を記録するオブジェクト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(
    const SbjNode* node, ///< [in] ノード
    const Cut* cut,      ///< [in] カット
    double cur_weight    ///< [in] 現在の重み
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各ノードのベスト値を記録する配列．
  vector<double> mBestCost;

  // 境界ノードの印
  vector<std::uint8_t> mBoundaryMark;

  // 各入力から根の出力に抜ける経路上の重みを入れる配列
  vector<double> mWeight;

};

END_NAMESPACE_LUTMAP

#endif // AREACOVER_H
