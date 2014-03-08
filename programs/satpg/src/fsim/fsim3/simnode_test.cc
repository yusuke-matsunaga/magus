
/// @file simnode_test.cc
/// @brief  SimNode::calc_gval(), calc_fval() のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "fsim3_nsdef.h"
#include "PackedVal.h"
#include "SimNode.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

PackedVal
val_0(ymuint val)
{
  switch ( val ) {
  case 0: return kPvAll1;
  case 1:
  case 2: return kPvAll0;
  }
  return kPvAll0;
}

PackedVal
val_1(ymuint val)
{
  switch ( val ) {
  case 1: return kPvAll1;
  case 0:
  case 2: return kPvAll0;
  }
  return kPvAll0;
}

ymuint buff_table[3] = {
  0, 1, 2
};

ymuint and_table[9] = {
  0, 0, 0,
  0, 1, 2,
  0, 2, 2
};

ymuint or_table[9] = {
  0, 1, 2,
  1, 1, 1,
  2, 1, 2
};

ymuint xor_table[9] = {
  0, 1, 2,
  1, 0, 2,
  2, 2, 2
};

void
dump_pval(ostream& s,
	  PackedVal val)
{
  for (ymuint i = 0; i < 64; ++ i) {
    if ( val & (1ULL << i) ) {
      s << '1';
    }
    else {
      s << '0';
    }
  }
}

void
dump_pval3(ostream& s,
	   PackedVal val0,
	   PackedVal val1)
{
  dump_pval(s, val0);
  s << ", ";
  dump_pval(s, val1);
}

bool
gval_test(SimNode* node,
	  ymuint o_expected,
	  const char* testname)
{
  bool stat = node->calc_gval3();

  PackedVal oval_0 = val_0(o_expected);
  PackedVal oval_1 = val_1(o_expected);

  bool has_error = false;
  if ( node->gval_0() != oval_0 ||
       node->gval_1() != oval_1 ||
       (stat != (o_expected != 2)) ) {
    cerr << testname << " gval failed." << endl;
    ymuint ni = node->nfi();
    for (ymuint i = 0; i < ni; ++ i) {
      SimNode* inode = node->fanin(i);
      cerr << "input" << i << "->gval: ";
      dump_pval3(cerr, inode->gval_0(), inode->gval_1());
      cerr << endl;
    }
    cerr << "output(exp): ";
    dump_pval3(cerr, oval_0, oval_1);
    cerr << endl;
    cerr << "node->gval: ";
    dump_pval3(cerr, node->gval_0(), node->gval_1());
    cerr << endl;
    has_error = true;
  }
  if ( node->fval_0() != oval_0 ||
       node->fval_1() != oval_1 ) {
    cerr << testname << " fval(gval) failed." << endl;
    ymuint ni = node->nfi();
    for (ymuint i = 0; i < ni; ++ i) {
      SimNode* inode = node->fanin(i);
      cerr << "input" << i << "->gval: ";
      dump_pval3(cerr, inode->gval_0(), inode->gval_1());
      cerr << endl;
    }
    cerr << "output(exp): ";
    dump_pval3(cerr, oval_0, oval_1);
    cerr << endl;
    cerr << "node->fval: ";
    dump_pval3(cerr, node->fval_0(), node->fval_1());
    cerr << endl;
    has_error = true;
  }

  return has_error;
}

bool
fval_test(SimNode* node,
	  ymuint o_expected,
	  const char* testname)
{
  node->set_fmask(kPvAll1);
  bool stat = node->calc_fval3(kPvAll1);

  bool stat_exp = (node->gval_0() != node->fval_0()) || (node->gval_1() != node->fval_1());

  PackedVal oval_0 = val_0(o_expected);
  PackedVal oval_1 = val_1(o_expected);

  bool has_error = false;
  if ( node->fval_0() != oval_0 ||
       node->fval_1() != oval_1 ||
       stat != stat_exp ) {
    cerr << testname << " fval failed." << endl;
    ymuint ni = node->nfi();
    for (ymuint i = 0; i < ni; ++ i) {
      SimNode* inode = node->fanin(i);
      cerr << "input" << i << "->fval: ";
      dump_pval3(cerr, inode->fval_0(), inode->fval_1());
      cerr << endl;
    }
    cerr << "output(exp): ";
    dump_pval3(cerr, oval_0, oval_1);
    cerr << endl;
    cerr << "node->fval: ";
    dump_pval3(cerr, node->fval_0(), node->fval_1());
    cerr << endl;
    has_error = true;
  }

  return has_error;
}

