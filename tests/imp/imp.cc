
/// @file imp.cc
/// @brief 構造的含意の列挙プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
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

#include "StrImp.h"
#include "CnfImp.h"

#include "ym_logic/ZddMgr.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"

#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS


void
imp(const string& filename,
    bool blif,
    bool iscas89,
    const string& method_str)
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

  vector<ImpInfo> imp_list;
  if ( method_str == "structure" ) {
    StrImp imp;
    imp.learning(network, imp_list);
  }
  else if ( method_str == "cnf" ) {
    CnfImp imp;
    imp.learning(network, imp_list);
  }
  else {
    cerr << "Unknown method: " << method_str << endl;
  }

#if 0
  SimpleOp op;

  if ( method_str == "bottom_up" ) {
    BottomUp enumcut;

    enumcut(network, cut_size, &op);
  }
  else if ( method_str == "top_down" ) {
    TopDown enumcut;

    enumcut(network, cut_size, &op);
  }
  else if ( method_str == "zdd" ) {
    ZddMgr mgr("zddmgr");
    ZddImp enumcut(mgr);

    enumcut(network, cut_size);
  }
  else if ( method_str == "zdd2" ) {
    ZddMgr mgr("zddmgr");
    ZddImp2 enumcut(mgr);

    enumcut(network, cut_size);
  }
  else if ( method_str == "zdd_top_down" ) {
    ZddMgr mgr("zddmgr");
    ZddTopDown enumcut(mgr);

    enumcut(network, cut_size);
  }
  else {
    cerr << "Unknown method: " << method_str << endl;
    return;
  }
#endif
}

END_NAMESPACE_YM_NETWORKS


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm::nsNetworks;

  const char* method_str = "bottom_up";
  bool blif = false;
  bool iscas = false;

  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "method", 'm', POPT_ARG_STRING, &method_str, 0,
      "specify evaluation method", "bottom_up|top_down|zdd" },

    { "blif", '\0', POPT_ARG_NONE, NULL, 0x100,
      "blif mode", NULL },

    { "iscas89", '\0', POPT_ARG_NONE, NULL, 0x101,
      "iscas89 mode", NULL },

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

  if ( !blif && !iscas ) {
    blif = true;
  }

  // 残りの引数はファイル名とみなす．
  const char* str = poptGetArg(popt_context);
  if ( str == NULL ) {
    fprintf(stderr, "No filename.\n");
    return 2;
  }

  string filename(str);
  imp(filename, blif, iscas, method_str);

  return 0;
}
