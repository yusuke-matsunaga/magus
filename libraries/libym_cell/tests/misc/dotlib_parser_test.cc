
/// @file dotlib_parser_test.cc
/// @brief DotlibParser のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include <iomanip>

#include "dotlib/DotlibMgr.h"
#include "dotlib/DotlibParser.h"
#include "dotlib/DotlibNode.h"
#include "dotlib/DotlibLibrary.h"
#include "dotlib/DotlibCell.h"
#include "dotlib/DotlibFF.h"
#include "dotlib/DotlibLatch.h"
#include "dotlib/DotlibPin.h"

#include "utils/StopWatch.h"

#include "utils/MsgMgr.h"
#include "utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

int
dotlibparser_test(int argc,
		  char** argv)
{
  StreamMsgHandler handler(&cerr);
  MsgMgr::reg_handler(&handler);

  DotlibMgr mgr;

  DotlibParser parser;

  StopWatch timer;
  timer.start();

  bool error = false;
  bool debug = false;
  bool loop = false;
  bool dump = false;
  bool parse = false;
  for (int i = 1; i < argc; ++ i) {
    if ( strcmp(argv[i], "-d") == 0 ) {
      debug = true;
    }
    else if ( strcmp(argv[i], "-l") == 0 ) {
      loop = true;
    }
    else if ( strcmp(argv[i], "-v") == 0 ) {
      dump = true;
    }
    else if ( strcmp(argv[i], "-p") == 0 ) {
      parse = true;
    }
    else {
      bool stat = parser.read_file(argv[i], mgr, debug);
      if ( !stat ) {
	error = true;
      }
      break;
    }
  }

  if ( dump ) {
    const DotlibNode* library = mgr.root_node();
    library->dump(cout);
  }
  if ( parse ) {
    for ( ; ; ) {
      // break を使いたいだけの擬似ループ
      const DotlibNode* dt_library = mgr.root_node();
      DotlibLibrary library_info;
      if ( !library_info.set_data(dt_library) ) {
	break;
      }
      cout << "library name = " << library_info.name() << endl;

      const list<const DotlibNode*>& cell_list = library_info.cell_list();
      for (list<const DotlibNode*>::const_iterator p = cell_list.begin();
	   p != cell_list.end(); ++ p) {
	const DotlibNode* dt_cell = *p;
	DotlibCell cell_info;
	if ( !cell_info.set_data(dt_cell) ) {
	  continue;
	}
	cout << endl
	     << "  cell name = " << cell_info.name() << endl
	     << "       area = " << cell_info.area() << endl;
	const DotlibNode* dt_ff = cell_info.ff();
	if ( dt_ff ) {
	  DotlibFF ff_info;
	  if ( !ff_info.set_data(dt_ff) ) {
	    continue;
	  }
	  cout << "       ff (" << ff_info.var1_name()
	       << ", " << ff_info.var2_name() << ")" << endl;
	  const DotlibNode* next_state = ff_info.next_state();
	  if ( next_state ) {
	    cout << "          next_state = ";
	    next_state->dump(cout);
	    cout << endl;
	  }
	  const DotlibNode* clocked_on = ff_info.clocked_on();
	  if ( clocked_on ) {
	    cout << "          clocked_on = ";
	    clocked_on->dump(cout);
	    cout << endl;
	  }
	  const DotlibNode* clocked_on_also = ff_info.clocked_on_also();
	  if ( clocked_on_also ) {
	    cout << "          clocked_on_also = ";
	    clocked_on_also->dump(cout);
	    cout << endl;
	  }
	  const DotlibNode* clear = ff_info.clear();
	  if ( clear ) {
	    cout << "          clear = ";
	    clear->dump(cout);
	    cout << endl;
	  }
	  const DotlibNode* preset = ff_info.preset();
	  if ( preset ) {
	    cout << "          preset = ";
	    preset->dump(cout);
	    cout << endl;
	  }
	  cout << "          clear_preset_var1 = "
	       << ff_info.clear_preset_var1() << endl;
	  cout << "          clear_preset_var2 = "
	       << ff_info.clear_preset_var2() << endl;
	}
	const DotlibNode* dt_latch = cell_info.latch();
	if ( dt_latch ) {
	  DotlibLatch latch_info;
	  if ( !latch_info.set_data(dt_latch) ) {
	    continue;
	  }
	  cout << "       latch (" << latch_info.var1_name()
	       << ", " << latch_info.var2_name() << ")" << endl;
	  const DotlibNode* data_in = latch_info.data_in();
	  if ( data_in ) {
	    cout << "          data_in = ";
	    data_in->dump(cout);
	    cout << endl;
	  }
	  const DotlibNode* enable = latch_info.enable();
	  if ( enable ) {
	    cout << "          enable = ";
	    enable->dump(cout);
	    cout << endl;
	  }
	  const DotlibNode* enable_also = latch_info.enable_also();
	  if ( enable_also ) {
	    cout << "          enable_also = ";
	    enable_also->dump(cout);
	    cout << endl;
	  }
	  const DotlibNode* clear = latch_info.clear();
	  if ( clear ) {
	    cout << "          clear = ";
	    clear->dump(cout);
	    cout << endl;
	  }
	  const DotlibNode* preset = latch_info.preset();
	  if ( preset ) {
	    cout << "          preset = ";
	    preset->dump(cout);
	    cout << endl;
	  }
	  cout << "          clear_preset_var1 = "
	       << latch_info.clear_preset_var1() << endl;
	  cout << "          clear_preset_var2 = "
	       << latch_info.clear_preset_var2() << endl;
	}
	const list<const DotlibNode*>& dt_pin_list = cell_info.pin_list();
	const list<const DotlibNode*>& dt_bus_list = cell_info.bus_list();
	const list<const DotlibNode*>& dt_bundle_list = cell_info.bundle_list();
	for (list<const DotlibNode*>::const_iterator q = dt_pin_list.begin();
	     q != dt_pin_list.end(); ++ q) {
	  const DotlibNode* dt_pin = *q;
	  DotlibPin pin_info;
	  if ( !pin_info.set_data(dt_pin) ) {
	    continue;
	  }
	  cout << "    pin name = ";
	  ymuint np = pin_info.num();
	  for (ymuint i = 0; i < np; ++ i) {
	    cout << " " << pin_info.name(i);
	  }
	  cout << endl
	       << "        direction = ";
	  switch ( pin_info.direction() ) {
	  case DotlibPin::kInput: cout << "input"; break;
	  case DotlibPin::kOutput: cout << "output"; break;
	  case DotlibPin::kInout: cout << "inout"; break;
	  case DotlibPin::kInternal: cout << "internal"; break;
	  }
	  cout << endl;
	  if ( pin_info.direction() == DotlibPin::kInput ||
	       pin_info.direction() == DotlibPin::kInout ) {
	    cout << "        capacitance = "
		 << pin_info.capacitance() << endl
		 << "        rise capacitance = "
		 << pin_info.rise_capacitance() << endl
		 << "        fall capacitance = "
		 << pin_info.fall_capacitance() << endl;
	  }
	  if ( pin_info.direction() == DotlibPin::kOutput ||
	       pin_info.direction() == DotlibPin::kInout ) {
	    cout << "        max_fanout = "
		 << pin_info.max_fanout() << endl
		 << "        min_fanout = "
		 << pin_info.min_fanout() << endl
		 << "        max_capacitance = "
		 << pin_info.max_capacitance() << endl
		 << "        min_capacitance = "
		 << pin_info.min_capacitance() << endl
		 << "        max_transition = "
		 << pin_info.max_transition() << endl
		 << "        min_transition = "
		 << pin_info.min_transition() << endl;
	  }
	  if ( pin_info.direction() == DotlibPin::kOutput ) {
	    const DotlibNode* func_node = pin_info.function();
	    if ( func_node ) {
	      cout << "        function = ";
	      func_node->dump(cout);
	      cout << endl;
	    }
	    const DotlibNode* three_state = pin_info.three_state();
	    if ( three_state ) {
	      cout << "        three_state = ";
	      three_state->dump(cout);
	      cout << endl;
	    }
	  }
	}
	for (list<const DotlibNode*>::const_iterator q = dt_bus_list.begin();
	     q != dt_bus_list.end(); ++ q) {
	  const DotlibNode* dt_bus = *q;
	  dt_bus->dump(cout);
	}
	for (list<const DotlibNode*>::const_iterator q = dt_bundle_list.begin();
	     q != dt_bundle_list.end(); ++ q) {
	  const DotlibNode* dt_bundle = *q;
	  dt_bundle->dump(cout);
	}
      }
    }
  }

  timer.stop();
  USTime time = timer.time();
  cout << "Time: " << time << endl;

  mgr.show_stats(cout);

  if ( loop ) {
    for ( ; ; ) { }
  }

  return error ? 0 : -1;
}

END_NAMESPACE_YM_DOTLIB

int
main(int argc,
     char** argv)
{
  using namespace nsYm;

  if ( argc < 2 ) {
    cerr << "USAGE: " << argv[0]
	 << " <file-name> ..." << endl;
    return -1;
  }

  int stat = nsYm::nsCell::nsDotlib::dotlibparser_test(argc, argv);

  return stat;
}
