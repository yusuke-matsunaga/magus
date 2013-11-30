
/// @file check_reconvergent.cc
/// @brief 再収斂のチェックを行う
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS

void
check_reconvergent(ImpMgr& mgr)
{
  ymuint n = mgr.node_num();

  for (ymuint i = 0; i < n; ++ i) {
    ImpNode* node = mgr.node(i);

    // node の TFO を求める．
    vector<ymuint> tfo_p;
    vector<ymuint> tfo_n;
    vector<bool> mark(n, false);
    const vector<ImpEdge*>& fo_list = node->fanout_list();
    for (vector<ImpEdge*>::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      const ImpEdge* edge = *p;
      mark_tfo(edge, false, tfo_p, tfo_n, mark);
    }
    sort(tfo_p.begin(), tfo_p.end());
    sort(tfo_n.begin(), tfo_n.end());

  }
}

END_NAMESPACE_YM_NETWORKS
