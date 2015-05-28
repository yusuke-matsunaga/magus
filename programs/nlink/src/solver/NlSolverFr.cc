
/// @file NlSolverFr.cc
/// @brief NlSolverFr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolverFr.h"
#include "NlProblem.h"
#include "NlSolution.h"
#include "FrgGraph.h"
#include "FrNode.h"

#include "YmUtils/HashMap.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM

template<>
struct
HashFunc<vector<ymuint> >
{
  ymuint
  operator()(const vector<ymuint>& key) const
  {
    ymuint ans = 0U;
    for (ymuint i = 0; i < key.size(); ++ i) {
      ans += ans * 117 + ans;
    }
    return ans;
  }
};

END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolverFr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlSolverFr::NlSolverFr()
{
}

// @brief デストラクタ
NlSolverFr::~NlSolverFr()
{
}

bool
update_frontier(FrontierInfo& fr,
		FrgEdge* edge,
		bool selected,
		FrgGraph& graph)
{
  vector<bool> mark(graph.max_node_id(), false);
  vector<ymuint> del_list;
  del_list.reserve(fr.node_num());
  for (ymuint i = 0; i < fr.node_num(); ++ i) {
    ymuint id = fr.node_id(i);
    mark[id] = true;
    FrgNode* node = graph.node(id);
    const vector<FrgEdge*>& edge_list = node->edge_list();
    bool found = false;
    for (ymuint j = 0; j < edge_list.size(); ++ j) {
      FrgEdge* edge1 = edge_list[j];
      if ( edge1->id() > edge->id() ) {
	// まだ未処理の枝がある．
	found = true;
	break;
      }
    }
    if ( !found ) {
      // 全て処理した．
      del_list.push_back(id);
    }
  }
  fr.delete_nodes(del_list);

  FrgNode* node1 = edge->node1();
  FrgNode* node2 = edge->node2();
  if ( !mark[node1->id()] ) {
    ymuint deg = selected ? 1 : 0;
    int comp_id = node1->id();
    fr.add_node(node1, deg, comp_id);
  }

}

// @brief 問題を解く
// @param[in] problem 問題
// @param[in] verbose verbose フラグ
// @param[out] solution 解
void
NlSolverFr::solve(const NlProblem& problem,
		  bool verbose,
		  NlSolution& solution)
{
  FrgGraph graph(problem);

  solution.init(problem);

  ymuint ne = graph.edge_num();
  // 各レベルごとの解集合
  vector<vector<FrNode*> > node_list_array(ne + 1);

  FrNode* root_node = new FrNode(0);
  node_list_array[0].push_back(root_node);

  for (ymuint i = 0; i < ne; ++ i) {
    FrgEdge* edge = graph.edge(i);
    const vector<FrNode*>& node_list = node_list_array[i];
    vector<FrNode*>& new_node_list = node_list_array[i + 1];
    for (ymuint j = 0; j < node_list.size(); ++ j) {
      FrNode* node = node_list[j];

      // edge を選ばないノードを作る．
      FrNode* node0 = new FrNode(i + 1);
      FrontierInfo& fr0 = node0->frontier_info();
      // 親のフロンティアをコピーする．
      fr0 = node->frontier_info();

      // フロンティアの更新
      // edge が最後の未処理枝だったノードを削除し
      // edge が最初の処理枝だったノードを追加する．
      // その際に，削除されるノードが条件を満たさなければ false を返す．
      bool stat = update_frontier(fr0, edge, false, graph);
      if ( !stat ) {
	// node の 0 枝は NULL
      }
      else {
	// node の 0 枝に node0 をセットする．

	new_node_list.push_back(node0);
      }

      // edge を選ぶノードを作る．
      FrNode* node1 = new FrNode(i + 1);
      FrontierInfo& fr1 = node1->frontier_info();
      // 親のフロンティアをコピーする．
      fr1 = node->frontier_info();

      new_node_list.push_back(node1);
    }
  }
}

END_NAMESPACE_YM_NLINK
