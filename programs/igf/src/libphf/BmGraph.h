#ifndef BMGRAPH_H
#define BMGRAPH_H

/// @file BmGraph.h
/// @brief BmGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class BmNode;
class BmEdge;

//////////////////////////////////////////////////////////////////////
/// @class BmGraph BmGraph.h "BmGraph.h"
/// @brief Bipartite Matching 用のグラフを表すクラス
//////////////////////////////////////////////////////////////////////
class BmGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] v1_num 節点集合 V1 の要素数
  /// @param[in] v2_num 節点集合 V2 の要素数
  BmGraph(ymuint v1_num,
	  ymuint v2_num);

  /// @brief デストラクタ
  ~BmGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 節点集合 V1 の要素数を返す．
  ymuint
  v1_num();

  /// @brief 節点集合 V1 の要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < v1_num() )
  BmNode*
  v1_elem(ymuint pos);

  /// @brief 節点集合 V2 の要素数を返す．
  ymuint
  v2_num();

  /// @brief 節点集合 V2 の要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < v2_num() )
  BmNode*
  v2_elem(ymuint pos);

  /// @brief 枝数を返す．
  ymuint
  edge_num();

  /// @brief 枝を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
  BmEdge*
  edge(ymuint pos);

  /// @brief 枝を作る．
  /// @param[in] v1 V1 側の要素
  /// @param[in] v2 V2 側の要素
  /// @note 重複チェックは行なわない．
  BmEdge*
  new_edge(BmNode* v1,
	   BmNode* v2);

  /// @breif 最大マッチングを求める．
  void
  find_match(vector<BmEdge*>& edge_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // V1 の要素数
  ymuint32 mV1Num;

  // V1 の配列
  BmNode* mV1Array;

  // V2 の要素数
  ymuint32 mV2Num;

  // V2 の配列
  BmNode* mV2Array;

  // 枝の配列
  vector<BmEdge*> mEdgeList;

};

END_NAMESPACE_YM_IGF

#endif // BMGRAPH_H
