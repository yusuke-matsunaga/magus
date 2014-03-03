
/// @file base_test.cc
/// @brief BdnMgr の基本機能のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/BdnMgr.h"
#include "networks/BdnPort.h"
#include "networks/BdnNode.h"
#include "networks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKS

bool
base_test()
{
  BdnMgr network;

  bool result = true;

  // name(), set_name() のテスト
  network.set_name("foo");
  if ( network.name() != "foo" ) {
    cout << "ERROR[set_name]: name() != \"foo\"" << endl;
    result = false;
  }
  network.set_name("bar");
  if ( network.name() != "bar" ) {
    cout << "ERROR[set_name]: name() != \"bar\"" << endl;
    result = false;
  }

  // new_input_port() のテスト
  if ( network.input_num() != 0 ) {
    cout << "ERROR[new_input_test]: input_num() != 0" << endl;
    result = false;
  }
  BdnPort* port0 = network.new_input_port("port0", 1);
  BdnNode* input0 = port0->_input(0);
  if ( network.input_num() != 1 ) {
    cout << "ERROR[new_input_test]: input_num() != 1" << endl;
    result = false;
  }
  if ( input0 == NULL ) {
    cout << "ERROR[new_input_test]: input0 == NULL" << endl;
    result = false;
  }

  BdnPort* port1 = network.new_input_port("port1", 2);
  BdnNode* input1_0 = port1->_input(0);
  BdnNode* input1 = port1->_input(1);
  if ( network.input_num() != 3 ) {
    cout << "ERROR[new_input_test]: input_num() != 3" << endl;
    result = false;
  }
  if ( input1 == NULL ) {
    cout << "ERROR[new_input_test]: input1_1 == NULL" << endl;
    result = false;
  }

  // new_logic() のテスト

  // 0 & 1 を作る．
  BdnNodeHandle and11_h = network.new_and(BdnNodeHandle(input0, false),
					  BdnNodeHandle(input1, false));
  if ( network.lnode_num() != 1 ) {
    cout << "ERROR[new_logic_test]: lnode_num() != 1" << endl;
    result = false;
  }
  if ( and11_h.node() == NULL ) {
    cout << "ERROR[new_logic_test]: and11_h.node() == NULL" << endl;
    result = false;
  }
  if ( and11_h.inv() == true ) {
    cout << "ERROR[new_logic_test]: and11_h.inv() == true" << endl;
    result = false;
  }
  BdnNode* and11 = and11_h.node();
  if ( !and11->is_and() ) {
    cout << "ERROR[new_logic_test]: !and11->is_and()" << endl;
    result = false;
  }
  if ( and11->fanin0() != input0 ) {
    cout << "ERROR[new_logic_test]: and11->fanin0() != input0" << endl;
    result = false;
  }
  if ( and11->fanin0_inv() ) {
    cout << "ERROR[new_logic_test]: and11->fanin0_inv()" << endl;
    result = false;
  }
  if ( and11->fanin1() != input1 ) {
    cout << "ERROR[new_logic_test]: and11->fanin1() != input1" << endl;
    result = false;
  }
  if ( and11->fanin1_inv() ) {
    cout << "ERROR[new_logic_test]: and11->fanin1_inv()" << endl;
    result = false;
  }

  // 0 & 1 を探す．
  BdnNodeHandle new_handle = network.find_and(BdnNodeHandle(input0, false),
					      BdnNodeHandle(input1, false));
  if ( new_handle.is_error() ) {
    cout << "ERROR[find_and_test]: and11_h not found." << endl;
    result = false;
  }
  if ( new_handle != and11_h ) {
    cout << "ERROR[find_and_test]: new_handle != and11_h." << endl;
    result = false;
  }

  BdnNodeHandle and11_h2 = network.new_and(~BdnNodeHandle(input0, true),
					   BdnNodeHandle(input1, false));
  if ( and11_h2 != and11_h ) {
    cout << "ERROR[new_logic_test]: and11_h2 != and11_h" << endl;
    result = false;
  }

  BdnNodeHandle and11_h3 = network.new_and(BdnNodeHandle(input0, false),
					   ~BdnNodeHandle(input1, true));
  if ( and11_h3 != and11_h ) {
    cout << "ERROR[new_logic_test]: and11_h3 != and11_h" << endl;
    result = false;
  }

  BdnNodeHandle and11_h4 = network.new_and(~BdnNodeHandle(input0, true),
					   ~BdnNodeHandle(input1, true));
  if ( and11_h4 != and11_h ) {
    cout << "ERROR[new_logic_test]: and11_h4 != and11_h" << endl;
    result = false;
  }

  BdnNodeHandle and11_h5 = network.new_and(BdnNodeHandle(input1, false),
					   BdnNodeHandle(input0, false));
  if ( and11_h5 != and11_h ) {
    cout << "ERROR[new_logic_test]: and11_h5 != and11_h" << endl;
    result = false;
  }

  // ~0 & 1 を作る．
  BdnNodeHandle and01_h = network.new_and(BdnNodeHandle(input0, true),
					  BdnNodeHandle(input1, false));
  if ( network.lnode_num() != 2 ) {
    cout << "ERROR[new_logic_test]: lnode_num() != 2" << endl;
    result = false;
  }
  if ( and01_h.node() == NULL ) {
    cout << "ERROR[new_logic_test]: and01_h.node() == NULL" << endl;
    result = false;
  }
  if ( and01_h.inv() == true ) {
    cout << "ERROR[new_logic_test]: and01_h.inv() == true" << endl;
    result = false;
  }
  BdnNode* and01 = and01_h.node();
  if ( !and01->is_and() ) {
    cout << "ERROR[new_logic_test]: !and01->is_and()" << endl;
    result = false;
  }
  if ( and01->fanin0() != input0 ) {
    cout << "ERROR[new_logic_test]: and01->fanin0() != input0" << endl;
    result = false;
  }
  if ( !and01->fanin0_inv() ) {
    cout << "ERROR[new_logic_test]: !and01->fanin0_inv()" << endl;
    result = false;
  }
  if ( and01->fanin1() != input1 ) {
    cout << "ERROR[new_logic_test]: and01->fanin1() != input1" << endl;
    result = false;
  }
  if ( and01->fanin1_inv() ) {
    cout << "ERROR[new_logic_test]: and01->fanin1_inv()" << endl;
    result = false;
  }

  BdnNodeHandle and01_h2 = network.new_and(~BdnNodeHandle(input0, false),
					   BdnNodeHandle(input1, false));
  if ( and01_h2 != and01_h ) {
    cout << "ERROR[new_logic_test]: and01_h2 != and01_h" << endl;
    result = false;
  }

  BdnNodeHandle and01_h3 = network.new_and(~BdnNodeHandle(input0, false),
					   ~BdnNodeHandle(input1, true));
  if ( and01_h3 != and01_h ) {
    cout << "ERROR[new_logic_test]: and01_h3 != and01_h" << endl;
    result = false;
  }

  BdnNodeHandle and01_h4 = network.new_and(BdnNodeHandle(input0, true),
					   ~BdnNodeHandle(input1, true));
  if ( and01_h4 != and01_h ) {
    cout << "ERROR[new_logic_test]: and01_h4 != and01_h" << endl;
    result = false;
  }

  // 0 & ~1 を探す(見つからないはず)
  BdnNodeHandle and10_h = network.find_and(BdnNodeHandle(input0, false),
					   BdnNodeHandle(input1, true));
  if ( !and10_h.is_error() ) {
    cout << "ERROR[find_and_test]: and10_h found." << endl;
    result = false;
  }

  // 定数0との XOR
  BdnNodeHandle tmp1_h = network.new_xor(BdnNodeHandle::make_zero(),
					 and11_h);
  if ( tmp1_h != and11_h ) {
    cout << "ERROR[new_logic_test]: tmp1_h != and11_h" << endl;
    result = false;
  }

  // 定数1との XOR
  BdnNodeHandle tmp2_h = network.new_xor(and11_h,
					 BdnNodeHandle::make_one());
  if ( tmp2_h != ~and11_h ) {
    cout << "ERROR[new_logic_test]: tmp2_h != ~and11_h" << endl;
    result = false;
  }

  // 自分自身との AND
  BdnNodeHandle tmp3_h = network.new_and(and11_h, and11_h);
  if ( tmp3_h != and11_h ) {
    cout << "ERROR[new_logic_test]: tmp3_h != and11_h" << endl;
    result = false;
  }

  // 自分自身との XOR
  BdnNodeHandle tmp4_h = network.new_xor(and11_h, and11_h);
  if ( tmp4_h != and11_h ) {
    cout << "ERROR[new_logic_test]: tmp4_h != and11_h." << endl;
    result = false;
  }

  // 自分自身の否定との XOR
  BdnNodeHandle tmp5_h = network.new_xor(and11_h, ~and11_h);
  if ( tmp5_h != BdnNodeHandle::make_zero() ) {
    cout << "ERROR[new_logic_test]: tmp5_h != 0" << endl;
    result = false;
  }

  // 定数0との AND
  BdnNodeHandle tmp6_h = network.new_and(BdnNodeHandle::make_zero(),
					 and11_h);
  if ( tmp6_h != BdnNodeHandle::make_zero() ) {
    cout << "ERROR[new_logic_test]: tmp6_h != 0" << endl;
    result = false;
  }

  // 定数1との AND
  BdnNodeHandle tmp7_h = network.new_and(and11_h,
					 BdnNodeHandle::make_one());
  if ( tmp7_h != and11_h ) {
    cout << "ERROR[new_logic_test]: tmp7_h != and11_h" << endl;
    result = false;
  }

  return result;
}

END_NAMESPACE_YM_NETWORKS

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsNetworks;

  if ( !base_test() ) {
    return 255;
  }

  return 0;
}
