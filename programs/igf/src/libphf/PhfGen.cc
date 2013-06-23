
/// @file PhfGen.cc
/// @brief PhfGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PhfGen.h"
#include "PhfGraph.h"
#include "PhfNode.h"
#include "PhfEdge.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス PhfGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PhfGen::PhfGen()
{
}

// @brief デストラクタ
PhfGen::~PhfGen()
{
}

// @brief マッピングを求める．
// @param[in] func_list 関数のリスト
// @param[out] g_list マッピングのリスト
bool
PhfGen::mapping(const vector<const FuncVect*>& func_list,
		vector<vector<ymuint32>* >& g_list)
{
  PhfGraph pg(func_list);

  if ( !pg.simple_check() ) {
    return false;
  }

  vector<PhfEdge*> edge_list;
  if ( !pg.acyclic_check(edge_list) ) {
    return false;
  }

  ymuint ne = edge_list.size();
  for (ymuint i = 0; i < ne; ++ i) {
    PhfEdge* edge = edge_list[i];
    ymuint nn = edge->node_num();
    for (ymuint j = 0; j < nn; ++ j) {
      PhfNode* node1 = edge->node(j);
      if ( !node1->is_assigned() ) {
	ymuint32 val = edge->val();
	for (ymuint k = 0; k < nn; ++ k) {
	  if ( k != j ) {
	    PhfNode* node2 = edge->node(k);
	    val ^= node2->val();
	  }
	}
	node1->set_val(val);
	vector<ymuint32>& g1 = *g_list[j];
	g1[node1->pat()] = val;
      }
    }
  }

  return true;
}

END_NAMESPACE_YM_IGF
