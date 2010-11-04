
/// @file libym_techmap/test/patmgr_test.cc
/// @brief PatMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen/patgen_nsdef.h"
#include "patgen/PgFuncMgr.h"
#include "ym_lexp/LogExpr.h"
#include "PatMgr.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellMislibReader.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsCell;
  using namespace nsYm::nsTechmap::nsPatgen;

  const char* filename = argv[1];
  CellMislibReader reader;
  const CellLibrary* library = reader.read(filename);
  if ( library == NULL ) {
    return 1;
  }

  const char* datafile = "patdata.bin";
  ofstream os;
  os.open(datafile, ios::binary);
  if ( !os ) {
    // エラー
    cerr << "Could not create " << datafile << endl;
    return 2;
  }
  dump_library(os, *library);

  PgFuncMgr pgf_mgr;
  vector<list<ymuint> > id_list_array;
  ymuint nc = library->cell_num();
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library->cell(i);
    ymuint np = cell->pin_num();
    const CellPin* opin = NULL;
    for (ymuint j = 0; j < np; ++ j) {
      const CellPin* pin = cell->pin(j);
      if ( pin->direction() == kDirOutput ) {
	if ( opin != NULL ) {
	  opin = NULL;
	  break;
	}
	opin = pin;
      }
    }
    if ( opin == NULL ) continue;
    if ( !opin->has_function() ) continue;
    LogExpr func = opin->function();
    ymuint fid = pgf_mgr.reg_expr(func);
    while ( id_list_array.size() <= fid ) {
      id_list_array.push_back(list<ymuint>());
    }
    list<ymuint>& id_list = id_list_array[fid];
    id_list.push_back(i);
  }

  pg_dump(ofs, pgf_mgr);

  PatMgr pat_mgr;
  {
    ifstream ifs;
    ifs.open(datafile, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << filename << endl;
      return;
    }

    pat_mgr.load(ifs);
  }

  dump(cout, pat_mgr);
}
