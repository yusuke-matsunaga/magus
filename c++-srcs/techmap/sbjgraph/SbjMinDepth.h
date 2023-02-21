#ifndef SBJMINDEPTH_H
#define SBJMINDEPTH_H

/// @file SbjMinDepth.h
/// @brief SbjMinDepth のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"
#include "SbjGraph.h"


BEGIN_NAMESPACE_SBJ

class SmdNode;

//////////////////////////////////////////////////////////////////////
/// @class SbjMinDepth SbjMinDepth.h "SbjMinDepth.h"
/// @brief SbjGraph を k-LUT にマッピングしたときの最小段数を求めるクラス
//////////////////////////////////////////////////////////////////////
class SbjMinDepth
{
public:

  /// @brief コンストラクタ
  SbjMinDepth(
    const SbjGraph& sbjgraph ///< [in] 対象のサブジェクトグラフ
  );

  /// @brief デストラクタ
  ~SbjMinDepth();


public:
  //////////////////////////////////////////////////////////////////////
  // minimum depth 関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 各ノードの minimum depth を求める．
  /// @return 出力の最大深さを返す．
  SizeType
  operator()(
    SizeType k,                   ///< [in] LUT の最大入力数
    vector<SizeType>& depth_array ///< [out] 各ノードの深さを収める配列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // mindepth 関係の関数
  //////////////////////////////////////////////////////////////////////

  // node を根とする深さ d - 1 の k-feasible cut が存在するかどうか調べる．
  bool
  find_k_cut(
    SmdNode* node,
    SizeType k,
    SizeType d
  );

  // node およびその TFI に rmark を付ける．
  // depth が d のノードに tmark を付ける．
  void
  mark_tfi(
    SmdNode* node,
    SizeType d
  );

  // node のファンアウトを探索する．
  bool
  dfs_fanout(
    SmdNode* node
  );

  bool
  dfs(
    SmdNode* cur_node,
    int dir
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のサブジェクトグラフ
  const SbjGraph& mSbjGraph;

  // PPI ノードのリスト
  vector<SmdNode*> mInputList;

  // ソートされた論理ノードのリスト
  vector<SmdNode*> mLogicNodeList;

  // mark_tfi で用いるノードのリスト
  vector<SmdNode*> mTfiNodeList;

  // ノード数
  SizeType mNodeNum;

  // ノードごとの作業領域
  SmdNode* mNodeArray;

};

END_NAMESPACE_SBJ

#endif // SBJMINDEPTH_H