bool
node1_test(SimNode* node,
	   const ymuint vals[],
	   bool inv,
	   const char* testname)
{
  cerr << testname << endl;

  SimNode* input0 = node->fanin(0);

  bool has_error = false;
  for (ymuint i = 0; i < 3; ++ i) {
    PackedVal ival_0 = val_0(i);
    PackedVal ival_1 = val_1(i);

    input0->set_gval(ival_0, ival_1);

    ymuint o = vals[i];
    if ( inv ) {
      if ( o == 0 ) {
	o = 1;
      }
      else if ( o == 1 ) {
	o = 0;
      }
    }

    if ( gval_test(node, o, testname) ) {
      has_error = true;
    }
  }

  for (ymuint i = 0; i < 3; ++ i) {
    PackedVal ival_0 = val_0(i);
    PackedVal ival_1 = val_1(i);

    input0->set_fval(ival_0, ival_1);

    ymuint o = vals[i];
    if ( inv ) {
      if ( o == 0 ) {
	o = 1;
      }
      else if ( o == 1 ) {
	o = 0;
      }
    }

    if ( fval_test(node, o, testname) ) {
      has_error = true;
    }
  }

  return has_error;
}

bool
node2_test(SimNode* node,
	   const ymuint vals[],
	   bool inv,
	   const char* testname)
{
  cerr << testname << endl;

  SimNode* input0 = node->fanin(0);
  SimNode* input1 = node->fanin(1);

  bool has_error = false;
  for (ymuint i0 = 0; i0 < 3; ++ i0) {
    PackedVal ival0_0 = val_0(i0);
    PackedVal ival0_1 = val_1(i0);
    for (ymuint i1 = 0; i1 < 3; ++ i1) {
      PackedVal ival1_0 = val_0(i1);
      PackedVal ival1_1 = val_1(i1);

      input0->set_gval(ival0_0, ival0_1);
      input1->set_gval(ival1_0, ival1_1);

      ymuint o = vals[i0 * 3 + i1];
      if ( inv ) {
	if ( o == 0 ) {
	  o = 1;
	}
	else if ( o == 1 ) {
	  o = 0;
	}
      }

      if ( gval_test(node, o, testname) ) {
	has_error = true;
      }
    }
  }

  for (ymuint i0 = 0; i0 < 3; ++ i0) {
    PackedVal ival0_0 = val_0(i0);
    PackedVal ival0_1 = val_1(i0);
    for (ymuint i1 = 0; i1 < 3; ++ i1) {
      PackedVal ival1_0 = val_0(i1);
      PackedVal ival1_1 = val_1(i1);

      input0->set_fval(ival0_0, ival0_1);
      input1->set_fval(ival1_0, ival1_1);

      ymuint o = vals[i0 * 3 + i1];
      if ( inv ) {
	if ( o == 0 ) {
	  o = 1;
	}
	else if ( o == 1 ) {
	  o = 0;
	}
      }

      if ( fval_test(node, o, testname) ) {
	has_error = true;
      }
    }
  }

  return has_error;
}

