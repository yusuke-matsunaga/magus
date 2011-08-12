
/// @file makenode_test.cc
/// @brief MvnNode を生成するテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnMgr.h"
#include "ym_networks/MvnModule.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnDumper.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  try {
    MvnMgr mgr;

    MvnModule* module = mgr.new_module("test", 2, 1, 1, 0);

    MvnNode* node3 = mgr.new_through(module, 1);

    MvnNode* node4 = mgr.new_not(module, 2);

    MvnNode* node5 = mgr.new_and(module, 3);

    MvnNode* node6 = mgr.new_or(module, 4);

    MvnNode* node7 = mgr.new_xor(module, 5);

    MvnNode* node8 = mgr.new_rand(module, 6);

    MvnNode* node9 = mgr.new_ror(module, 7);

    MvnNode* node10 = mgr.new_rxor(module, 8);

    MvnNode* node11 = mgr.new_equal(module, 9);

    MvnNode* node12 = mgr.new_lt(module, 10);

    MvnNode* node13 = mgr.new_sll(module, 1, 2, 3);

    MvnNode* node14 = mgr.new_srl(module, 6, 5, 4);

    MvnNode* node15 = mgr.new_sla(module, 1, 1, 1);

    MvnNode* node16 = mgr.new_sra(module, 1, 1, 1);

    MvnNode* node17 = mgr.new_add(module, 32, 32, 33);

    MvnNode* node18 = mgr.new_sub(module, 64, 64, 64);

    MvnNode* node19 = mgr.new_mult(module, 16, 16, 32);

    MvnNode* node20 = mgr.new_div(module, 32, 16, 16);

    MvnNode* node21 = mgr.new_mod(module, 32, 32, 32);

    MvnNode* node22 = mgr.new_pow(module, 32, 32, 32);

    MvnNode* node23 = mgr.new_ite(module, 32);

    vector<ymuint> ibw_array(2);
    ibw_array[0] = 32;
    ibw_array[1] = 30;
    MvnNode* node24 = mgr.new_concat(module, ibw_array);

    MvnModule* module1 = mgr.new_module("sub", 8, 4, 4, 0);

    vector<ymuint32> val(1);
    val[0] = 10;
    MvnNode* node41 = mgr.new_const(module, 8, val);

    MvnDumper dump;
    dump(cout, mgr);
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
