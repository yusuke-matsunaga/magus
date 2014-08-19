
/// @file genaig.cc
/// @brief N 入力関数を実現する AIG  を生成するプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_POPT
#include <popt.h>
#else
#error "<popt.h> not found."
#endif

#include "YmTools.h"
#include "GenAig.h"
#include "GenAigNaive.h"
#include "GenAigDj.h"


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;

  const char* method_str = "exact";
  int slack = 0;

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
      "specify method", "exact|naive|disjoint" },

    { "slack", 's', POPT_ARG_INT, &slack, 0,
      "specify slack value", NULL },

    POPT_AUTOHELP

    { NULL, '\0', 0, NULL, 0, NULL, NULL }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, "[OPTIONS]* <#inputs>");

  // オプション解析を行なう．
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
  }

  // 残りの引数は入力数
  const char* str = poptGetArg(popt_context);
  if ( str == NULL ) {
    fprintf(stderr, "No arguments\n");
    return 2;
  }

  ymuint ni = atoi(str);

  if ( strcmp(method_str, "exact") == 0 ) {
    GenAig genaig;

    cout << "genaig(" << ni << ", " << slack << ")" << endl;
    genaig(ni, slack);
  }
  else if ( strcmp(method_str, "naive") == 0 ) {
    GenAigNaive genaig;

    cout << "genaig_naive(" << ni << ", " << slack << ")" << endl;
    genaig(ni, slack);
  }
  else if ( strcmp(method_str, "disjoint") == 0 ) {
    GenAigDj genaig;

    cout << "genaig_disjoint(" << ni << ")" << endl;
    genaig(ni);
  }
  else {
    cerr << "Invalid method name: " << method_str << endl;
  }

  return 0;
}
