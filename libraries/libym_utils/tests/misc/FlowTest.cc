
/// @file FlowTest.cc
/// @brief FlowGraph のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "FlowGraph.h"
#include "FlowNode.h"
#include "FlowEdge.h"
#include "ym_utils/BtgMatch.h"


BEGIN_NAMESPACE_YM

int
flow_test(int argc,
	  const char** argv)
{
  FlowGraph fg;
  BtgMatch bm(1, 1);

  ymuint n1 = bm.node1_num();
  ymuint n2 = bm.node2_num();

  vector<FlowNode*> n1_array(n1);
  FlowNode* source = fg.source_node();
  for (ymuint i = 0; i < n1; ++ i) {
    FlowNode* node = fg.new_node();
    n1_array[i] = node;
    fg.new_edge(source, node, 0);
  }

  vector<FlowNode*> n2_array(n2);
  FlowNode* sink = fg.sink_node();
  for (ymuint i = 0; i < n2; ++ i) {
    FlowNode* node = fg.new_node();
    n2_array[i] = node;
    fg.new_edge(node, sink, 0);
  }

  ymuint ne = bm.edge_num();
  ymuint max_weight = 0;
  for (ymuint i = 0; i < ne; ++ i) {
    ymuint id1;
    ymuint id2;
    ymuint weight;
    bm.edge_info(i, id1, id2, weight);
    if ( max_weight < weight ) {
      max_weight = weight;
    }
  }
  for (ymuint i = 0; i < ne; ++ i) {
    ymuint id1;
    ymuint id2;
    ymuint weight;
    bm.edge_info(i, id1, id2, weight);
    ymuint cost = max_weight - weight;
    fg.new_edge(n1_array[id1], n2_array[id2], cost);
  }

  vector<ymuint> edge_list;
  ymuint w = bm.calc_match(edge_list);

  fg.max_flow();

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  return nsYm::flow_test(argc, argv);
}
