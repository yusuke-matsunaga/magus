
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
#include "VarSplitter.h"
#include "XorSplitter.h"
#include "MultiSplitter.h"
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

  // Splitter を生成
  vector<XorSplitter*> splitters;
  for (ymuint i = 0; i < n; ++ i) {
    XorSplitter* splitter = new XorSplitter(vector<ymuint32>(1, i));
    splitters.push_back(splitter);
  }
  for (ymuint comp_i = 2; comp_i <= comp; ++ comp_i) {
    CombiGen cg(n, comp_i);

    for (CombiGen::iterator p = cg.begin(); !p.is_end(); ++ p) {
      vector<ymuint32> var_list(2);
      var_list[0] = p(0);
      var_list[1] = p(1);
      XorSplitter* splitter = new XorSplitter(var_list);
      splitters.push_back(splitter);
    }
  }

  cout << endl;
  for (vector<XorSplitter*>::iterator p = splitters.begin();
       p != splitters.end(); ++ p) {
    XorSplitter& splitter = **p;
    const vector<ymuint32>& var_list = splitter.varid_list();
    for (vector<ymuint32>::const_iterator q = var_list.begin();
	 q != var_list.end(); ++ q) {
      cout << " " << *q;
    }
    cout << endl;

    const vector<RegVect*>& vect_list = rvmgr.vect_list();
    for (vector<RegVect*>::const_iterator q = vect_list.begin();
	 q != vect_list.end(); ++ q) {
      RegVect* vect = *q;
      if ( splitter(vect) == 0 ) {
	vect->dump(cout);
      }
    }
    cout << "-------------" << endl;
    for (vector<RegVect*>::const_iterator q = vect_list.begin();
	 q != vect_list.end(); ++ q) {
      RegVect* vect = *q;
      if ( splitter(vect) == 1 ) {
	vect->dump(cout);
      }
    }
    cout << endl
	 << "=============" << endl;
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
