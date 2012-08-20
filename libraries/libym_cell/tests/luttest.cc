
/// @file libym_cell/tests/luttest.cc
/// @brief CellDotlibReader のテスト用プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"
#include "ym_cell/CellDotlibReader.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellTiming.h"
#include "ym_cell/CellLut.h"


//#define ERROR_EPSILON 1.0e-10
#define ERROR_EPSILON DBL_EPSILON * 1.0

BEGIN_NAMESPACE_YM_CELL

bool
test_lut1(const CellLut* lut)
{
  vector<ymuint32> pos_array(1);
  vector<double> val_array(1);
  bool error = false;
  bool disp_lut = false;

  ymuint n = lut->index_num(0);
  for (ymuint i = 0; i < n; ++ i) {
    pos_array[0] = i;
    double ref_val = lut->grid_value(pos_array);
    val_array[0] = lut->index(0, i);
    double val = lut->value(val_array);
    double delta = fabs(ref_val - val) / ref_val;
    if ( delta > ERROR_EPSILON ) {
      cout << "Error ref_val != val" << endl
	   << "  idx     = " << i << endl
	   << "  ref_val = " << ref_val << endl
	   << "  val     = " << val << endl
	   << "  delta   = " << delta << endl;
      if ( !disp_lut ) {
	disp_lut = true;
	cout << "index = ";
	for (ymuint i = 0; i < n; ++ i) {
	  cout << " " << lut->index(0, i);
	}
	cout << endl;
	cout << "value = ";
	for (ymuint i = 0; i < n; ++ i) {
	  vector<ymuint32> pos_array(1);
	  pos_array[0] = i;
	  cout << " " << lut->grid_value(pos_array);
	}
	cout << endl;
      }
      error = true;
    }
  }
  return error;
}

bool
test_lut2(const CellLut* lut)
{
  vector<ymuint32> pos_array(2);
  vector<double> val_array(2);
  bool error = false;
  bool disp_lut = false;

  ymuint n1 = lut->index_num(0);
  ymuint n2 = lut->index_num(1);
  for (ymuint i = 0; i < n1; ++ i) {
    pos_array[0] = i;
    val_array[0] = lut->index(0, i);
    for (ymuint j = 0; j < n2; ++ j) {
      pos_array[1] = j;
      val_array[1] = lut->index(1, j);
      double ref_val = lut->grid_value(pos_array);
      double val = lut->value(val_array);
      double delta = fabs(ref_val - val);
      if ( delta > ERROR_EPSILON ) {
	cout << "Error ref_val != val" << endl
	     << "  idx     = " << i << ", " << j << endl
	     << "  ref_val = " << ref_val << endl
	     << "  val     = " << val << endl
	     << "  delta   = " << delta << endl;
	if ( !disp_lut ) {
	  disp_lut = true;
	  cout << "index_1 = ";
	  for (ymuint i = 0; i < n1; ++ i) {
	    cout << " " << lut->index(0, i);
	  }
	  cout << endl;
	  cout << "index_2 = ";
	  for (ymuint i = 0; i < n2; ++ i) {
	    cout << " " << lut->index(1, i);
	  }
	  cout << endl;
	  cout << "value = ";
	  vector<ymuint32> pos_array(2);
	  for (ymuint i = 0; i < n1; ++ i) {
	    pos_array[0] = i;
	    for (ymuint j = 0; j < n2; ++ j) {
	      pos_array[1] = j;
	      cout << " " << lut->grid_value(pos_array);
	    }
	    cout << endl;
	  }
	  cout << endl;
	}
	cout << endl;
	error = true;
      }
    }
  }
  return error;
}

bool
test_lut3(const CellLut* lut)
{
  return true;
}

bool
test_lut(const CellLut* lut)
{
  ymuint d = lut->dimension();
  switch ( d ) {
  case 1: return test_lut1(lut);

  case 2: return test_lut2(lut);

  case 3: return test_lut3(lut);
  }
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

bool
test_timing(const Cell* cell,
	    ymuint ipos,
	    ymuint opos,
	    tCellTimingSense sense)
{
  bool status = false;
  ymuint n = cell->timing_num(ipos, opos, sense);
  for (ymuint i = 0; i < n; ++ i) {
    const CellTiming* timing = cell->timing(ipos, opos, sense, i);

    if ( timing->cell_rise() ) {
      if ( !test_lut(timing->cell_rise()) ) {
	status = false;
      }
    }

    if ( timing->cell_fall() ) {
      if ( !test_lut(timing->cell_fall()) ) {
	status = false;
      }
    }

    if ( timing->rise_transition() ) {
      if ( !test_lut(timing->rise_transition()) ) {
	status = false;
      }
    }

    if ( timing->fall_transition() ) {
      if ( !test_lut(timing->fall_transition()) ) {
	status = false;
      }
    }

    if ( timing->rise_propagation() ) {
      if ( !test_lut(timing->rise_propagation()) ) {
	status = false;
      }
    }

    if ( timing->fall_propagation() ) {
      if ( !test_lut(timing->fall_propagation()) ) {
	status = false;
      }
    }
  }

  return status;
}

int
dotlib_reader_test(const string& filename)
{
  CellDotlibReader read;

  MsgHandler* mh = new StreamMsgHandler(&cerr);
  mh->set_mask(kMaskAll);
  mh->delete_mask(kMsgInfo);
  mh->delete_mask(kMsgDebug);
  MsgMgr::reg_handler(mh);

  const CellLibrary* library = read(filename);
  if ( library == NULL) {
    return 1;
  }

  bool error = false;
  ymuint nc = library->cell_num();
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library->cell(i);

    ymuint ni2 = cell->input_num2();
    ymuint no2 = cell->output_num2();
    for (ymuint ipos = 0; ipos < ni2; ++ ipos) {
      for (ymuint opos = 0; opos < no2; ++ opos) {
	if ( !test_timing(cell, ipos, opos, kCellPosiUnate) ) {
	  error = true;
	}
	if ( !test_timing(cell, ipos, opos, kCellNegaUnate) ) {
	  error = true;
	}
      }
    }
  }

  if ( error ) {
    return 1;
  }

  return 0;
}

END_NAMESPACE_YM_CELL


int
main(int argc,
     const char** argv)
{
  using namespace std;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 255;
  }

  int stat = nsYm::nsCell::dotlib_reader_test(argv[1]);

  return stat;
}