bool
node3_test(SimNode* node,
	   const ymuint vals[],
	   bool inv,
	   const char* testname)
{
  cerr << testname << endl;

  SimNode* input0 = node->fanin(0);
  SimNode* input1 = node->fanin(1);
  SimNode* input2 = node->fanin(2);

  bool has_error = false;
  for (ymuint i0 = 0; i0 < 3; ++ i0) {
    PackedVal ival0_0 = val_0(i0);
    PackedVal ival0_1 = val_1(i0);
    for (ymuint i1 = 0; i1 < 3; ++ i1) {
      PackedVal ival1_0 = val_0(i1);
      PackedVal ival1_1 = val_1(i1);
      for (ymuint i2 = 0; i2 < 3; ++ i2) {
	PackedVal ival2_0 = val_0(i2);
	PackedVal ival2_1 = val_1(i2);

	input0->set_gval(ival0_0, ival0_1);
	input1->set_gval(ival1_0, ival1_1);
	input2->set_gval(ival2_0, ival2_1);

	ymuint tmp = vals[i0 * 3 + i1];
	ymuint o = vals[tmp * 3 + i2];
	if ( inv ) {
	  if ( o == 0 ) {
	    o = 1;
	  }
	  else if ( o == 1 ) {
	    o = 0;
	  }
	}

	if ( gval_test(node, o, testname) ) {
	  has_error = true;
	}
      }
    }
  }

  for (ymuint i0 = 0; i0 < 3; ++ i0) {
    PackedVal ival0_0 = val_0(i0);
    PackedVal ival0_1 = val_1(i0);
    for (ymuint i1 = 0; i1 < 3; ++ i1) {
      PackedVal ival1_0 = val_0(i1);
      PackedVal ival1_1 = val_1(i1);
      for (ymuint i2 = 0; i2 < 3; ++ i2) {
	PackedVal ival2_0 = val_0(i2);
	PackedVal ival2_1 = val_1(i2);

	input0->set_fval(ival0_0, ival0_1);
	input1->set_fval(ival1_0, ival1_1);
	input2->set_fval(ival2_0, ival2_1);

	ymuint tmp = vals[i0 * 3 + i1];
	ymuint o = vals[tmp * 3 + i2];
	if ( inv ) {
	  if ( o == 0 ) {
	    o = 1;
	  }
	  else if ( o == 1 ) {
	    o = 0;
	  }
	}

	if ( fval_test(node, o, testname) ) {
	  has_error = true;
	}
      }
    }
  }

  return has_error;
}

bool
node4_test(SimNode* node,
	   const ymuint vals[],
	   bool inv,
	   const char* testname)
{
  cerr << testname << endl;

  SimNode* input0 = node->fanin(0);
  SimNode* input1 = node->fanin(1);
  SimNode* input2 = node->fanin(2);
  SimNode* input3 = node->fanin(3);

  bool has_error = false;
  for (ymuint i0 = 0; i0 < 3; ++ i0) {
    PackedVal ival0_0 = val_0(i0);
    PackedVal ival0_1 = val_1(i0);
    for (ymuint i1 = 0; i1 < 3; ++ i1) {
      PackedVal ival1_0 = val_0(i1);
      PackedVal ival1_1 = val_1(i1);
      for (ymuint i2 = 0; i2 < 3; ++ i2) {
	PackedVal ival2_0 = val_0(i2);
	PackedVal ival2_1 = val_1(i2);
	for (ymuint i3 = 0; i3 < 3; ++ i3) {
	  PackedVal ival3_0 = val_0(i3);
	  PackedVal ival3_1 = val_1(i3);

	  input0->set_gval(ival0_0, ival0_1);
	  input1->set_gval(ival1_0, ival1_1);
	  input2->set_gval(ival2_0, ival2_1);
	  input3->set_gval(ival3_0, ival3_1);

	  ymuint tmp = vals[i0 * 3 + i1];
	  tmp = vals[tmp * 3 + i2];
	  ymuint o = vals[tmp * 3 + i3];
	  if ( inv ) {
	    if ( o == 0 ) {
	      o = 1;
	    }
	    else if ( o == 1 ) {
	      o = 0;
	    }
	  }

	  if ( gval_test(node, o, testname) ) {
	    has_error = true;
	  }
	}
      }
    }
  }

  for (ymuint i0 = 0; i0 < 3; ++ i0) {
    PackedVal ival0_0 = val_0(i0);
    PackedVal ival0_1 = val_1(i0);
    for (ymuint i1 = 0; i1 < 3; ++ i1) {
      PackedVal ival1_0 = val_0(i1);
      PackedVal ival1_1 = val_1(i1);
      for (ymuint i2 = 0; i2 < 3; ++ i2) {
	PackedVal ival2_0 = val_0(i2);
	PackedVal ival2_1 = val_1(i2);
	for (ymuint i3 = 0; i3 < 3; ++ i3) {
	  PackedVal ival3_0 = val_0(i3);
	  PackedVal ival3_1 = val_1(i3);

	  input0->set_fval(ival0_0, ival0_1);
	  input1->set_fval(ival1_0, ival1_1);
	  input2->set_fval(ival2_0, ival2_1);
	  input3->set_fval(ival3_0, ival3_1);

	  ymuint tmp = vals[i0 * 3 + i1];
	  tmp = vals[tmp * 3 + i2];
	  ymuint o = vals[tmp * 3 + i3];
	  if ( inv ) {
	    if ( o == 0 ) {
	      o = 1;
	    }
	    else if ( o == 1 ) {
	      o = 0;
	    }
	  }

	  if ( fval_test(node, o, testname) ) {
	    has_error = true;
	  }
	}
      }
    }
  }

  return has_error;
}

