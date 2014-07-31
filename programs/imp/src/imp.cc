
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

#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnNode.h"
#include "YmNetworks/BdnBlifReader.h"
#include "YmNetworks/BdnIscas89Reader.h"

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

#include "YmLogic/ZddMgr.h"

#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"

#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS


void
imp(const string& filename,
    bool blif,
    bool iscas89,
    bool do_direct,
    bool do_const,
    bool do_contra,
    bool do_naimp,
    bool do_naimp2,
    bool do_sat,
    bool do_recur,
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

  timer.start();

  ImpMgr imp_mgr;

  // BDN の情報を ImpMgr にコピーする．
  imp_mgr.set(bdn_network);

  timer.reset();
  timer.start();
  StrImp strimp;
  ImpInfo direct_imp;

  if ( do_direct ) {
    strimp.learning(imp_mgr, direct_imp);
  }
  timer.stop();
  USTime direct_time = timer.time();

  timer.reset();
  timer.start();
  ConstImp constimp;
  if ( do_const ) {
    constimp.learning(imp_mgr);
  }
  timer.stop();
  USTime const_time = timer.time();

  timer.reset();
  timer.start();
  ContraImp contraimp;
  ImpInfo contra_imp;

  if ( do_contra ) {
    contraimp.learning(imp_mgr, contra_imp);
    //contra_imp.make_closure();
  }

  timer.stop();
  USTime contra_time = timer.time();

  timer.reset();
  timer.start();
  NaImp naimp;
  ImpInfo na_imp;

  if ( do_naimp ) {
    naimp.learning(imp_mgr, na_imp);
    //na_imp.make_closure();
  }

  timer.stop();
  USTime na_time = timer.time();

  timer.reset();
  timer.start();
  NaImp naimp2;
  ImpInfo na_imp2;
  naimp2.use_cap_merge2(false);

  if ( do_naimp2 ) {
    naimp2.learning(imp_mgr, na_imp2);
  }

  timer.stop();
  USTime na_time2 = timer.time();

  timer.reset();
  timer.start();
  RlImp rlimp;
  ImpInfo rl_imp;
  if ( level > 0 ) {
    rlimp.set_learning_level(level);
  }
  if ( do_recur ) {
    rlimp.learning(imp_mgr, rl_imp);
    //rl_imp.make_closure();
  }
  timer.stop();
  USTime rl_time = timer.time();

  timer.reset();
  timer.start();
  SatImp satimp;
  ImpInfo sat_imp;
  if ( do_sat ) {
    satimp.learning(imp_mgr, sat_imp);
  }
  timer.stop();
  USTime sat_time = timer.time();

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

  cout << "BDN:" << endl
       << "Total " << bdn_network.input_num() << " inputs" << endl
       << "Total " << node_num2 << " nodes"
       << " ( " << and_node << " ANDs + "
       << xor_node << " XORs )" << endl
       << "Total " << imp_mgr.node_num() - imp_mgr.input_num()
       << " ImpNodes" << endl
       << "Constant detection:              " << setw(10) << 0
       << ": " << const_time << endl
       << "Direct Implications:             " << setw(10) << direct_imp.imp_num(imp_mgr)
       << ": " << direct_time << endl
       << "Contraposition Implications:     " << setw(10) << contra_imp.imp_num(imp_mgr)
       << ": " << contra_time << endl
       << "Recursive Learning Implications: " << setw(10) << rl_imp.imp_num(imp_mgr)
       << ": " << rl_time << endl
       << "Naive Implications:              " << setw(10) << na_imp.imp_num(imp_mgr)
       << ": " << na_time << endl
       << "Naive Implications(2):           " << setw(10) << na_imp2.imp_num(imp_mgr)
       << ": " << na_time2 << endl
       << "Complete Implications:           " << setw(10) << sat_imp.imp_num(imp_mgr)
       << ": " << sat_time << endl;
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
  bool do_direct = false;
  bool do_const = false;
  bool do_contra = false;
  bool do_naimp = false;
  bool do_naimp2 = false;
  bool do_sat = false;
  bool do_recur = false;
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
    { "blif", '\0', POPT_ARG_NONE, NULL, 0x100,
      "blif mode", NULL },

    { "iscas89", '\0', POPT_ARG_NONE, NULL, 0x101,
      "iscas89 mode", NULL },

    { "direct", '\0', POPT_ARG_NONE, NULL, 0x130,
      "do direct implication", NULL},

    { "no-direct", '\0', POPT_ARG_NONE, NULL, 0x131,
      "without direct implication", NULL},

    { "contra", '\0', POPT_ARG_NONE, NULL, 0x140,
      "do contraposition learning", NULL},

    { "no-contra", '\0', POPT_ARG_NONE, NULL, 0x141,
      "without contraposition learning", NULL},

    { "naive", '\0', POPT_ARG_NONE, NULL, 0x150,
      "do naive-learning", NULL},

    { "no-naive", '\0', POPT_ARG_NONE, NULL, 0x151,
      "without naive-learning", NULL},

    { "naive2", '\0', POPT_ARG_NONE, NULL, 0x160,
      "do naive2-learning", NULL},

    { "no-naive2", '\0', POPT_ARG_NONE, NULL, 0x161,
      "without naive2-learning", NULL},

    { "sat", '\0', POPT_ARG_NONE, NULL, 0x200,
      "do sat-learning", NULL},

    { "no-sat", '\0', POPT_ARG_NONE, NULL, 0x201,
      "without sat-learning", NULL},

    { "recursive", '\0', POPT_ARG_NONE, NULL, 0x210,
      "do recursive-learning", NULL},

    { "no-recursive", '\0', POPT_ARG_NONE, NULL, 0x211,
      "without recursive-learning", NULL},

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
    else if ( rc == 0x130 ) {
      do_direct = true;
    }
    else if ( rc == 0x131 ) {
      do_direct = false;
    }
    else if ( rc == 0x140 ) {
      do_contra = true;
    }
    else if ( rc == 0x141 ) {
      do_contra = false;
    }
    else if ( rc == 0x150 ) {
      do_naimp = true;
    }
    else if ( rc == 0x151 ) {
      do_naimp = false;
    }
    else if ( rc == 0x160 ) {
      do_naimp2 = true;
    }
    else if ( rc == 0x161 ) {
      do_naimp2 = false;
    }
    else if ( rc == 0x200 ) {
      do_sat = true;
    }
    else if ( rc == 0x201 ) {
      do_sat = false;
    }
    else if ( rc == 0x210 ) {
      do_recur = true;
    }
    else if ( rc == 0x211 ) {
      do_recur = false;
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
  imp(filename, blif, iscas,
      do_direct, do_const, do_contra, do_naimp, do_naimp2, do_sat, do_recur, level);

  return 0;
}
