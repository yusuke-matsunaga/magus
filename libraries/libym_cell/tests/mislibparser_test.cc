
/// @file libym_cell/tests/mislib_test.cc
/// @brief MisLib のテスト用プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: mislib_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "mislib/MislibParser.h"
#include "ym_utils/MsgHandler.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"
#include "ym_lexp/LogExpr.h"


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsCell;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 255;
  }

  MislibParser parser;

  MsgHandler* mh = new StreamMsgHandler(&cerr);
  mh->set_mask(MsgHandler::kMaskAll);
  mh->delete_mask(kMsgInfo);
  mh->delete_mask(kMsgDebug);
  parser.msg_mgr().reg_handler(mh);

  const CellLibrary* library = parser.read(argv[1]);
  if ( library == NULL) {
    return 1;
  }

  ymuint n = library->cell_num();
  for (ymuint i = 0; i < n; ++ i) {
    const Cell* cell = library->cell(i);
    cout << "Cell (" << cell->name() << ")" << endl;
    cout << "  area = " << cell->area() << endl;
    ymuint np = cell->pin_num();
    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      switch ( pin->direction() ) {
      case kDirInput:
	cout << "  Input Pin (" << pin->name() << ")" << endl;
	cout << "    Capacitance = " << pin->capacitance() << endl
	     << "    Rise Capacitance = " << pin->rise_capacitance() << endl
	     << "    Fall Capacitance = " << pin->fall_capacitance() << endl;
	break;

      case kDirOutput:
	cout << "  Output Pin(" << pin->name() << ")" << endl;
	cout << "    Function = " << pin->function() << endl
	     << "    Max Fanout = " << pin->max_fanout() << endl
	     << "    Min Fanout = " << pin->min_fanout() << endl
	     << "    Max Capacitance = " << pin->max_capacitance() << endl
	     << "    Min Capacitance = " << pin->min_capacitance() << endl
	     << "    Max Transition = " << pin->max_transition() << endl
	     << "    Min Transition = " << pin->min_transition() << endl;
	for (ymuint k = 0; k < np; ++ k) {
	  const CellTiming* timing = pin->timing(k, kSensePosiUnate);
	  if ( timing ) {
	    cout << "    Timing ( PosiUnate )" << endl
		 << "      Related Pin = " << cell->pin(k)->name() << endl
		 << "      Rise Intrinsic = " << timing->intrinsic_rise() << endl
		 << "      Rise Resistance = " << timing->rise_resistance() << endl
		 << "      Fall Intrinsic = " << timing->intrinsic_fall() << endl
		 << "      Fall Resistance = " << timing->fall_resistance() << endl;
	  }
	  timing = pin->timing(k, kSenseNegaUnate);
	  if ( timing ) {
	    cout << "    Timing ( NegaUnate )" << endl
		 << "      Related Pin = " << cell->pin(k)->name() << endl
		 << "      Rise Intrinsic = " << timing->intrinsic_rise() << endl
		 << "      Rise Resistance = " << timing->rise_resistance() << endl
		 << "      Fall Intrinsic = " << timing->intrinsic_fall() << endl
		 << "      Fall Resistance = " << timing->fall_resistance() << endl;
	  }
	}
	break;
      }
    }
    cout << endl;
  }

  return 0;
}
