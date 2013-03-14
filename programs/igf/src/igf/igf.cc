
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
  ymuint32 multi = 0;
  ymuint32 comp = 0;

  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "indirect", 'i', POPT_ARG_NONE, NULL, 'i',
      "indirect method", NULL},
    { "direct", 'd', POPT_ARG_NONE, NULL, 'd',
      "direct method", NULL},
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
