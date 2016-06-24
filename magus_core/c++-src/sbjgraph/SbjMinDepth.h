#ifndef SBJMINDEPTH_H
#define SBJMINDEPTH_H

/// @file SbjMinDepth.h
/// @brief SbjMinDepth のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"
#include "SbjGraph.h"
#include "ym/SimpleAlloc.h"


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
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  SbjMinDepth(const SbjGraph& sbjgraph);

  /// @brief デストラクタ
  ~SbjMinDepth();


public:
  //////////////////////////////////////////////////////////////////////
  // minimum depth 関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 各ノードの minimum depth を求める．
  /// @param[in] k LUT の最大入力数
  /// @param[out] depth_array 各ノードの深さを収める配列
  /// @return 出力の最大深さを返す．
  int
  operator()(int k,
	     vector<int>& depth_array);


private:
  //////////////////////////////////////////////////////////////////////
  // mindepth 関係の関数
  //////////////////////////////////////////////////////////////////////

  // node を根とする深さ d - 1 の k-feasible cut が存在するかどうか調べる．
  bool
  find_k_cut(SmdNode* node,
	     int k,
	     int d);

  // node およびその TFI に rmark を付ける．
  // depth が d のノードに tmark を付ける．
  void
  mark_tfi(SmdNode* node,
	   int d);

  // node のファンアウトを探索する．
  bool
  dfs_fanout(SmdNode* node);

  bool
  dfs(SmdNode* cur_node,
      int dir);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のオブジェクト
  SimpleAlloc mAlloc;

  // 対象のサブジェクトグラフ
  const SbjGraph& mSbjGraph;

  // PPI ノードのリスト
  vector<SmdNode*> mInputList;

  // ソートされた論理ノードのリスト
  vector<SmdNode*> mLogicNodeList;

  // mark_tfi で用いるノードのリスト
  vector<SmdNode*> mTfiNodeList;

  // ノード数
  int mNodeNum;

  // ノードごとの作業領域
  SmdNode* mNodeArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_SBJ

#endif // SBJMINDEPTH_H
