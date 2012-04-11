
/// @file imp.cc
/// @brief 構造的含意の列挙プログラム
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
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnBlifReader.h"
#include "ym_networks/BdnIscas89Reader.h"

#include "ImpMgr.h"
#include "ImpInfo.h"

#include "StrImp.h"
#include "ContraImp.h"
#include "ConstImp.h"
#include "CnfImp.h"
#include "CnfImp2.h"
#include "SatImp.h"
#include "RlImp.h"
#include "NaImp.h"
#include "NaImp2.h"

#include "ym_logic/ZddMgr.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"

#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS


void
imp(const string& filename,
    bool blif,
    bool iscas89,
    const string& method_str,
    ymuint level)
{
  MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(msg_handler);

  BdnMgr bdn_network;

  if ( blif ) {
    BdnBlifReader bdn_read;
    if ( !bdn_read(filename, bdn_network) ) {
      cerr << "Error in reading " << filename << endl;
      return;
    }
  }
  else {
    BdnIscas89Reader bdn_read;
    if ( !bdn_read(filename, bdn_network) ) {
      cerr << "Error in reading " << filename << endl;
      return;
    }
  }

  StopWatch timer;

  if ( method_str == "cnf" ) {
    timer.reset();
    timer.start();

    ImpMgr imp_mgr;

#if 0
    // BDN の情報を ImpMgr にコピーする．
    imp_mgr.set(network);
    CnfImp2 cnfimp;
    ImpInfo cnf_imp;
    cnfimp.learning(imp_mgr, cnf_imp);
    timer.stop();
    USTime cnf_time = timer.time();
    cout << "CNF Implications:             " << setw(10) << cnf_imp.size()
	 << ": " << cnf_time << endl;
#endif
  }
  else {
    timer.start();

    ImpMgr imp_mgr;

    // BDN の情報を ImpMgr にコピーする．
    imp_mgr.set(bdn_network);

    timer.reset();
    timer.start();
    ConstImp constimp;
#if 0
    constimp.learning(imp_mgr);
#endif
    timer.stop();
    USTime const_time = timer.time();

    timer.reset();
    timer.start();
    StrImp strimp;
    ImpInfo direct_imp;
    strimp.learning(imp_mgr, direct_imp);
    timer.stop();
    USTime direct_time = timer.time();

    timer.reset();
    timer.start();
    ContraImp contraimp;
    ImpInfo contra_imp;
    contraimp.learning(imp_mgr, direct_imp, contra_imp);
    timer.stop();
    USTime contra_time = timer.time();

    timer.reset();
    timer.start();
    SatImp satimp;
    ImpInfo sat_imp;
#if 1
    satimp.learning(imp_mgr, direct_imp, sat_imp);
#endif
    timer.stop();
    USTime sat_time = timer.time();

    timer.reset();
    timer.start();
    RlImp rlimp;
    ImpInfo rl_imp;
    if ( level > 0 ) {
      rlimp.set_learning_level(level);
    }
#if 1
    rlimp.learning(imp_mgr, direct_imp, rl_imp);
#endif
    timer.stop();
    USTime rl_time = timer.time();

    if ( 0 ) {
      ImpInfo diff_imp;
      ymuint diff = sat_imp.compare(rl_imp, diff_imp);
      if ( diff > 0 ) {
	cout << "Diff" << endl;
	diff_imp.print(cout);
      }
    }

    timer.reset();
    timer.start();
    NaImp naimp;
    ImpInfo na_imp;
#if 1
    naimp.learning(imp_mgr, direct_imp, na_imp);
#endif
    timer.stop();
    USTime na_time = timer.time();

#if 0
    timer.reset();
    timer.start();
    NaImp2 naimp2;
    ImpInfo na_imp2;
#if 1
    naimp2.learning(imp_mgr, direct_imp, na_imp2);
#endif
    timer.stop();
    USTime na_time2 = timer.time();
#endif

    ymuint and_node = 0;
    ymuint xor_node = 0;
    {
      const BdnNodeList& node_list = bdn_network.lnode_list();
      for (BdnNodeList::const_iterator p = node_list.begin();
	   p != node_list.end(); ++ p) {
	const BdnNode* node = *p;
	if ( node->is_and() ) {
	  ++ and_node;
	}
	else if ( node->is_xor() ) {
	  ++ xor_node;
	}
	else {
	  assert_not_reached(__FILE__, __LINE__);
	}
      }
    }
    ymuint node_num2 = bdn_network.lnode_num();
    ymuint dcimp = direct_imp.size() + contra_imp.size();
    cout << "BDN:" << endl
	 << "Total " << bdn_network.input_num() << " inputs" << endl
	 << "Total " << node_num2 << " nodes"
	 << " ( " << and_node << " ANDs + "
	 << xor_node << " XORs )" << endl
	 << "Total " << imp_mgr.node_num() - imp_mgr.input_num()
	 << " ImpNodes" << endl
	 << "Constant detection:              " << setw(10) << 0
	 << ": " << const_time << endl
	 << "Direct Implications:             " << setw(10) << direct_imp.size()
	 << ": " << direct_time << endl
	 << "Contraposition Implications:     " << setw(10) << contra_imp.size()
	 << ": " << contra_time << endl
	 << "                                 " << setw(10) << contra_imp.imp_num() << endl
	 << "D + C Implications:              " << setw(10) << dcimp << endl
	 << "Recursive Learning Implications: " << setw(10) << rl_imp.size()
	 << ": " << rl_time << endl
	 << "Naive Implications:              " << setw(10) << na_imp.size()
	 << ": " << na_time << endl
	 << ":                                " << setw(10) << na_imp.imp_num() << endl
#if 0
	 << "Naive Implications(2):           " << setw(10) << na_imp2.size()
	 << ": " << na_time2 << endl
#endif
	 << "Complete Implications:           " << setw(10) << sat_imp.size()
	 << ": " << sat_time << endl;
  }
#if 0
  else if ( method_str == "cnf" ) {
    CnfImp imp;
    imp.learning(network, imp_info);
  }
  else if ( method_str == "cnf2" ) {
    CnfImp2 imp;
    imp.learning(network, imp_info);
  }
  else if ( method_str == "sat" ) {
    SatImp imp;
    imp.learning(network, imp_info);
  }
  else if ( method_str == "rl" ) {
    RlImp imp;
    if ( level > 0 ) {
      imp.set_learning_level(level);
    }
    imp.learning(network, imp_info);
  }
  else {
    cerr << "Unknown method: " << method_str << endl;
  }
#if 0
  imp_info.print(cout);
#else
  cout << "Total " << imp_info.size() << " implications" << endl;
#endif
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
  ymuint level = 0;

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

    { "level", 'l', POPT_ARG_INT, &level, 0,
      "specify recursive learning level", "<level>" },

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
  imp(filename, blif, iscas, method_str, level);

  return 0;
}
