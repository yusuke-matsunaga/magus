
/// @file libym_logic/test/bdd/makebdd.cc
/// @brief 論理回路の BDD を作るプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Bdd.h"
#include "ym_logic/BddMgr.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnBlifReader.h"
#include "ym_networks/BdnNode.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"

#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM

bool
makebdd(const string& filename)
{
  MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(msg_handler);

  BdnBlifReader read;
  BdnMgr network;

  if ( !read(filename, network) ) {
    cerr << "Error in reading " << filename << endl;
    return false;
  }

  BddMgr bddmgr("bmc", "Bdd Manager");

  BddMgrParam param;
  param.mMemLimit = 0;
  bddmgr.param(param, BddMgrParam::MEM_LIMIT);
  //bddmgr.set_logstream(cerr);

  {
    ymuint n = network.max_node_id();
    vector<Bdd> bddmap(n);

    const BdnNodeList& input_list = network.input_list();
    ymuint id = 0;
    for (BdnNodeList::const_iterator p = input_list.begin();
	 p != input_list.end(); ++ p) {
      const BdnNode* node = *p;
      Bdd bdd = bddmgr.make_posiliteral(VarId(id));
      ++ id;
      bddmap[node->id()] = bdd;
    }

    vector<const BdnNode*> node_list;
    network.sort(node_list);
    ymuint node_num = node_list.size();
    id = 0;
    for (vector<const BdnNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const BdnNode* node = *p;
      cout << id << " / " << node_num;
      ++ id;
      const BdnNode* fanin0 = node->fanin0();
      Bdd bdd0 = bddmap[fanin0->id()];
      if ( node->fanin0_inv() ) {
	bdd0 = ~bdd0;
      }
      const BdnNode* fanin1 = node->fanin1();
      Bdd bdd1 = bddmap[fanin1->id()];
      if ( node->fanin1_inv() ) {
	bdd1 = ~bdd1;
      }
#if 0
      cout << " " << bdd0.size() << " x " << bdd1.size();
      cout.flush();
#endif
      if ( node->is_and() ) {
	Bdd bdd = bdd0 & bdd1;
	bddmap[node->id()] = bdd;
      }
      else if ( node->is_xor() ) {
	Bdd bdd = bdd0 ^ bdd1;
	bddmap[node->id()] = bdd;
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
#if 0
      cout << " = " << bddmap[node->id()].size();
#endif
      cout << endl;
    }

    bddmgr.disable_gc();
  }

  return true;
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " blif-file" << endl;
    return 2;
  }

  StopWatch sw;
  sw.start();

  makebdd(argv[1]);

  sw.stop();

  USTime time = sw.time();
  cout << time << endl;
}
