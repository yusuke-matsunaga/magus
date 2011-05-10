
/// @file libym_cell/tests/dotlib_parser_test.cc
/// @brief DotlibParser のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dotlib_lex_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include <iomanip>

#include "ym_dotlib/DotlibMgr.h"
#include "ym_dotlib/DotlibParser.h"
#include "ym_dotlib/DotlibNode.h"
#include "ym_dotlib/DotlibLibrary.h"
#include "ym_dotlib/DotlibCell.h"
#include "ym_dotlib/DotlibPin.h"

#include "ym_utils/StopWatch.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"


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
      if ( !dt_library->attr_value()->get_library_info(library_info) ) {
	break;
      }
      cout << "library name = " << library_info.name() << endl;

      const list<const DotlibNode*>& cell_list = library_info.cell_list();
      for (list<const DotlibNode*>::const_iterator p = cell_list.begin();
	   p != cell_list.end(); ++ p) {
	const DotlibNode* dt_cell = *p;
	DotlibCell cell_info;
	if ( !dt_cell->get_cell_info(cell_info) ) {
	  continue;
	}
	cout << endl
	     << "  cell name = " << cell_info.name() << endl
	     << "       area = " << cell_info.area() << endl;
	const list<const DotlibNode*>& dt_pin_list = cell_info.pin_list();
	const list<const DotlibNode*>& dt_bus_list = cell_info.bus_list();
	const list<const DotlibNode*>& dt_bundle_list = cell_info.bundle_list();
	for (list<const DotlibNode*>::const_iterator q = dt_pin_list.begin();
	     q != dt_pin_list.end(); ++ q) {
	  const DotlibNode* dt_pin = *q;
	  DotlibPin pin_info;
	  if ( !dt_pin->get_pin_info(pin_info) ) {
	    continue;
	  }
	  cout << "    pin name = " << pin_info.name() << endl
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

  int stat = nsYm::nsDotlib::dotlibparser_test(argc, argv);

  return stat;
}
