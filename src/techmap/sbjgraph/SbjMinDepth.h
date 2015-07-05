#ifndef LIBYM_TECHMAP_SBJMINDEPTH_H
#define LIBYM_TECHMAP_SBJMINDEPTH_H

/// @file SbjMinDepth.h
/// @brief SbjMinDepth のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SbjGraph.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"
#include "SbjGraph.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_SBJ

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
  ymuint
  operator()(ymuint k,
	     vector<ymuint>& depth_array);


private:
  //////////////////////////////////////////////////////////////////////
  // mindepth 関係の関数
  //////////////////////////////////////////////////////////////////////

  // node を根とする深さ d - 1 の k-feasible cut が存在するかどうか調べる．
  bool
  find_k_cut(SmdNode* node,
	     ymuint k,
	     ymuint d);

  // node およびその TFI に rmark を付ける．
  // depth が d のノードに tmark を付ける．
  void
  mark_tfi(SmdNode* node,
	   ymuint d);

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
  ymuint32 mNodeNum;

  // ノードごとの作業領域
  SmdNode* mNodeArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_SBJ

#endif // LIBYM_TECHMAP_SBJGRAPH_SBJMINDEPTH_H