bool
node5_test(SimNode* node,
	   const ymuint vals[],
	   bool inv,
	   const char* testname)
{
  cerr << testname << endl;

  SimNode* input0 = node->fanin(0);
  SimNode* input1 = node->fanin(1);
  SimNode* input2 = node->fanin(2);
  SimNode* input3 = node->fanin(3);
  SimNode* input4 = node->fanin(4);

  bool has_error = false;
  for (ymuint i0 = 0; i0 < 3; ++ i0) {
    PackedVal ival0_0 = val_0(i0);
    PackedVal ival0_1 = val_1(i0);
    for (ymuint i1 = 0; i1 < 3; ++ i1) {
      PackedVal ival1_0 = val_0(i1);
      PackedVal ival1_1 = val_1(i1);
      for (ymuint i2 = 0; i2 < 3; ++ i2) {
	PackedVal ival2_0 = val_0(i2);
	PackedVal ival2_1 = val_1(i2);
	for (ymuint i3 = 0; i3 < 3; ++ i3) {
	  PackedVal ival3_0 = val_0(i3);
	  PackedVal ival3_1 = val_1(i3);
	  for (ymuint i4 = 0; i4 < 3; ++ i4) {
	    PackedVal ival4_0 = val_0(i4);
	    PackedVal ival4_1 = val_1(i4);

	    input0->set_gval(ival0_0, ival0_1);
	    input1->set_gval(ival1_0, ival1_1);
	    input2->set_gval(ival2_0, ival2_1);
	    input3->set_gval(ival3_0, ival3_1);
	    input4->set_gval(ival4_0, ival4_1);

	    ymuint tmp = vals[i0 * 3 + i1];
	    tmp = vals[tmp * 3 + i2];
	    tmp = vals[tmp * 3 + i3];
	    ymuint o = vals[tmp * 3 + i4];
	    if ( inv ) {
	      if ( o == 0 ) {
		o = 1;
	      }
	      else if ( o == 1 ) {
		o = 0;
	      }
	    }

	    if ( gval_test(node, o, testname) ) {
	      has_error = true;
	    }
	  }
	}
      }
    }
  }

  for (ymuint i0 = 0; i0 < 3; ++ i0) {
    PackedVal ival0_0 = val_0(i0);
    PackedVal ival0_1 = val_1(i0);
    for (ymuint i1 = 0; i1 < 3; ++ i1) {
      PackedVal ival1_0 = val_0(i1);
      PackedVal ival1_1 = val_1(i1);
      for (ymuint i2 = 0; i2 < 3; ++ i2) {
	PackedVal ival2_0 = val_0(i2);
	PackedVal ival2_1 = val_1(i2);
	for (ymuint i3 = 0; i3 < 3; ++ i3) {
	  PackedVal ival3_0 = val_0(i3);
	  PackedVal ival3_1 = val_1(i3);
	  for (ymuint i4 = 0; i4 < 3; ++ i4) {
	    PackedVal ival4_0 = val_0(i4);
	    PackedVal ival4_1 = val_1(i4);

	    input0->set_fval(ival0_0, ival0_1);
	    input1->set_fval(ival1_0, ival1_1);
	    input2->set_fval(ival2_0, ival2_1);
	    input3->set_fval(ival3_0, ival3_1);
	    input4->set_fval(ival4_0, ival4_1);

	    ymuint tmp = vals[i0 * 3 + i1];
	    tmp = vals[tmp * 3 + i2];
	    tmp = vals[tmp * 3 + i3];
	    ymuint o = vals[tmp * 3 + i4];
	    if ( inv ) {
	      if ( o == 0 ) {
		o = 1;
	      }
	      else if ( o == 1 ) {
		o = 0;
	      }
	    }

	    if ( fval_test(node, o, testname) ) {
	      has_error = true;
	    }
	  }
	}
      }
    }
  }

  return has_error;
}

