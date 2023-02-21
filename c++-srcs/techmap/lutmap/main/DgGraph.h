#ifndef DGGRAPH_H
#define DGGRAPH_H

/// @file DgGraph.h
/// @brief DgGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"


BEGIN_NAMESPACE_LUTMAP

class DgNode;

//////////////////////////////////////////////////////////////////////
/// @class DgGraph DgGraph.h "DgGraph.h"
/// @brief dependence graph (DG)を表すクラス
//////////////////////////////////////////////////////////////////////
class DgGraph
{
public:

  /// @brief コンストラクタ
  DgGraph(
    SizeType node_num ///< [in] ノード数
  );

  /// @brief デストラクタ
  ~DgGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  SizeType
  node_num() const
  {
    return mNodeNum;
  }

  /// @brief ノードを得る．
  DgNode*
  node(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < node_num() )
  );

  /// @brief 枝を張る．
  void
  connect(
    SizeType id1, ///< [in] 片方のノード番号
    SizeType id2  ///< [in] 他方のノード番号
  );

  /// @brief maximal independent set のサイズを求める．
  SizeType
  get_mis_size();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  SizeType mNodeNum;

  // ノードの(実体の)配列
  // サイズは mNodeNum
  DgNode* mNodeArray;

  // 枝を表す辞書
  // キーは id1 * mNodeNum + id2，値は
  // node1->adj_list() 中の位置
  unordered_map<SizeType, SizeType> mEdgeHash;

};

END_NAMESPACE_LUTMAP

#endif // DGGRAPH_H
