
/// @file change_test.cc
/// @brief BdnMgr::change_logic() のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnPort.h"
#include "YmNetworks/BdnNodeHandle.h"
#include "YmNetworks/BdnDumper.h"


BEGIN_NAMESPACE_YM_NETWORKS

void
change_test()
{
  BdnMgr network;
  BdnDumper dump;

  network.set_name("change_test");

  BdnPort* port_a = network.new_input_port("a", 1);
  BdnPort* port_b = network.new_input_port("b", 1);
  BdnPort* port_c = network.new_input_port("c", 1);
  BdnPort* port_d = network.new_output_port("d", 1);

  BdnNode* a = port_a->_input(0);
  BdnNode* b = port_b->_input(0);
  BdnNode* c = port_c->_input(0);
  BdnNode* d = port_d->_output(0);

  // a & b
  BdnNodeHandle h1 = network.new_and(BdnNodeHandle(a, false),
				     BdnNodeHandle(b, false));

  // a & c
  BdnNodeHandle h2 = network.new_and(BdnNodeHandle(a, false),
				     BdnNodeHandle(c, false));

  // (a & b) | (a & c)
  BdnNodeHandle h3 = network.new_or(h1, h2);

  network.change_output_fanin(d, h3);

  cout << "===Before change_logic===" << endl;
  dump(cout, network);

  // (b | c)
  BdnNodeHandle h4 = network.new_or(BdnNodeHandle(b, false),
				    BdnNodeHandle(c, false));

  network.change_and(h3.node(),
		     BdnNodeHandle(a, false),
		     h4);

  cout << "===After change_logic===" << endl;
  dump(cout, network);

  // 論理を反転
  network.change_nand(h3.node(),
		      BdnNodeHandle(a, false),
		      h4);

  cout << "===After change_logic(2)===" << endl;
  dump(cout, network);

}

END_NAMESPACE_YM_NETWORKS


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using nsYm::nsNetworks::change_test;

  change_test();

  return 0;
}
