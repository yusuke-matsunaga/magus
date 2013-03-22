
/// @file igf.cc
/// @brief igf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_POPT
#include <popt.h>
#else
#error "<popt.h> not found."
#endif


#include "igf_nsdef.h"
#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"
#include "IguGen.h"
#include "ym_utils/Generator.h"


BEGIN_NAMESPACE_YM_IGF

// usage を出力する．
void
usage(poptContext optCon,
      int exitcode,
      const char* error = NULL,
      const char* addl = NULL)
{
  poptPrintUsage(optCon, stderr, 0);
  if ( error ) {
    fprintf(stderr, "%s: %s\n", error, addl);
  }
  exit(exitcode);
}

void
solve_igf(const vector<RegVect*>& vector_list,
	  ymuint multi,
	  const vector<Variable*>& variable_list,
	  vector<Variable*>& solution)
{
}

int
igf(int argc,
    const char** argv)
{
  ymuint32 multi = 1;
  ymuint32 comp = 1;
  ymuint32 blimit = 0;
  ymuint32 tlimit = 0;
  ymuint32 debug = 0;

  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "multi", 'm', POPT_ARG_INT, &multi, 'm',
      "specify multiplicity", "<INT>"},
    { "xor-complex", 'x', POPT_ARG_INT, &comp, 'x',
      "specify XOR complexity", "<INT>"},
    { "branch-limit", 'b', POPT_ARG_INT, &blimit, 'b',
      "set branch limit", "<INT>"},
    { "time-limit", 't', POPT_ARG_INT, &tlimit, 't',
      "set time limit", "<INT>(min)"},
    { "debug-level", 'd', POPT_ARG_INT, &debug, 'd',
      "set_debug_level", "<INT>"},

    POPT_AUTOHELP

    { NULL, '\0', 0, NULL, 0, NULL, NULL }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, " <dimension of vectors> <# of vectors>");

  // オプション解析行う．
  for ( ; ; ) {
    int rc = poptGetNextOpt(popt_context);
    if ( rc == -1 ) {
      break;
    }
    if ( rc < -1 ) {
      // エラーが起きた．
      cerr << poptBadOption(popt_context, POPT_BADOPTION_NOALIAS)
	   << ": " << poptStrerror(rc) << endl;
      return 1;
    }
  }

  const char* f_str = poptGetArg(popt_context);
  if ( f_str == NULL ) {
    usage(popt_context, 2);
  }

  const char* dummy = poptGetArg(popt_context);
  if ( dummy != NULL ) {
    usage(popt_context, 2);
  }

  ifstream ifs(f_str);
  if ( !ifs ) {
    cerr << f_str << ": No such file" << endl;
    return 2;
  }

  RvMgr rvmgr;

  bool stat = rvmgr.read_data(ifs);
  if ( !stat ) {
    cerr << "read error" << endl;
    return 3;
  }

  ymuint n = rvmgr.vect_size();

  // Variable を生成
  vector<Variable*> var_list;
  for (ymuint i = 0; i < n; ++ i) {
    Variable* var = new Variable(i);
    var_list.push_back(var);
  }
  for (ymuint comp_i = 2; comp_i <= comp; ++ comp_i) {
    CombiGen cg(n, comp_i);

    for (CombiGen::iterator p = cg.begin(); !p.is_end(); ++ p) {
      vector<ymuint> vid_list(comp_i);
      for (ymuint j = 0; j < comp_i; ++ j) {
	vid_list[j] = p(j);
      }
      Variable* var = new Variable(vid_list);
      var_list.push_back(var);
    }
  }

  IguGen igu_gen;
  vector<Variable*> solution;

  if ( blimit > 0 ) {
    igu_gen.set_branch_limit(blimit);
  }
  if ( tlimit > 0 ) {
    igu_gen.set_time_limit(tlimit, 0);
  }
  if ( debug > 0 ) {
    igu_gen.set_debug_level(debug);
  }

  igu_gen.solve(rvmgr.vect_list(), multi, var_list, solution);

  cout << "Variables = " << endl;
  for (vector<Variable*>::iterator p = solution.begin();
       p != solution.end(); ++ p) {
    Variable* var = *p;
    const vector<ymuint>& vid_list = var->vid_list();
    for (vector<ymuint>::const_iterator q = vid_list.begin();
	 q != vid_list.end(); ++ q) {
      cout << " " << *q;
    }
    cout << endl;
  }
  cout << endl;

  {
    ymuint n = rvmgr.vect_size();
    ymuint p = solution.size();
    ymuint q = rvmgr.index_size();
    ymuint exp_p = (1 << p);
    ymuint exp_q = (1 << q);
    cout << "p = " << p << endl
	 << "k = " << rvmgr.vect_list().size() << endl
	 << "q = " << q << endl;
    cout << "Total memory size = "
	 << exp_p << " x (" << n << " - " << p << " + " << q << ") x " << multi
	 << " = " << (exp_p * (n - p + q) * multi) << endl;
    cout << "Sasao's IGU       = "
	 << exp_p << " x " << q << " + " << exp_q << " x ("
	 << n << " - " << p << ") x " << multi
	 << " = " << ((exp_p * q + exp_q * (n - p)) * multi) << endl;
  }
  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::igf;

  return igf(argc, argv);
}
