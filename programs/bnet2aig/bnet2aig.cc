
/// @file bnet2aig.cc
/// @brief bnet から aig を作るテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BNetwork.h"
#include "YmNetworks/BNetBlifReader.h"
#include "YmLogic/AigMgr.h"
#include "YmLogic/Aig.h"

//#include "aig/AigSatMgr.h"
//#include "sat/SatSolver.h"

#include "YmUtils/HashMap.h"
#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"


BEGIN_NAMESPACE_YM

// BNetwork から AIG をつくるコンストラクタ
void
bnet2aig(const BNetwork& network,
	 AigMgr& aig_mgr)
{
  // BNetwork 中のノードと AIG 中のノードの対応を持つ配列
  vector<Aig> aig_map(network.max_node_id());

  // 外部入力を作る．
  ymuint ipos = 0;
  for (BNodeList::const_iterator p = network.inputs_begin();
       p != network.inputs_end(); ++p, ++ ipos) {
    BNode* bnode = *p;
    Aig anode = aig_mgr.make_input(VarId(ipos));
    aig_map[bnode->id()] = anode;
  }

  // 内部ノードを作る．
  // まず入力からのトポロジカル順にソートし buff に入れる．
  BNodeVector node_list;
  network.tsort(node_list);
  ymuint nv = network.logic_node_num();
  for (ymuint i = 0; i < nv; ++ i) {
    BNode* bnode = node_list[i];
    ymuint ni = bnode->fanin_num();
    HashMap<VarId, Aig> input_map;
    for (ymuint pos = 0; pos < ni; ++ pos) {
      Aig iaig = aig_map[bnode->fanin(pos)->id()];
      input_map.add(VarId(pos), iaig);
      cout << "input_map[" << pos << "] = " << iaig << endl;
    }
    cout << "bnode->func() = " << bnode->func() << endl;
    Aig anode = aig_mgr.make_logic(bnode->func(), input_map);
    cout << "anode = " << anode << endl;
    aig_map[bnode->id()] = anode;
  }

  // 外部出力を作る．
  list<Aig> output_handle_list;
  const BNodeList& output_list = network.outputs();
  for (BNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    BNode* obnode = *p;
    BNode* ibnode = obnode->fanin(0);
    Aig ianode = aig_map[ibnode->id()];
    output_handle_list.push_back(ianode);
  }

  aig_mgr.print_handles(cout, output_handle_list);

#if 0
  SatSolver* solver = SatSolverFactory::gen_solver();
  AigSatMgr aigsat(aig_mgr, *solver);
  Aig root = output_handle_list.front();
  vector<Bool3> model;
  Bool3 stat = aigsat.sat(root, model);
#endif
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

  string filename = argv[1];

  try {
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    MsgMgr::reg_handler(msg_handler);

    BNetwork network;
    BNetBlifReader read;
    if ( !read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    AigMgr aig_mgr;
    bnet2aig(network, aig_mgr);
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
