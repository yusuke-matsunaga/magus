
/// @file EufMgr_test.cc
/// @brief EufMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufMgr.h"
#include "EufNode.h"


BEGIN_NAMESPACE_YM_LLVMEQ

int
EufMgr_test(int argc,
	    const char** argv)
{
  EufMgr euf_mgr;

  EufNode* node_a = euf_mgr.new_variable("a", false);
  EufNode* node_b = euf_mgr.new_variable("b", false);
  EufNode* node_c = euf_mgr.new_variable("c", false);
  EufNode* node_d = euf_mgr.new_variable("d", false);
  EufNode* node_a_eq_b = euf_mgr.new_equality(node_a, node_b);
  EufNode* node_c_eq_d = euf_mgr.new_equality(node_c, node_d);
  vector<EufNode*> arg_list(2);
  arg_list[0] = node_a;
  arg_list[1] = node_c;
  EufNode* node_f = euf_mgr.new_function("f", arg_list);
  arg_list[0] = node_b;
  arg_list[1] = node_d;
  EufNode* node_g = euf_mgr.new_function("g", arg_list);
  EufNode* node_f_eq_g = euf_mgr.new_equality(node_f, node_g);

  vector<const EufNode*> tmp_list;
  tmp_list.push_back(node_f);
  tmp_list.push_back(node_g);
  tmp_list.push_back(node_a_eq_b);
  tmp_list.push_back(node_c_eq_d);
  tmp_list.push_back(node_f_eq_g);

  display(cout, tmp_list);

  bool stat = euf_mgr.check_validity(node_f_eq_g);
  if ( stat ) {
    cout << "f == g" << endl;
  }
  else {
    cout << "f != g" << endl;
  }

  return 0;
}

END_NAMESPACE_YM_LLVMEQ


int
main(int argc,
     const char** argv)
{
  using nsYm::nsLLVMeq::EufMgr_test;

  return EufMgr_test(argc, argv);
}
