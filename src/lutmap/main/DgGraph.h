#ifndef DGGRAPH_H
#define DGGRAPH_H

/// @file DgGraph.h
/// @brief DgGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "ym/HashMap.h"

BEGIN_NAMESPACE_YM_LUTMAP

class DgNode;

//////////////////////////////////////////////////////////////////////
/// @class DgGraph DgGraph.h "DgGraph.h"
/// @brief dependence graph (DG)を表すクラス
//////////////////////////////////////////////////////////////////////
class DgGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  DgGraph(ymuint node_num);

  /// @brief デストラクタ
  ~DgGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
  DgNode*
  node(ymuint pos);

  /// @brief 枝を張nる．
  /// @param[in] id1, id2 両端のノード番号
  void
  connect(ymuint id1,
	  ymuint id2);

  /// @brief maximal independent set のサイズを求める．
  ymuint
  get_mis_size();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint mNodeNum;

  // ノードの(実体の)配列
  DgNode* mNodeArray;

  HashMap<ymuint, ymuint> mEdgeHash;

};

// @brief ノード数を得る．
inline
ymuint
DgGraph::node_num() const
{
  return mNodeNum;
}

END_NAMESPACE_YM_LUTMAP

#endif // DGGRAPH_H
