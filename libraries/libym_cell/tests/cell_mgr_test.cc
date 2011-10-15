
/// @file cell_mgr_test.cc
/// @brief CellMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellMislibReader.h"
#include "ym_cell/CellDotlibReader.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm::nsCell;

  if ( argc < 2 ) {
    cerr << "Usage: " << argv[0] << " [--liberty] <liberty-file>" << endl;
    return 1;
  }
  ymuint base = 1;
  bool dotlib = false;
  if ( argc == 3 && strcmp(argv[1], "--liberty") == 0 ) {
    dotlib = true;
    base = 2;
  }

  const char* filename = argv[base];

  const CellLibrary* library = NULL;
  if ( dotlib ) {
    CellDotlibReader dotlib_reader;
    library = dotlib_reader(filename);
  }
  else {
    CellMislibReader mislib_reader;
    library = mislib_reader(filename);
  }
  if ( library == NULL ) {
    cerr << filename << ": Error in reading library" << endl;
    return 1;
  }

  display_library(cout, *library);

  ymuint n = library->cell_num();
  for (ymuint i = 0; i < n; ++ i) {
    const Cell* cell = library->cell(i);
    cout << "Cell: " << cell->name() << endl;
    const Cell* cell2 = library->cell(cell->name());
    if ( cell != cell2 ) {
      cout << "Error" << endl;
    }

    ymuint ni = cell->input_num();
    for (ymuint j = 0; j < ni; ++ j) {
      const CellPin* pin = cell->input(j);
      cout << "  Input: " << pin->name() << endl;
      const CellPin* pin2 = cell->pin(pin->name());
      if ( pin != pin2 ) {
	cout << "  Error" << endl;
      }
    }

    ymuint no = cell->output_num();
    for (ymuint j = 0; j < no; ++ j) {
      const CellPin* pin = cell->output(j);
      cout << "  Output: " << pin->name() << endl;
      const CellPin* pin2 = cell->pin(pin->name());
      if ( pin != pin2 ) {
	cout << "  Error" << endl;
      }
    }

    ymuint nio = cell->inout_num();
    for (ymuint j = 0; j < nio; ++ j) {
      const CellPin* pin = cell->inout(j);
      cout << "  Inout: " << pin->name() << endl;
      const CellPin* pin2 = cell->pin(pin->name());
      if ( pin != pin2 ) {
	cout << "  Error" << endl;
      }
    }
  }

  return 0;
}
