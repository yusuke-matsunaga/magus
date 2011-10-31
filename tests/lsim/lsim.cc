
/// @file tests/lsim/lsim.cc
/// @brief 論理シミュレータ
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

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"

#include "ym_utils/RandGen.h"

#include "ym_utils/StopWatch.h"

#include "LsimNaive.h"
#include "LsimNaive2.h"
#include "LsimBdd.h"
#include "LsimBdd1.h"
#include "LsimBdd2.h"
#include "LsimBdd3.h"
#include "LsimBdd10.h"
#include "LsimLcc.h"
#include "LsimMpx.h"
#include "LsimMpx2.h"


BEGIN_NAMESPACE_YM

string
read_name(istream& s)
{
  string buf;
  while ( !s.eof() ) {
    char c = s.get();
    if ( !isspace(c) ) {
      buf += c;
      break;
    }
  }
  while ( !s.eof() ) {
    char c = s.get();
    if ( isspace(c) || c == EOF ) {
      break;
    }
    buf += c;
  }
  return buf;
}

bool
read_order(const char* filename,
	   hash_map<string, ymuint>& order_map)
{
  ifstream fs;

  fs.open(filename);
  if ( !fs ) {
    cerr << "Could not open " << filename << endl;
    return false;
  }

  ymuint pos = 0;
  while ( !fs.eof() ) {
    string name = read_name(fs);
#if 0
    cout << name << ": " << pos << endl;
#endif
    order_map.insert(make_pair(name, pos));
    ++ pos;
  }
  return true;
}

void
do_lsim(Lsim& lsim,
	ymuint nloop,
	BdnMgr& network,
	hash_map<string, ymuint>& order_map)
{
  StopWatch sw;
  sw.start();

  lsim.set_network(network, order_map);

  sw.stop();

  USTime time1 = sw.time();
  cout << "Initialize:       \t" << time1 << endl;

  RandGen rg;

  sw.reset();
  sw.start();

  ymuint ni = network.input_num();
  ymuint no = network.output_num();
  vector<ymuint64> iv(ni);
  vector<ymuint64> ov(no);
  for (ymuint i = 0; i < nloop; ++ i) {
    for (ymuint j = 0; j < ni; ++ j) {
      ymuint64 tmp = rg.int32();
      tmp <<= 32;
      tmp += rg.int32();
      iv[j] = tmp;
    }
    lsim.eval(iv, ov);
  }

  sw.stop();

  USTime time2 = sw.time();
  cout << "Evaluation[" << nloop << "]:\t" << time2 << endl;
}

void
lsim(const string& filename,
     bool blif,
     bool iscas89,
     int loop_count,
     const string& method_str,
     const char* order_file)
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

  hash_map<string, ymuint> order_map;
  if ( order_file ) {
    if ( !read_order(order_file, order_map) ) {
      cerr << "Error in reading " << order_file << endl;
      return;
    }
  }

  if ( method_str == "naive" ) {
    LsimNaive lsim;
    do_lsim(lsim, loop_count, network, order_map);
  }
  else if ( method_str == "naive2" ) {
    LsimNaive2 lsim;
    do_lsim(lsim, loop_count, network, order_map);
  }
  else if ( method_str == "bdd" ) {
    LsimBdd lsim;
    do_lsim(lsim, loop_count, network, order_map);
  }
  else if ( method_str == "bdd1" ) {
    LsimBdd1 lsim;
    do_lsim(lsim, loop_count, network, order_map);
  }
  else if ( method_str == "bdd2" ) {
    LsimBdd2 lsim;
    do_lsim(lsim, loop_count, network, order_map);
  }
  else if ( method_str == "bdd3" ) {
    LsimBdd3 lsim;
    do_lsim(lsim, loop_count, network, order_map);
  }
  else if ( method_str == "bdd10" ) {
    LsimBdd10 lsim;
    do_lsim(lsim, loop_count, network, order_map);
  }
  else if ( method_str == "lcc" ) {
    LsimLcc lsim;
    do_lsim(lsim, loop_count, network, order_map);
  }
  else if ( method_str == "tv" ) {
  }
  else if ( method_str == "mpx" ) {
    LsimMpx lsim;
    do_lsim(lsim, loop_count, network, order_map);
  }
  else if ( method_str == "mpx2" ) {
    LsimMpx2 lsim;
    do_lsim(lsim, loop_count, network, order_map);
  }
  else {
    cerr << "Unknown method: " << method_str << endl;
  }
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;

  const char* method_str = "naive";
  const char* order_file = NULL;
  int loop_count = 2000;
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
      "specify evaluation method", "naive|bdd|tv" },

    { "loop-num", 'n', POPT_ARG_INT, &loop_count, 0,
      "specify loop count", NULL },

    { "blif", '\0', POPT_ARG_NONE, NULL, 0x100,
      "blif mode", NULL },

    { "iscas89", '\0', POPT_ARG_NONE, NULL, 0x101,
      "iscas89 mode", NULL },

    { "order", 'o', POPT_ARG_STRING, &order_file, 0,
      "specify variable order file", NULL },

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
  lsim(filename, blif, iscas, loop_count, method_str, order_file);

  return 0;
}
