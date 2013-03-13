
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

class BinaryRandGen
{
public:

  /// @brief コンストラクタ
  BinaryRandGen();

  /// @brief デストラクタ
  ~BinaryRandGen();


public:

  /// @brief シードで初期化する．
  /// @param[in] seed 初期化で用いるシード
  void
  init(ymuint32 seed);

  /// @brief 0/1 の乱数を発生させる．
  bool
  value();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 乱数発生器
  RandGen mRandGen;

  // 一時バッファ
  ymuint32 mBuffer;

  // バッファ中の次に利用可能な位置
  ymuint32 mNextPos;

};


// @brief コンストラクタ
BinaryRandGen::BinaryRandGen()
{
  mNextPos = 32;
}

// @brief デストラクタ
BinaryRandGen::~BinaryRandGen()
{
}

// @brief シードで初期化する．
// @param[in] seed 初期化で用いるシード
void
BinaryRandGen::init(ymuint32 seed)
{
  mRandGen.init(seed);
}

// @brief 0/1 の乱数を発生させる．
bool
BinaryRandGen::value()
{
  if ( mNextPos >= 32 ) {
    mBuffer = mRandGen.int32();
    mNextPos = 0;
  }
  bool ans = static_cast<bool>((mBuffer >> mNextPos) & 1U);
  ++ mNextPos;
  return ans;
}


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

// n 次元 k 個の登録ベクタを出力する．
void
gen_vectors(ymuint n,
	    ymuint k,
	    BinaryRandGen& rg,
	    ostream& s)
{
  s << n << " " << k << endl;
  // k 個のベクタを出力
  for (ymuint i = 0; i < k; ++ i) {
    for (ymuint j = 0; j < n; ++ j) {
      s << rg.value();
    }
    s << endl;
  }
}

int
rvgen(int argc,
      const char** argv)
{
  ymuint32 seed = 0;

  BinaryRandGen rg;

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
    if ( rc == 1 ) {
      // seed オプション
      rg.init(seed);
    }
  }

  const char* n_str = poptGetArg(popt_context);
  if ( n_str == NULL ) {
    usage(popt_context, 2);
  }

  const char* k_str = poptGetArg(popt_context);
  if ( k_str == NULL ) {
    usage(popt_context, 2);
  }

  const char* dummy = poptGetArg(popt_context);
  if ( dummy != NULL ) {
    usage(popt_context, 2);
  }

  int tmp_n = atoi(n_str);
  if ( tmp_n <= 0 ) {
    cerr << "Argument value error: first argument should be >= 0" << endl;
    return 3;
  }

  int tmp_k = atoi(k_str);
  if ( tmp_k <= 0 ) {
    cerr << "Argument value error: second argument should be >= 0" << endl;
    return 3;
  }

  ymuint n = tmp_n;
  ymuint k = tmp_k;

  ymuint np = 1U << n;
  if ( n < 32 && k > np ) {
    cerr << "Argument value error: second argument should be < 2^n" << endl;
    return 3;
  }


  //  n と k を出力
  gen_vectors(n, k, rg, cout);

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::rvgen;

  return rvgen(argc, argv);
}
