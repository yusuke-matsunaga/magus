
/// @file libym_techmap/tests/patgraph_test.cc
/// @brief PatGraph のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatGraph.h"


BEGIN_NAMESPACE_YM_TECHMAP

ymuint32
encode(ymuint from_id,
       ymuint to_id,
       ymuint fanin_pos,
       bool inv)
{
  ymuint32 ans = 0U;
  ans |= (from_id << 2);
  ans |= (to_id << 17);
  ans |= ((fanin_pos & 1) << 1);
  ans |= static_cast<ymuint>(inv);
  return ans;
}

void
test()
{
  static ymuint32 data[] = {
    5, // ノード数
    3, // 入力数
    4, // 根の ID
    static_cast<ymuint32>(PatGraph::kInput),
    static_cast<ymuint32>(PatGraph::kInput),
    static_cast<ymuint32>(PatGraph::kInput),
    static_cast<ymuint32>(PatGraph::kAnd),
    static_cast<ymuint32>(PatGraph::kXor),
    4, // 枝数
    encode(3, 4, 0, false),
    encode(0, 3, 0, false),
    encode(1, 3, 1, true),
    encode(2, 4, 1, false)
  };

  PatGraph pat_graph(data);

  dump(cout, pat_graph);
}

END_NAMESPACE_YM_TECHMAP


int
main(int argc,
     char* argv[])
{
  nsYm::nsTechmap::test();

  return 0;
}
