
/// @file vltest_parse.cc
/// @brief parse モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/StopWatch.h"
#include "VlTestLineWatcher.h"
#include "YmVerilog/VlMgr.h"
#include "PtDumper.h"

#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"


BEGIN_NAMESPACE_YM_VERILOG

void
parse_mode(const vector<string>& filename_list,
	   const char* spath,
	   int watch_line,
	   bool verbose,
	   bool profile,
	   int loop,
	   bool dump_pt)
{
  MsgHandler* tmh = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(tmh);

  SearchPathList splist;
  if ( spath ) {
    splist.set(spath);
  }

  VlTestLineWatcher watcher(watch_line);
  list<VlLineWatcher*> watcher_list;
  if ( watch_line ) {
    watcher_list.push_back(&watcher);
  }

  ymuint c = loop + 1;
  for (ymuint i = 0; i < c; ++ i) {
    try {
      StopWatch timer;
      timer.start();
      VlMgr vlmgr;
      for (vector<string>::const_iterator p = filename_list.begin();
	   p != filename_list.end(); ++ p) {
	const string& name = *p;
	if ( verbose ) {
	  cerr << "Reading " << name;
	  cerr.flush();
	}
	bool stat = vlmgr.read_file(name, splist, watcher_list);
	if ( verbose ) {
	  cerr << " end" << endl;
	}
      }
      timer.stop();
      USTime time = timer.time();
      if ( verbose ) {
	cerr << "Parsing time: " << time << endl;
      }

      if ( dump_pt ) {
	const list<const PtUdp*>& udp_list = vlmgr.pt_udp_list();
	const list<const PtModule*>& module_list = vlmgr.pt_module_list();
	PtDumper dp(cout);
	dp.enable_file_loc_mode();
	dp.put(udp_list, module_list);
      }

      switch ( MsgMgr::error_num() ) {
      case 0:
	cerr << "No errors" << endl;
	break;
      case 1:
	cerr << "Total 1 error" << endl;
	break;

      default:
	cerr << "Total " << MsgMgr::error_num() << " errors" << endl;
	break;
      }

      if ( profile ) {
	//ptfactory->dump_profile(cerr);
	cerr << vlmgr.allocated_size() / (1024 * 1024)
	     << "M bytes" << endl;
	sleep(10);
      }
    }
    catch ( AssertError x ) {
      cerr << x << endl;
    }
    catch ( std::bad_alloc x ) {
      cerr << "no more memory" << endl;
      exit(1);
    }
    catch (...) {
      cerr << "unkown exception" << endl;
    }
  }
}

END_NAMESPACE_YM_VERILOG