bool
buff_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* node = SimNode::new_node(1, kTgGateBuff, vector<SimNode*>(1, input0));

  return node1_test(node, buff_table, false, "buff_test");
}

bool
not_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* node = SimNode::new_node(1, kTgGateNot, vector<SimNode*>(1, input0));

  return node1_test(node, buff_table, true, "not_test");
}

bool
and2_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  vector<SimNode*> inputs(2);
  inputs[0] = input0;
  inputs[1] = input1;
  SimNode* node = SimNode::new_node(2, kTgGateAnd, inputs);

  return node2_test(node, and_table, false, "and2_test");
}

bool
and3_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  vector<SimNode*> inputs(3);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  SimNode* node = SimNode::new_node(3, kTgGateAnd, inputs);

  return node3_test(node, and_table, false, "and3_test");
}

bool
and4_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  SimNode* input3 = SimNode::new_input(3);
  vector<SimNode*> inputs(4);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  inputs[3] = input3;
  SimNode* node = SimNode::new_node(4, kTgGateAnd, inputs);

  return node4_test(node, and_table, false, "and4_test");
}

bool
and5_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  SimNode* input3 = SimNode::new_input(3);
  SimNode* input4 = SimNode::new_input(4);
  vector<SimNode*> inputs(5);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  inputs[3] = input3;
  inputs[4] = input4;
  SimNode* node = SimNode::new_node(5, kTgGateAnd, inputs);

  return node5_test(node, and_table, false, "and5_test");
}

bool
nand2_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  vector<SimNode*> inputs(2);
  inputs[0] = input0;
  inputs[1] = input1;
  SimNode* node = SimNode::new_node(2, kTgGateNand, inputs);

  return node2_test(node, and_table, true, "nand2_test");
}

bool
nand3_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  vector<SimNode*> inputs(3);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  SimNode* node = SimNode::new_node(3, kTgGateNand, inputs);

  return node3_test(node, and_table, true, "nand3_test");
}

bool
nand4_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  SimNode* input3 = SimNode::new_input(3);
  vector<SimNode*> inputs(4);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  inputs[3] = input3;
  SimNode* node = SimNode::new_node(4, kTgGateNand, inputs);

  return node4_test(node, and_table, true, "nand4_test");
}

bool
nand5_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  SimNode* input3 = SimNode::new_input(3);
  SimNode* input4 = SimNode::new_input(4);
  vector<SimNode*> inputs(5);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  inputs[3] = input3;
  inputs[4] = input4;
  SimNode* node = SimNode::new_node(5, kTgGateNand, inputs);

  return node5_test(node, and_table, true, "nand5_test");
}

bool
or2_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  vector<SimNode*> inputs(2);
  inputs[0] = input0;
  inputs[1] = input1;
  SimNode* node = SimNode::new_node(2, kTgGateOr, inputs);

  return node2_test(node, or_table, false, "or2_test");
}

bool
or3_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  vector<SimNode*> inputs(3);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  SimNode* node = SimNode::new_node(3, kTgGateOr, inputs);

  return node3_test(node, or_table, false, "or3_test");
}

bool
or4_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  SimNode* input3 = SimNode::new_input(3);
  vector<SimNode*> inputs(4);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  inputs[3] = input3;
  SimNode* node = SimNode::new_node(4, kTgGateOr, inputs);

  return node4_test(node, or_table, false, "or4_test");
}

bool
or5_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  SimNode* input3 = SimNode::new_input(3);
  SimNode* input4 = SimNode::new_input(4);
  vector<SimNode*> inputs(5);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  inputs[3] = input3;
  inputs[4] = input4;
  SimNode* node = SimNode::new_node(5, kTgGateOr, inputs);

  return node5_test(node, or_table, false, "or5_test");
}

