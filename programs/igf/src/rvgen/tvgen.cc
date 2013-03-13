
/// @file rvgen.cc
/// @brief rvgen の実装ファイル
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
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

int
rvgen(int argc,
      char** argv)
{
  int seed = 1234;

  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "seed", 's', POPT_ARG_INT, &seed, 1,
      "specify random seed", "<INT>"},

    POPT_AUTOHELP

    { NULL, '\0', 0, NULL, 0, NULL, NULL }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, "[OPTIONS]* <dimension of vectors> <# of vectors>");

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
    if ( rc == 1 ) {
      // seed オプション

    }
  }

  const char* n_str = poptGetArg(popt_context);
  if ( n_str == NULL ) {
    usage();
    return 2;
  }

  const char* k_str = poptGetArg(popt_context);
  if ( k_str == NULL ) {
    usage();
    return 2;
  }

  const char* dummy = poptGetArg(popt_context);
  if ( dummy != NULL ) {
    usage();
    return 2;
  }

  int n = atoi(n_str);
  if ( n <= 0 ) {
    cerr << "Argument value error: first argument should be >= 0" << endl;
    return 3;
  }

  int k = atoi(k_str);
  if ( k <= 0 ) {
    cerr << "Argument value error: second argument should be >= 0" << endl;
    return 3;
  }

  ymuint np = 1U << n;
  if ( k > np ) {
    cerr << "Argument value error: second argument should be < 2^n" << endl;
    return 3;
  }

}

END_NAMESPACE_YM_IGF


int
main(int argc,
     char** argv)
{
  using nsYm::nsIgf::rvgen;

  return rvgen(argc, argv);
}
