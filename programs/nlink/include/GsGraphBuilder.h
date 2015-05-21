#ifndef GSGRAPHBUILDER_H
#define GSGRAPHBUILDER_H

/// @file GsGraphBuilder.h
/// @brief GsGraphBuilder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class GsGraphBuilder GsGraphBuilder.h "GsGraphBuilder.h"
/// @brief GsGraph を作るためのビルダクラス
//////////////////////////////////////////////////////////////////////
class GsGraphBuilder
{
public:

  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  GsGraphBuilder(ymuint node_num);

  /// @brief デストラクタ
  ~GsGraphBuilder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 始点を指定する．
  /// @param[in] node_id 始点のノード番号
  void
  set_start_node(ymuint node_id);

  /// @brief 終点を指定する．
  /// @param[in] node_id 終点のノード番号
  void
  set_end_node(ymuint node_id);

  /// @brief 枝を追加する．
  /// @param[in] node1_id, node2_id 両端のノード番号
  /// @param[in] var 枝の変数番号
  void
  add_edge(ymuint node1_id,
	   ymuint node2_id,
	   VarId var);

  /// @brief ノード数を返す．
  ymuint
  node_num() const;

  /// @brief 始点のノード番号を返す．
  ymuint
  start_node() const;

  /// @brief 終点のノード番号を返す．
  ymuint
  end_node() const;

  /// @brief ノードの枝番号リストを返す．
  /// @param[in] pos ノードの番号 ( 0 <= pos < node_num() )
  const vector<ymuint>&
  edge_list(ymuint pos) const;

  /// @brief 枝数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝の一方のノード番号を返す．
  /// @param[in] pos 枝の番号 ( 0 <= pos < edge_num() )
  ymuint
  node1(ymuint pos) const;

  /// @brief 枝の他方のノード番号を返す．
  /// @param[in] pos 枝の番号 ( 0 <= pos < edge_num() )
  ymuint
  node2(ymuint pos) const;

  /// @brief 枝の変数番号を返す．
  VarId
  var(ymuint pos) const;


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

  // 始点のノード番号
  ymuint mStartId;

  // 終点のノード番号
  ymuint mEndId;

  // ノードごとの枝番号リスト
  vector<vector<ymuint> > mEdgeListArray;

  // 枝の一方のノード番号のリスト
  vector<ymuint> mNode1List;

  // 枝の他方のノード番号のリスト
  vector<ymuint> mNode2List;

  // 枝の変数番号のリスト
  vector<VarId> mVarIdList;

};

END_NAMESPACE_YM_NLINK

#endif // GSGRAPHBUILDER_H
