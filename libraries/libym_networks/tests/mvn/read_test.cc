
/// @file read_test.cc
/// @brief MvnNode を生成するテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_POPT
#include <popt.h>
#else
#error "No <popt.h>"
#endif


#include "ym_networks/MvnMgr.h"
#include "ym_networks/MvnVerilogReader.h"
#include "ym_networks/MvnVlMap.h"
#include "ym_networks/MvnDumper.h"
#include "ym_networks/MvnVerilogWriter.h"

#include "ym_cell/CellDotlibReader.h"
#include "ym_cell/CellMislibReader.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;

  char* liberty_name = NULL;
  char* mislib_name = NULL;

  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "liberty", '\0', POPT_ARG_STRING, &liberty_name, 0,
      "specify liberty library", "\"file name\"" },

    { "mislib", '\0', POPT_ARG_STRING, &mislib_name, 0,
      "specify mislib library", "\"file name\"" },

    { "dump", 'd', POPT_ARG_NONE, NULL, 1,
      "dump network", NULL},

    { "verilog", 'V', POPT_ARG_NONE, NULL, 2,
      "dump verilog", NULL},

    POPT_AUTOHELP

    { NULL, '\0', 0, NULL, 0, NULL, NULL }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, "[OPTIONS]* <file-name> ...");

  // オプション解析を行う．
  ymuint mode = 1;
  for ( ; ; ) {
    int rc = poptGetNextOpt(popt_context);
    if ( rc == -1 ) {
      break;
    }
    if ( rc < -1 ) {
      // エラーが起きた．
      fprintf(stderr, "%s: %s\n",
	      poptBadOption(popt_context, POPT_BADOPTION_NOALIAS),
	      poptStrerror(rc));
      return 1;
    }
    mode = rc;
  }

  // 残りの引数はすべてファイル名と見なす
  list<string> filename_list;
  for ( ; ; ) {
    const char* str = poptGetArg(popt_context);
    if ( str == NULL ) break;
    filename_list.push_back(str);
  }

  const CellLibrary* cell_library = NULL;
  if ( liberty_name != NULL ) {
    CellDotlibReader read;
    cell_library = read(liberty_name);
  }
  else if ( mislib_name != NULL ) {
    CellMislibReader read;
    cell_library = read(mislib_name);
  }

#if !defined(YM_DEBUG)
  try {
#endif
    MsgHandler* mh = new StreamMsgHandler(&cerr);
    mh->set_mask(kMaskAll);
    mh->delete_mask(kMsgInfo);
    mh->delete_mask(kMsgDebug);
    MsgMgr::reg_handler(mh);

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
    MvnMgr mgr;
    MvnVlMap node_map;
    bool stat = reader.gen_network(mgr, cell_library, node_map);
    cerr << " End" << endl;
    if ( !stat ) {
      cerr << "error occured" << endl;
      return 2;
    }

    if ( mode == 1 ) {
      MvnDumper dump;
      dump(cout, mgr);
    }
    else if ( mode == 2 ) {
      MvnVerilogWriter vl_writer;

      vl_writer(cout, mgr, node_map);
    }

#if !defined(YM_DEBUG)
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif
  return 0;
}
