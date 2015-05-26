
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
      fr0 = node->frontier_info();

      new_node_list.push_back(node0);

      // edge を選ぶノードを作る．
      FrNode* node1 = new FrNode(i + 1);
      FrontierInfo& fr1 = node1->frontier_info();
      fr1 = node->frontier_info();

      new_node_list.push_back(node1);
    }
  }
}

END_NAMESPACE_YM_NLINK
