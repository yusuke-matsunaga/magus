
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

int
igf(int argc,
    const char** argv)
{
  ymuint32 multi = 1;
  ymuint32 comp = 1;

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

  rvmgr.dump(cout);

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

#if 0
  cout << endl;
  for (vector<Variable*>::iterator p = var_list.begin();
       p != var_list.end(); ++ p) {
    Variable* var = *p;
    const vector<ymuint>& vid_list = var->vid_list();
    for (vector<ymuint>::const_iterator q = vid_list.begin();
	 q != vid_list.end(); ++ q) {
      cout << " " << *q;
    }
    cout << endl;

    const vector<RegVect*>& vect_list = rvmgr.vect_list();
    for (vector<RegVect*>::const_iterator q = vect_list.begin();
	 q != vect_list.end(); ++ q) {
      RegVect* vect = *q;
      if ( var->classify(vect) == 0 ) {
	vect->dump(cout);
      }
    }
    cout << "-------------" << endl;
    for (vector<RegVect*>::const_iterator q = vect_list.begin();
	 q != vect_list.end(); ++ q) {
      RegVect* vect = *q;
      if ( var->classify(vect) == 1 ) {
	vect->dump(cout);
      }
    }
    cout << endl
	 << "=============" << endl;
  }
#else
  cout << endl;
  for (vector<Variable*>::iterator p = var_list.begin();
       p != var_list.end(); ++ p) {
    Variable* var = *p;
    const vector<ymuint>& vid_list = var->vid_list();
    for (vector<ymuint>::const_iterator q = vid_list.begin();
	 q != vid_list.end(); ++ q) {
      cout << " " << *q;
    }
    cout << "     ";

    ymuint n0 = 0;
    ymuint n1 = 0;
    const vector<RegVect*>& vect_list = rvmgr.vect_list();
    for (vector<RegVect*>::const_iterator q = vect_list.begin();
	 q != vect_list.end(); ++ q) {
      RegVect* vect = *q;
      if ( var->classify(vect) == 0 ) {
	++ n0;
      }
      else {
	++ n1;
      }
    }
    cout << n0 << " : " << n1 << endl;
  }
#endif

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
