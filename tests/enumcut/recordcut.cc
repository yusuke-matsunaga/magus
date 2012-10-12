
/// @file recordcut.cc
/// @brief カットを記録するプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_POPT
#include <popt.h>
#else
#error "<popt.h> not found."
#endif

#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnBlifReader.h"
#include "ym_networks/BdnIscas89Reader.h"
#include "ym_networks/BdnNode.h"

#include "TopDown.h"
#include "FuncMgr.h"
#include "FuncRec.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"

#include "ym_utils/StopWatch.h"

#include "ym_utils/FileBinI.h"
#include "ym_utils/FileBinO.h"


BEGIN_NAMESPACE_YM_NETWORKS

void
rec_func(FuncMgr& func_mgr,
	 const string& filename,
	 bool blif,
	 bool iscas89,
	 ymuint min_cut_size,
	 ymuint max_cut_size)
{
  MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(msg_handler);

  BdnMgr network;

  if ( blif ) {
    BdnBlifReader read;
    if ( !read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return;
    }
  }
  else {
    BdnIscas89Reader read;
    if ( !read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return;
    }
  }

  FuncRec op(func_mgr);
  TopDown enumcut;

  op.set_min_size(min_cut_size);
  op.set_debug_level(1);

  enumcut(network, max_cut_size, &op);

  vector<TvFunc> func_list;
  func_mgr.func_list(func_list);

  cout << "Total " << setw(12) << func_list.size() << " functions" << endl;
  for (ymuint i = min_cut_size; i <= max_cut_size; ++ i) {
    vector<TvFunc> func_list;
    func_mgr.func_list(i, func_list);
    cout << "Total " << setw(12) << func_list.size() << " " << setw(2) << i << " input functions" << endl;
  }

#if 0
  const list<Cut*>& cut_list = cut_mgr.cut_list();
  cout << "Total " << cut_list.size() << " unique cuts" << endl;

  const char* datafile = "cuts.dump";
  {
    ofstream os;
    os.open(datafile, ios::binary);
    if ( !os ) {
      cerr << "Could not create " << datafile << endl;
      return;
    }
    BinOStream bos(os);

    cut_mgr.dump(bos);
  }

  CutMgr cut_mgr2;
  {
    ifstream is;
    is.open(datafile, ios::binary);
    if ( !is ) {
      cerr << "Could not open " << datafile << endl;
      return;
    }
    BinIStream bis(is);
    cut_mgr2.restore(bis);
  }
  cout << "cut_mgr2:: total " << cut_mgr2.cut_list().size() << " unique cuts" << endl;
#endif
}

void
dump_func(FuncMgr& func_mgr,
	  const char* filename)
{
  FileBinO bo(filename);
  if ( !bo ) {
    cerr << "Could not create " << filename << endl;
    return;
  }

  func_mgr.dump(bo);
}

void
restore_func(FuncMgr& func_mgr,
	     const char* filename)
{
  FileBinI bi(filename);
  if ( !bi ) {
    cerr << "Could not create " << filename << endl;
    return;
  }

  func_mgr.restore(bi);
}

END_NAMESPACE_YM_NETWORKS


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm::nsNetworks;

  bool blif = false;
  bool iscas = false;
  int max_cut_size = -1;
  int min_cut_size = -1;

  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "blif", '\0', POPT_ARG_NONE, NULL, 0x100,
      "blif mode", NULL },

    { "iscas89", '\0', POPT_ARG_NONE, NULL, 0x101,
      "iscas89 mode", NULL },

    { "max_cut_size", 'C', POPT_ARG_INT, &max_cut_size, 0,
      "specify maximum cut size", NULL },

    { "min_cut_size", 'c', POPT_ARG_INT, &min_cut_size, 0,
      "specify minimum cut size", NULL },

    POPT_AUTOHELP

    { NULL, '\0', 0, NULL, 0, NULL, NULL }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, "[OPTIONS]* <file-name> ...");

  // オプション解析行う．
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
    if ( rc == 0x100 ) {
      blif = true;
    }
    else if ( rc == 0x101 ) {
      iscas = true;
    }
  }

  if ( max_cut_size == -1 ) {
    if ( min_cut_size == -1 ) {
      max_cut_size = 4;
      min_cut_size = 4;
      cerr << "Both min_cut_size and max_cut_size are not specified. '4' is assumed" << endl;
    }
    else {
      max_cut_size = min_cut_size;
    }
  }
  else {
    if ( min_cut_size == -1 ) {
      min_cut_size = max_cut_size;
    }
  }

  if ( !blif && !iscas ) {
    blif = true;
  }

  // 残りの引数はファイル名とみなす．
  const char* str = poptGetArg(popt_context);
  if ( str == NULL ) {
    fprintf(stderr, "No filename.\n");
    return 2;
  }

  FuncMgr func_mgr;

  string filename(str);
  rec_func(func_mgr, filename, blif, iscas, min_cut_size, max_cut_size);

  return 0;
}
