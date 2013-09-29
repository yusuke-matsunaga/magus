
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

  EufNode* node_a = euf_mgr.new_variable("a");
  EufNode* node_b = euf_mgr.new_variable("b");
  EufNode* node_a_eq_b = euf_mgr.new_equality(node_a, node_b);
  EufNode* node_c = euf_mgr.new_variable("c");
  EufNode* node_a_eq_c = euf_mgr.new_equality(node_a, node_c);
  vector<EufNode*> arg_list(3);
  arg_list[0] = node_a;
  arg_list[1] = node_b;
  arg_list[2] = node_c;
  EufNode* node_f = euf_mgr.new_function("f", arg_list);

  vector<const EufNode*> tmp_list;
  tmp_list.push_back(node_a_eq_b);
  tmp_list.push_back(node_a_eq_c);
  tmp_list.push_back(node_f);

  display(cout, tmp_list);

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
