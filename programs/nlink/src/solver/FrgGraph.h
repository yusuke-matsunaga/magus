#ifndef FRGGRAPH_H
#define FRGGRAPH_H

/// @file FrgGraph.h
/// @brief FrgGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

class FrgNode;
class FrgEdge;

//////////////////////////////////////////////////////////////////////
/// @class FrgGraph FrgGraph.h "FrgGraph.h"
/// @brief 問題のグラフ
//////////////////////////////////////////////////////////////////////
class FrgGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] problem 問題
  FrgGraph(const NlProblem& problem);

  /// @brief デストラクタ
  ~FrgGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 幅を返す．
  ymuint
  width() const;

  /// @brief 高さを返す．
  ymuint
  height() const;

  /// @brief ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノード番号からノードを返す．
  /// @param[in] id ノード番号
  FrgNode*
  node(ymuint id) const;

  /// @brief 座標からノードを返す．
  /// @param[in] x, y 座標
  FrgNode*
  node(ymuint x,
       ymuint y) const;

  /// @brief 枝数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝番号から枝を返す．
  FrgEdge*
  edge(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 幅
  ymuint mWidth;

  // 高さ
  ymuint mHeight;

  // ノードの配列
  vector<FrgNode*> mNodeList;

  // 枝の配列
  vector<FrgEdge*> mEdgeList;

};

END_NAMESPACE_YM_NLINK

#endif // FRGGRAPH_H