bool
nor2_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  vector<SimNode*> inputs(2);
  inputs[0] = input0;
  inputs[1] = input1;
  SimNode* node = SimNode::new_node(2, kTgGateNor, inputs);

  return node2_test(node, or_table, true, "nor2_test");
}

bool
nor3_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  vector<SimNode*> inputs(3);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  SimNode* node = SimNode::new_node(3, kTgGateNor, inputs);

  return node3_test(node, or_table, true, "nor3_test");
}

bool
nor4_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  SimNode* input3 = SimNode::new_input(3);
  vector<SimNode*> inputs(4);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  inputs[3] = input3;
  SimNode* node = SimNode::new_node(4, kTgGateNor, inputs);

  return node4_test(node, or_table, true, "nor4_test");
}

bool
nor5_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  SimNode* input3 = SimNode::new_input(3);
  SimNode* input4 = SimNode::new_input(4);
  vector<SimNode*> inputs(5);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  inputs[3] = input3;
  inputs[4] = input4;
  SimNode* node = SimNode::new_node(5, kTgGateNor, inputs);

  return node5_test(node, or_table, true, "nor5_test");
}

bool
xor2_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  vector<SimNode*> inputs(2);
  inputs[0] = input0;
  inputs[1] = input1;
  SimNode* node = SimNode::new_node(2, kTgGateXor, inputs);

  return node2_test(node, xor_table, false, "xor2_test");
}

bool
xor3_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  vector<SimNode*> inputs(3);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  SimNode* node = SimNode::new_node(3, kTgGateXor, inputs);

  return node3_test(node, xor_table, false, "xor3_test");
}

bool
xnor2_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  vector<SimNode*> inputs(2);
  inputs[0] = input0;
  inputs[1] = input1;
  SimNode* node = SimNode::new_node(2, kTgGateXnor, inputs);

  return node2_test(node, xor_table, true, "xnor2_test");
}

bool
xnor3_test()
{
  SimNode* input0 = SimNode::new_input(0);
  SimNode* input1 = SimNode::new_input(1);
  SimNode* input2 = SimNode::new_input(2);
  vector<SimNode*> inputs(3);
  inputs[0] = input0;
  inputs[1] = input1;
  inputs[2] = input2;
  SimNode* node = SimNode::new_node(3, kTgGateXnor, inputs);

  return node3_test(node, xor_table, true, "xnor3_test");
}

bool
simnode_test()
{
  bool has_error = false;

  if ( buff_test() ) {
    has_error = true;
  }

  if ( not_test() ) {
    has_error = true;
  }

  if ( and2_test() ) {
    has_error = true;
  }

  if ( and3_test() ) {
    has_error = true;
  }

  if ( and4_test() ) {
    has_error = true;
  }

  if ( and5_test() ) {
    has_error = true;
  }

  if ( nand2_test() ) {
    has_error = true;
  }

  if ( nand3_test() ) {
    has_error = true;
  }

  if ( nand4_test() ) {
    has_error = true;
  }

  if ( nand5_test() ) {
    has_error = true;
  }

  if ( or2_test() ) {
    has_error = true;
  }

  if ( or3_test() ) {
    has_error = true;
  }

  if ( or4_test() ) {
    has_error = true;
  }

  if ( or5_test() ) {
    has_error = true;
  }

  if ( nor2_test() ) {
    has_error = true;
  }

  if ( nor3_test() ) {
    has_error = true;
  }

  if ( nor4_test() ) {
    has_error = true;
  }

  if ( nor5_test() ) {
    has_error = true;
  }

  if ( xor2_test() ) {
    has_error = true;
  }

  if ( xor3_test() ) {
    has_error = true;
  }

  if ( xnor2_test() ) {
    has_error = true;
  }

  if ( xnor3_test() ) {
    has_error = true;
  }

  return has_error;
}

END_NAMESPACE_YM_SATPG_FSIM


int
main(int argc,
     const char** argv)
{
  if ( nsYm::nsSatpg::nsFsim3:: simnode_test() ) {
    return 1;
  }
  return 0;
}
