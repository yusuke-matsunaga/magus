
/// @file read_table.cc
/// @brief read_table のメインプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "RwtMgr.h"
#include "RwtNode.h"
#include "NpnNodeMgr.h"
#include "NpnHandle.h"


BEGIN_NAMESPACE_YM

int
read_table(int argc,
	   const char** argv)
{
  RwtMgr rwt_mgr;
  NpnNodeMgr npn_mgr;

  rwt_mgr.init();

  //rwt_mgr.dump(cout);
  ymuint n = rwt_mgr.node_num();
  vector<NpnHandle> handle_map(n);
  for (ymuint i = 0; i < n; ++ i) {
    RwtNode* rwt_node = rwt_mgr.node(i);
    NpnHandle h;
    if ( rwt_node->func() == 0x0000 ) {
      h = npn_mgr.make_const0();
    }
    else if ( rwt_node->func() == 0xaaaa ) {
      h = npn_mgr.make_input(0);
    }
    else if ( rwt_node->func() == 0xcccc ) {
      h = npn_mgr.make_input(1);
    }
    else if ( rwt_node->func() == 0xf0f0 ) {
      h = npn_mgr.make_input(2);
    }
    else if ( rwt_node->func() == 0xff00 ) {
      h = npn_mgr.make_input(3);
    }
    else {
      RwtNode* l = rwt_node->child0();
      assert_cond( l != NULL, __FILE__, __LINE__);
      NpnHandle h0 = handle_map[l->id()];
      if ( rwt_node->inv0() ) {
	h0 = ~h0;
      }

      RwtNode* r = rwt_node->child1();
      assert_cond( r != NULL, __FILE__, __LINE__);
      NpnHandle h1 = handle_map[r->id()];
      if ( rwt_node->inv1() ) {
	h1 = ~h1;
      }

      if ( rwt_node->is_xor() ) {
	h = npn_mgr.make_xor(h0, h1);
      }
      else {
	h = npn_mgr.make_and(h0, h1);
      }
    }
    handle_map[i] = h;
  }
  cout << "# of NpnNodes = " << npn_mgr.node_num() << endl;

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  return nsYm::read_table(argc, argv);
}
