// $Id: npn_check3.cc 1594 2008-07-18 10:26:12Z matsunaga $

// NPN パッケージのテスト


#include "YmTools.h"

#if HAVE_POPT
#include <popt.h>
#endif

#include "YmUtils/RandGen.h"
#include "YmUtils/PermGen.h"
#include "YmLogic/TvFunc.h"
#include "YmLogic/NpnMgr.h"

#include "YmUtils/StopWatch.h"

//#define DEBUG

using namespace std;
using namespace nsYm;

typedef unordered_set<TvFunc> TvFuncSet;

bool verbose = false;

int check_count = 0;
int perm_count = 0;
int unresolved_count = 0;
ymuint mag = 1;

StopWatch sw;

double acum_time = 0.0;
double max_time = 0;

bool
str2vect(ymuint ni,
	 const char* buff,
	 vector<int>& v)
{
  ymuint ni_pow = 1 << ni;
  v.resize(ni_pow);

  for ( ; *buff == ' '; ++ buff) { }
  for (ymuint i = 0; i < ni_pow; ++ i) {
    char c = buff[i];
    if ( c == '1' ) {
      v[i] = 1;
    }
    else if ( c == '0' ) {
      v[i] = 0;
    }
    else {
      cerr << "Illegal char at " << i << " '" << c << "'("
	   << static_cast<int>(c) << ")" << endl;
      return false;
    }
  }
  return true;
}

ymuint
gen(ymuint ni,
    ymuint limit,
    ymuint flow,
    int algorithm,
    int dump)
{
  TvFuncSet repfunc_set;

  ymuint nerr = 0;

  ymuint ni_exp = 1 << ni;
  vector<int> buff(ni_exp);
  for (ymuint i = 0; i < ni_exp; ++ i) {
    buff[i] = 0;
  }

  sw.reset();
  ymuint frontier = 0;
  ymuint num = 0;
  ymulong w2count_total = 0;
  ymulong tvcount_total = 0;

  NpnMgr mgr;
  NpnMap map;
  TvFunc repfunc;
  for ( ; ; ) {
    if ( verbose ) {
      if ( buff[frontier] ) {
	for (ymuint i = 0; i < ni_exp; ++ i) {
	  cout << buff[i];
	}
	cout << endl;
      }
    }
    if ( buff[frontier] ) {
      ++ frontier;
    }

    TvFunc func(ni, buff);

    sw.start();
    for (ymuint i = 0; i < mag; ++ i) {
      mgr.cannonical(func, map, algorithm);
      if ( i == 0 ) {
	w2count_total += mgr.w2max_count();
	tvcount_total += mgr.tvmax_count();
      }
    }
    sw.stop();

    TvFunc orgfunc(ni, buff);
    repfunc = orgfunc.xform(map);
    if ( repfunc_set.count(repfunc) == 0 ) {
      repfunc_set.insert(repfunc);
    }
    ++ num;

    { // repfunc が本当に代表関数かどうかを検証する．
      NpnMap map1;
      mgr.cannonical(repfunc, map1);
      TvFunc func2 = repfunc.xform(map1);
      if ( repfunc != func2 ) {
	++ nerr;
	cerr << "repfunc: " << repfunc << endl
	     << "func2:   " << func2 << endl
	     << "orgfunc: " << orgfunc << endl
	     << endl;
      }
    }

    bool carry = false;
    for (ymuint i = 0; i < ni_exp; ++ i) {
      if ( buff[i] == 0 ) {
	buff[i] = 1;
	break;
      }
      buff[i] = 0;
      if ( i == ni_exp - 1 ) {
	carry = true;
      }
    }
    if ( carry ) {
      break;
    }
    if ( limit > 0 && frontier == limit ) {
      break;
    }
  }

  double usec = (sw.time().usr_time_usec() / num) / mag;
  cout << "Result of gen(ni = " << ni
       << ", mag = " << mag << ")"
       << endl
       << "w2max_recur:             "
       << static_cast<double>(w2count_total) / static_cast<double>(num)
       << endl
       << "tvmax_recur:             "
       << static_cast<double>(tvcount_total) / static_cast<double>(num)
       << endl
       << "NPN rep:                 " << repfunc_set.size() << endl
       << "AVE. CPU time(usec):     " << usec << endl;

  if ( dump ) {
    for (TvFuncSet::iterator p = repfunc_set.begin();
	 p != repfunc_set.end(); ++ p) {
      cout << ni << " " << *p << endl;
    }
  }

  return nerr;
}

