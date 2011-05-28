
/// @file libym_networks/tests/conv2sbj_test.cc
/// @brief conv2sbj_test のソースファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnMgr.h"
#include "ym_networks/MvnVerilogReader.h"
#include "ym_networks/MvnVlMap.h"
#include "ym_networks/MvnNodeMap.h"
#include "ym_networks/Mvn2Sbj.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_sbj/SbjDumper.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  list<string> filename_list;
  for (ymint i = 1; i < argc; ++ i) {
    filename_list.push_back(argv[i]);
  }

#if !defined(YM_DEBUG)
  try {
#endif
    MvnMgr mgr;
    MvnVerilogReader reader;
    MsgHandler* mh = new StreamMsgHandler(&cerr);
    mh->set_mask(MsgHandler::kMaskAll);
    mh->delete_mask(kMsgInfo);
    mh->delete_mask(kMsgDebug);
    reader.add_msg_handler(mh);

    for (list<string>::const_iterator p = filename_list.begin();
	 p != filename_list.end(); ++ p) {
      const string& name = *p;
      cerr << "Reading " << name;
      cerr.flush();
      bool stat = reader.read(name);
      cerr << " end" << endl;
      if ( !stat ) {
	return 1;
      }
    }
    cerr << "Generating MvnNetwork" << endl;
    MvnVlMap node_map;
    bool stat = reader.gen_network(mgr, node_map);
    cerr << " End" << endl;
    if ( !stat ) {
      cerr << "error occured" << endl;
      return 2;
    }

    SbjGraph sbjgraph;
    Mvn2Sbj conv;
    MvnNodeMap mvnode_map(mgr.max_node_id());
    conv(mgr, sbjgraph, mvnode_map);

    SbjDumper dumper;

    dumper.dump_verilog(cout, sbjgraph);
#if !defined(YM_DEBUG)
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif
  return 0;
}
