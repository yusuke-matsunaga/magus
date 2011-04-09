
/// @file libym_bdn/tests/mvnconv_test.cc
/// @brief mvnconv_test のソースファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnDumper.h"
#include "ym_mvn/MvnMgr.h"
#include "ym_mvn/MvnVerilogReader.h"
#include "ym_mvn/MvnVlMap.h"
#include "ym_mvn/MvnDumper.h"
#include "ym_mvnbdnconv/MvnBdnConv.h"
#include "ym_mvnbdnconv/MvnBdnMap.h"


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

    MvnDumper dump_mvn;
    dump_mvn(cout, mgr);

    BdNetwork bdnetwork;
    MvnBdnConv conv;
    MvnBdnMap mvnode_map(mgr.max_node_id());
    conv(mgr, bdnetwork, mvnode_map);

    BdnDumper dump_bdn;
    dump_bdn(cout, bdnetwork);

#if !defined(YM_DEBUG)
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif
  return 0;
}