ymuint
rgen(ymuint ni,
     int rseed,
     ymuint num,
     ymuint flow,
     int algorithm)
{
  TvFuncSet repfunc_set;

  ymuint nerr = 0;

  ymuint ni_exp = 1 << ni;
  vector<int> buff(ni_exp);

  RandGen rg;

  rg.init(rseed);

  ymulong w2count_total = 0;
  ymulong tvcount_total = 0;
  sw.reset();
  NpnMgr mgr;

  for (ymuint k = 0; k < num; ++ k) {
    for (ymuint i = 0; i < ni_exp; ++ i) {
      buff[i] = rg.int32() & 1;
    }
    if ( verbose ) {
      for (ymuint i = 0; i < ni_exp; ++ i) {
	cout << buff[i];
      }
      cout << endl;
    }

    TvFunc func(ni, buff);
    NpnMap map;

    sw.start();
    for (ymuint i = 0; i < mag; ++ i) {
      mgr.cannonical(func, map, algorithm);
      if ( i == 0 ) {
	w2count_total += mgr.w2max_count();
	tvcount_total += mgr.tvmax_count();
      }
    }
    sw.stop();

    { // repfunc が本当に代表関数かどうかを検証する．
      TvFunc repfunc = func.xform(map);
      NpnMap map1;
      mgr.cannonical(repfunc, map1);
      TvFunc func2 = repfunc.xform(map1);
      if ( repfunc != func2 ) {
	++ nerr;
	cerr << "repfunc: " << repfunc << endl
	     << "func2:   " << func2 << endl
	     << "orgfunc: " << func << endl
	     << endl;
      }
    }

    if ( flow & 1024 ) {
      TvFunc repfunc = func.xform(map);
      if ( repfunc_set.count(repfunc) == 0 ) {
	repfunc_set.insert(repfunc);
      }
    }
  }

  double usec = (sw.time().usr_time_usec() / num) / mag;
  cout << "Result of rgen(ni = " << ni
       << ", rseed = " << rseed
       << ", rnum = " << num
       << ", mag = " << mag << ")"
       << endl
       << "w2max_recur:             "
       << static_cast<double>(w2count_total) / static_cast<double>(num)
       << endl
       << "tvmax_recur:             "
       << static_cast<double>(tvcount_total) / static_cast<double>(num)
       << endl
       << "NPN rep:                 " << repfunc_set.size() << endl
       << "AVE. CPU time(usec):     " << usec << endl;

  return nerr;
}

void
rgen_walsh(ymuint ni,
	   int rseed,
	   ymuint num)
{
  TvFuncSet repfunc_set;

  ymuint ni_exp = 1 << ni;
  vector<int> buff(ni_exp);

  RandGen rg;
  rg.init(rseed);

  sw.reset();
  for (ymuint k = 0; k < num; ++ k) {
    for (ymuint i = 0; i < ni_exp; ++ i) {
      buff[i] = rg.int32() & 1;
    }
    if ( verbose ) {
      for (ymuint i = 0; i < ni_exp; ++ i) {
	cout << buff[i];
      }
      cout << endl;
    }

    TvFunc func(ni, buff);
    int w1[TvFunc::kMaxNi];
    //int w2[TvFunc::kMaxNi * TvFunc::kMaxNi];

    sw.start();
    for (ymuint i = 0; i < mag; ++ i) {
      //func.walsh_012(w1, w2);
      func.walsh_01(w1);
    }
    sw.stop();
  }

  double usec = (sw.time().usr_time_usec() / num) / mag;
  cout << "Result of rgen(ni = " << ni
       << ", rseed = " << rseed
       << ", rnum = " << num
       << ", mag = " << mag << ")"
       << endl
       << "AVE. CPU time(usec):     " << usec << endl;
}

