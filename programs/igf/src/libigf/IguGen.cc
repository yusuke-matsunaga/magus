
/// @file IguGen.cc
/// @brief IguGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IguGen.h"
#include "PhfGraph.h"
#include "PhfNode.h"
#include "PhfEdge.h"
#include "FuncVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス IguGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IguGen::IguGen()
{
}

// @brief デストラクタ
IguGen::~IguGen()
{
}

// @brief マッピングを求める．
// @param[in] func_list 関数のリスト
// @param[out] g_list マッピングのリスト
bool
IguGen::mapping(const vector<const FuncVect*>& func_list,
		vector<vector<ymuint32> >& g_list)
{
  PhfGraph pg(func_list);

  if ( !pg.simple_check() ) {
    return false;
  }

  vector<PhfEdge*> edge_list;
  if ( !pg.acyclic_check(edge_list) ) {
    return false;
  }

  ymuint nf = func_list.size();
  ymuint nv = func_list[0]->max_val();
  g_list.clear();
  g_list.resize(nf, vector<ymuint32>(nv, 0U));

  ymuint d = func_list.size();
  ymuint ne = edge_list.size();
  for (ymuint i = 0; i < ne; ++ i) {
    PhfEdge* edge = edge_list[i];
    for (ymuint j = 0; j < d; ++ j) {
      PhfNode* node1 = edge->node(j);
      if ( !node1->is_assigned() ) {
	ymuint32 val = edge->val();
	for (ymuint k = 0; k < d; ++ k) {
	  if ( k != j ) {
	    PhfNode* node2 = edge->node(k);
	    val ^= node2->val();
	  }
	}
	node1->set_val(val);
	vector<ymuint32>& g1 = g_list[j];
	g1[node1->pat()] = val;
      }
    }
  }

  return true;
}

// @brief collision free 分割を行う．
// @param[in] func_list 関数のリスト
// @param[out] block_map 分割を表す配列
bool
IguGen::cf_partition(const vector<const FuncVect*>& func_list,
		     vector<ymuint>& block_map)
{
  PhfGraph pg(func_list);

  return pg.cf_partition(block_map);
}

// @brief displace_decomposition を行う．
bool
IguGen::displace_decomposition(const FuncVect* func1,
			       const FuncVect* func2,
			       vector<ymuint>& displace_map,
			       bool use_xor)
{
  vector<const FuncVect*> func_list(2);
  func_list[0] = func1;
  func_list[1] = func2;
  PhfGraph pg(func_list);

  return pg.displace_decomposition(displace_map, use_xor);
}

END_NAMESPACE_YM_IGF
