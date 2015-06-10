#ifndef FRMGR_H
#define FRMGR_H

/// @file FrMgr.h
/// @brief FrMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

class FrNode;

//////////////////////////////////////////////////////////////////////
/// @class FrMgr FrMgr.h "FrMgr.h"
/// @brief フロンテイア法を実行するクラス
//////////////////////////////////////////////////////////////////////
class FrMgr
{
public:

  /// @brief コンストラクタ
  FrMgr();

  /// @brief デストラクタ
  virtual
  ~FrMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief フロンティア法を用いて経路を求める．
  /// @param[in] graph 問題のグラフ
  /// @param[in] sel_list 処理する端子のリスト
  FrNode*
  find_path(const NlGraph& graph,
	    const vector<bool>& sel_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝の順序付けを行う．
  /// @param[in] graph 問題のグラフ
  void
  ordering(const NlGraph& graph);

  /// @brief フロンティアの更新を行う．
  /// @param[inout] frontier_nodes フロンティアノードのリスト
  /// @param[in] edge 新しく加わる枝
  /// @param[in] del_list 削除されるノードのフロンティアリスト上の位置
  /// @param[out] pos1, pos2 枝の両端のノードの位置
  ///
  /// pos1, pos2 は該当のノードがない場合には -1 を返す．
  void
  calc_frontier(vector<const NlNode*>& frontier_nodes,
		const NlEdge* edge,
		ymuint max_node_id,
		vector<ymuint>& del_list,
		int& pos1,
		int& pos2);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 処理する順に枝を収めたリスト
  vector<const NlEdge*> mEdgeList;

  // 枝のIDをキーにして順序を記録する配列
  vector<ymuint> mEdgeOrder;

};

END_NAMESPACE_YM_NLINK

#endif // FRMGR_H