void
read_func(const char* filename,
	  ymuint flow,
	  int algorithm)
{
  ifstream fs;
  fs.open(filename);
  if ( !fs ) {
    cerr << "Could not open " << filename << endl;
    return;
  }

#define buff_size (1024 * 1024 + 100)

  char buff[buff_size];
  vector<int> vect;

  ymuint num = 0;
  ymulong w2count_total = 0;
  sw.reset();
  while ( fs.peek() != EOF ) {
    ymuint ni;
    fs >> ni;
    fs.getline(buff, buff_size);
    bool stat = str2vect(ni, buff, vect);
    if ( !stat ) {
      cerr << "Illegal data" << endl;
      return;
    }

    TvFunc func(ni, vect);
    NpnMgr mgr;
    NpnMap map;

    ++ num;
    sw.start();
    for (ymuint i = 0; i < mag; ++ i) {
      mgr.cannonical(func, map, algorithm);
      if ( i == 0 ) {
	w2count_total += mgr.w2max_count();
      }
    }
    sw.stop();
  }

  double usec = sw.time().usr_time_usec() / mag;
  cout << endl
       << "w2max_recur:             "
       << static_cast<double>(w2count_total) / static_cast<double>(num)
       << endl
       << "AVE. CPU time(usec):     " << usec << endl;
}

ymuint
verify(ymuint ni,
       ymuint rseed,
       ymuint num,
       ymuint flow,
       int algorithm)
{
  RandGen rg;
  rg.init(rseed);

  ymuint nerr = 0;

  ymuint ni_exp = 1 << ni;
  vector<int> buff(ni_exp);
  ymuint c = 0;
  for (ymuint k = 0; k < num; ++ k) {
    for (ymuint i = 0; i < ni_exp; ++ i) {
      buff[i] = rg.int32() & 1;
    }
    if ( verbose ) {
      for (ymuint i = 0; i < ni_exp; ++ i) {
	cout << buff[i];
      }
      cout << endl;
    }

    TvFunc orig_func(ni, buff);
    NpnMgr mgr;
    NpnMap map0;
    sw.start();
    mgr.cannonical(orig_func, map0, algorithm);
    sw.stop();
    ++ c;

    TvFunc rep_func = orig_func.xform(map0);

    // すべての極性・順列組合わせのバリエーションをためして
    // おなじ代表関数になるかチェックする．
    ymuint ni_pow = 2 << ni;
    for (ymuint bits = 0; bits < ni_pow; ++ bits) {
      bool oinv = (bits & 1) ? true : false;
      ymuint ibits = bits >> 1;

      NpnMap pol_map(ni, oinv);
      for (ymuint i = 0; i < ni; ++ i) {
	if ( ibits & (1 << i) ) {
	  pol_map.set(VarId(i), VarId(i), true);
	}
	else {
	  pol_map.set(VarId(i), VarId(i), false);
	}
      }

      for (PermGen pg(ni, ni); !pg.is_end(); ++ pg) {
	NpnMap perm_map(ni);
	for (ymuint i = 0; i < ni; ++ i) {
	  ymuint j = pg(i);
	  perm_map.set(VarId(i), VarId(j), false);
	}

	NpnMap tmpmap = pol_map * perm_map;
	TvFunc tmp_func = orig_func.xform(tmpmap);
	NpnMgr mgr;
	NpnMap repmap;
	sw.start();
	mgr.cannonical(tmp_func, repmap, algorithm);
	sw.stop();
	++ c;

	TvFunc rep_func2 = tmp_func.xform(repmap);

	if ( rep_func2 != rep_func ) {
	  cerr << "Error" << endl
	       << " orig_func: " << orig_func << endl
	       << " rep_func:  " << rep_func << endl
	       << endl
	       << " tmp_map: " << tmpmap << endl
	       << " tmp_func:  " << tmp_func << endl
	       << " rep_func2: " << rep_func2 << endl;
	  ++ nerr;
	}
      }
    }
  }
  double usec = (sw.time().usr_time() / c);
  cout << "Result of verify(ni = " << ni
       << ", rseed = " << rseed
       << ", rnum = " << num
       << ")"
       << endl
#if 0
       << "Step1  undetermined num: " << ph1unum << endl
       << "       phase undet num:  " << ph1vnum << endl
       << "       permutations:     " << ph1pnum << endl
       << "Step2  undetermined num: " << ph2unum << endl
       << "       phase undet num:  " << ph2vnum << endl
       << "       permutations:     " << ph2pnum << endl
       << "Step3  undetermined num: " << ph3unum << endl
       << "       phase undet num:  " << ph3vnum << endl
       << "       permutations:     " << ph3pnum << endl
       << "Step4  undetermined num: " << ph4unum << endl
       << "       permutations:     " << ph4pnum << endl
#endif
       << "CPU time:                " << usec << endl;

  return nerr;
}

