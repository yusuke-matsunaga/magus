
/// @file mvnconv_test.cc
/// @brief mvnconv_test のソースファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnDumper.h"
#include "YmNetworks/BdnVerilogWriter.h"
#include "YmNetworks/MvnMgr.h"
#include "YmNetworks/MvnVerilogReader.h"
#include "YmNetworks/MvnVlMap.h"
#include "YmNetworks/MvnDumper.h"
#include "YmNetworks/MvnBdnConv.h"
#include "YmNetworks/MvnBdnMap.h"

#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"


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
    MsgHandler* mh = new StreamMsgHandler(&cerr);
    mh->set_mask(kMaskAll);
    mh->delete_mask(kMsgInfo);
    mh->delete_mask(kMsgDebug);
    MsgMgr::reg_handler(mh);

    MvnMgr mgr;
    MvnVerilogReader reader;

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

    MvnDumper dump_mvn;
    dump_mvn(cout, mgr);

    BdnMgr bdnetwork;
    MvnBdnConv conv;
    MvnBdnMap mvnode_map(mgr.max_node_id());
    conv(mgr, bdnetwork, mvnode_map);

    BdnDumper dump_bdn;
    dump_bdn(cout, bdnetwork);

    BdnVerilogWriter verilog_writer;
    verilog_writer(cout, bdnetwork);

#if !defined(YM_DEBUG)
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif
  return 0;
}