void
usage(const char* argv0)
{
  cerr << "USAGE : " << argv0 << " check <# of inputs> <vect1> <vect2>"
       << endl
       << "        " << argv0 << " gen <# of inputs>" << endl
       << "        " << argv0 << " rgen <# of inputs> <# of functions>"
       << " <random seed>" << endl
       << "        " << argv0 << " signature <# of inputs>" << endl;
}

int
main(int argc,
     const char** argv)
{

  ymuint nerr = 0;
#if HAVE_POPT
  int algorithm = 0;
  int mode = 0;
  int step1mode = 1;
  int step2mode = 1;
  int step3mode = 1;
  int step4mode = 1;
  int table = 0;
  int finish = 0;
  int ni = 4;
  int rnum = 10000;
  int rseed = 0;
  int dump = 0;

  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "verbose", 'v', POPT_ARG_NONE, &verbose, 0,
      "enable verbose mode", NULL },

    { "algorithm", 'a', POPT_ARG_INT, &algorithm, 0,
      "specify algorithm", "[0-3]"},

    { "gen", 'g', POPT_ARG_NONE, NULL, 1,
      "generate all NPN equivalent functions mode", NULL },

    { "dump", 'd', POPT_ARG_NONE, &dump, 0,
      "dump rep. functions", NULL },

    { "rgen", 'r', POPT_ARG_NONE, NULL, 2,
      "randomly generate NPN equivalent functions mode", NULL },

    { "verify", 'c', POPT_ARG_NONE, NULL, 3,
      "verify mode", NULL },

    { "func", 'f', POPT_ARG_NONE, NULL, 4,
      "read function mode", NULL },

    { "rgen_walsh", 'w', POPT_ARG_NONE, NULL, 5,
      "randomly generate NPN equivalent functions mode", NULL },

    { "step1", '1', POPT_ARG_INT, &step1mode, 0,
      "step1 submode", "(0/1)" },

    { "step2", '2', POPT_ARG_INT, &step2mode, 0,
      "step2 submode",  "(0/1)" },

    { "step3", '3', POPT_ARG_INT, &step3mode, 0,
      "step3 submode", "(0/1)" },

    { "step4", '4', POPT_ARG_INT, &step4mode, 0,
      "step4 submode", "(0/1)" },

    { "table", 't', POPT_ARG_NONE, &table, 0,
      "table look-up mode", NULL },

    { "finish", 'f', POPT_ARG_INT, &finish, 0,
      "finish", NULL },

    { "ni", 'i', POPT_ARG_INT, &ni, 0,
      "number of inputs", NULL },

    { "rnum", 'n', POPT_ARG_INT, &rnum, 0,
      "number of functions to be generated", NULL },

    { "rseed", 's', POPT_ARG_INT, &rseed, 0,
      "random seed", NULL },

    { "mag", 'm', POPT_ARG_INT, &mag, 0,
      "magnitude", NULL },

    POPT_AUTOHELP

    { NULL, '\0', 0, NULL, 0, NULL, NULL }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, "[OPTIONS]* <file-name> ...");

  // オプション解析を行う．
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

  const char* argname = poptGetArg(popt_context);
  if ( (mode == 4 && argname == NULL) ||
       (mode != 4 && argname != NULL) ) {
    usage(argv[0]);
    exit(1);
  }

  int flow = 0;
  if ( step1mode ) {
    flow |= 1;
  }
  if ( step2mode ) {
    flow |= 2;
  }
  if ( step3mode ) {
    flow |= 4;
  }
  if ( step4mode ) {
    flow |= 8;
  }
  if ( table ) {
    flow |= 0x10000000;
  }
  if ( finish ) {
    flow |= 1024;
  }

#if !defined(YM_DEBUG)
  try {
#endif
    switch ( mode ) {
    case 1: // gen
      nerr = gen(ni, 0, flow, algorithm, dump);
      break;

    case 2: // rgen
      nerr = rgen(ni, rseed, rnum, flow, algorithm);
      break;

    case 3: // verify
      nerr = verify(ni, rseed, rnum, flow, algorithm);
      break;

    case 4: // function
      read_func(argname, flow, algorithm);
      break;

    case 5: // rgen_walsh
      rgen_walsh(ni, rseed, rnum);
      break;

    default:
      usage(argv[0]);
      exit(1);
    }
#if !defined(YM_DEBUG)
  }

  catch ( nsYm::AssertError x ) {
    cerr << x << endl;
  }
#endif
#else

#if 0
  int base = 1;
  bool dump_flag = false;
  ymuint flow_end = 1024;
  for ( ; base < argc; ++ base) {
    if ( argv[base][0] != '-' ) {
      break;
    }
    if ( strcmp(argv[base], "--verbose") == 0 ) {
      verbose = true;
    }
    else if ( strcmp(argv[base], "--dump") == 0 ) {
      dump_flag = true;
    }
    else if ( strcmp(argv[base], "--signature") == 0 ) {
      flow_end = 0;
    }
  }
  if ( argc - base <= 1 ) {
    usage(argv[0]);
    return 2;
  }

  try {
    if ( strcmp(argv[base], "signature") == 0 ) {
      if ( argc - base != 2 ) {
	usage(argv[0]);
	return 2;
      }

      ymuint ni = atoi(argv[base + 1]);
      NpnSigMgr npnsigmgr;
      ymuint ni_pow = 1 << ni;
      char buff[256];
      while ( cin.getline(buff, sizeof(buff)) ) {
	vector<int> tmp(ni_pow);
	str2vect(ni, buff, tmp);
	reg(npnsigmgr, ni, tmp, 0);
      }
    }
    else if ( strcmp(argv[base], "check") == 0 ) {
      if ( argc - base != 4 ) {
	usage(argv[0]);
	return 2;
      }

      ymuint ni = atoi(argv[base + 1]);
      NpnSigMgr npnsigmgr;
      vector<int> tmp(1 << ni);
      str2vect(ni, argv[base + 2], tmp);
      reg(npnsigmgr, ni, tmp, 0);
      str2vect(ni, argv[base + 3], tmp);
      reg(npnsigmgr, ni, tmp, 0);
    }
    else if ( strcmp(argv[base], "check1") == 0 ) {
      if ( argc - base != 4 ) {
	usage(argv[0]);
	return 2;
      }

      ymuint ni = atoi(argv[base + 1]);
      NpnSigMgr npnsigmgr;
      vector<int> tmp(1 << ni);
      str2vect(ni, argv[base + 2], tmp);
      reg(npnsigmgr, ni, tmp, 1);
      str2vect(ni, argv[base + 3], tmp);
      reg(npnsigmgr, ni, tmp, 1);
    }
    else if ( strcmp(argv[base], "gen") == 0 ) {
      if ( argc - base != 3 ) {
	usage(argv[0]);
	return 2;
      }

      ymuint ni = atoi(argv[base + 1]);
      ymuint flow = atoi(argv[base + 2]) | flow_end;
      gen(ni, 0, flow, 2);
    }
    else if ( strcmp(argv[base], "ngen") == 0 ) {
      if ( argc - base != 3 ) {
	usage(argv[0]);
	return 2;
      }

      ymuint ni = atoi(argv[base + 1]);
      ymuint n = atoi(argv[base + 2]);
      gen(ni, n, 0, 2);
    }
    else if ( strcmp(argv[base], "rgen") == 0 ) {
      if ( argc - base != 5 ) {
	usage(argv[0]);
	return 2;
      }

      ymuint ni = atoi(argv[base + 1]);
      ymuint flow = atoi(argv[base + 2]) | flow_end;
      ymuint num = atoi(argv[base + 3]);
      int seed = atoi(argv[base + 4]);
      init_random_seed(seed);
      rgen(ni, num, flow, 2);
    }
    else if ( strcmp(argv[base], "verify") == 0 ) {
      if ( argc - base != 5 ) {
	usage(argv[0]);
	return 2;
      }

      ymuint ni = atoi(argv[base + 1]);
      ymuint flow = atoi(argv[base + 2]) | flow_end;
      ymuint num = atoi(argv[base + 3]);
      int seed = atoi(argv[base + 4]);
      init_random_seed(seed);
      verify(ni, num, flow, 2);
    }
    else {
      usage(argv[0]);
      return 2;
    }
  }
  catch ( nsYm::AssertError x ) {
    cerr << x;
  }
#endif
#endif

  return nerr;
}
