
/// @file PoptMainApp.cc
/// @brief PoptMainApp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/PoptMainApp.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PoptMainApp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前 (alias 用)
// @param[in] auto_help --help オプションを有効にするフラグ
PoptMainApp::PoptMainApp(const char* name,
			 bool auto_help)
{
  mName = name;
  mAutoHelp = auto_help;
  mDone = false;
  mOptionTable = NULL;
  mCon = 0;
  mHelpText = NULL;
}

// @brief デストラクタ
PoptMainApp::~PoptMainApp()
{
  delete [] mOptionTable;
  poptFreeContext(mCon);
}

// @brief オプションを追加する．
// @param[in] option 追加するオプション
void
PoptMainApp::add_option(Popt* option)
{
  mOptionList.push_back(option);
}

// @brief ヘルプ文字列を指定する．
// @param[in] text ヘルプ文字列
void
PoptMainApp::set_other_option_help(const char* text)
{
  mHelpText = text;
}

// @brief リセットする．
void
PoptMainApp::reset()
{
  poptResetContext(mCon);
}

// @brief オプション解析を行なう．
// @param[in] argc コマンド行の引数の数
// @param[in] argv コマンド行の引数配列
// @param[in] flags フラグ
tPoptStat
PoptMainApp::parse_options(int argc,
			   const char** argv,
			   int flags)
{
  if ( mDone ) {
    cerr << "PoptMainApp::parse_options() is called more than once." << endl;
    return kPoptAbort;
  }

  // オプションテーブルを作る．
  ymuint n = mOptionList.size();
  ymuint n1 = n;
  if ( mAutoHelp ) {
    ++ n1;
  }
  // end-marker の分を1つ足す必要がある．
  mOptionTable = new poptOption[n1 + 1];
  for (ymuint i = 0; i < n; ++ i) {
    poptOption& option = mOptionTable[i];
    Popt* opt = mOptionList[i];
    option.longName = opt->opt_str();
    option.shortName = opt->opt_char();
    option.argInfo = opt->arg_info();
    option.arg = opt->arg();
    option.val = i + 1;
    option.descrip = opt->opt_desc();
    option.argDescrip = opt->arg_desc();
  }
  if ( mAutoHelp ) {
    poptOption& option = mOptionTable[n];
    option.longName = NULL;
    option.shortName = '\0';
    option.argInfo = POPT_ARG_INCLUDE_TABLE;
    option.arg = poptHelpOptions;
    option.val = 0;
    option.descrip = "Help options:";
    option.argDescrip = NULL;
    ++ n;
  }
  { // end-markder
    poptOption& option = mOptionTable[n1];
    option.longName = NULL;
    option.shortName = '\0';
    option.argInfo = 0;
    option.arg = NULL;
    option.val = 0;
    option.descrip = NULL;
    option.argDescrip = NULL;
  }

  // poptContext を作る．
  mCon = poptGetContext(mName, argc, argv, mOptionTable, flags);

  if ( mHelpText != NULL ) {
    poptSetOtherOptionHelp(mCon, mHelpText);
  }

  // オプション解析行う．
  for ( ; ; ) {
    int rc = poptGetNextOpt(mCon);
    if ( rc == -1 ) {
      break;
    }
    if ( rc < -1 ) {
      // エラーが起きた．
      cerr << poptBadOption(mCon, POPT_BADOPTION_NOALIAS)
	   << ": " << poptStrerror(rc) << endl;
      return kPoptAbort;
    }
    ymuint id = static_cast<ymuint>(rc);
    assert_cond( id > 0, __FILE__, __LINE__);
    -- id;
    assert_cond( id < mOptionList.size(), __FILE__, __LINE__);
    Popt* popt = mOptionList[id];
    ++ popt->mCount;
    // 該当するオプションの処理を行なう．
    bool stat = popt->action();
    if ( !stat ) {
      // 続行不可能なエラーが起きた．
      return kPoptAbort;
    }
  }
  return kPoptOk;
}

// @brief 残った引数を得る．
// @param[in] args 引数を格納するベクタ
// @return 引数の数を返す．
ymuint
PoptMainApp::get_args(vector<string>& args)
{
  args.clear();
  for ( ; ; ) {
    const char* arg = poptGetArg(mCon);
    if ( arg == NULL ) {
      break;
    }
    args.push_back(string(arg));
  }
  return args.size();
}

// @brief ヘルプメッセージを出力する．
// @param[in] fp FILE 構造体へのポインタ(古！)
// @param[in] flags フラグ(現時点では未使用)
void
PoptMainApp::print_help(FILE* fp,
			int flags)
{
  poptPrintHelp(mCon, fp, flags);
}

// @brief ユーセージ(ショートヘルプ)メッセージを出力する．
// @param[in] fp FILE 構造体へのポインタ(古！)
// @param[in] flags フラグ(現時点では未使用)
void
PoptMainApp::print_usage(FILE* fp,
			 int flags)
{
  poptPrintUsage(mCon, fp, flags);
}

// @brief usage を出力して終了する．
void
PoptMainApp::usage(int exitcode,
		   const char* error,
		   const char* addl)
{
  print_usage(stderr, 0);
  if ( error ) {
    fprintf(stderr, "%s: %s\n", error, addl);
  }
  exit(exitcode);
}

// @brief PoptMainApp 用の strerror() 関数
// @param[in] error エラーコード
const char*
PoptMainApp::popt_strerror(const int error)
{
  return poptStrerror(error);
}

// @brief エラーが起きた場合にそのもととなったオプション文字列を返す．
// @param[in] flags フラグ
// @note フラグに使用可能な値は以下のとおり
//  - 0 なにもしない．
//  - POPT_BADOPTION_NOALIAS もっとも外側(outermost)のオプションを返す．
const char*
PoptMainApp::bad_option(int flags)
{
  return poptBadOption(mCon, flags);
}

// @brief alias 用のデフォルト設定を読み込む．
// @param[in] flags フラグ(現時点では未使用)
// @return 返り値に関しては man popt に記載なし．
int
PoptMainApp::read_default_config(int flags)
{
  return poptReadDefaultConfig(mCon, flags);
}

// @brief alias 用の設定ファイルを読み込む．
// @param[in] filename ファイル名
int
PoptMainApp::read_config_file(const char* filename)
{
  return poptReadConfigFile(mCon, filename);
}

// @brief alias を追加する．
// @param[in] alias alias 構造体
// @param[in] flags フラグ(現時点では未使用)
int
PoptMainApp::add_alias(struct poptAlias alias,
		       int flags)
{
  return poptAddAlias(mCon, alias, flags);
}


//////////////////////////////////////////////////////////////////////
// クラス Popt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opt_str このオプションを表す文字列
// @param[in] opt_char このオプションを表す文字
// @param[in] opt_desc このオプションの説明文
// @note opt_str が空文字列だったり opt_char が \0 だったりする場合もある．
Popt::Popt(const char* opt_str,
	   char opt_char,
	   const char* opt_desc) :
  mOptStr(opt_str),
  mOptChar(opt_char),
  mOptDesc(opt_desc),
  mCount(0)
{
}

// @brief デストラクタ
Popt::~Popt()
{
}

// @brief オプション文字列を返す．
const char*
Popt::opt_str() const
{
  return mOptStr;
}

// @brief オプション文字を返す．
char
Popt::opt_char() const
{
  return mOptChar;
}

// @brief オプションの説明文を返す．
const char*
Popt::opt_desc() const
{
  return mOptDesc;
}

// @brief オプションの引数の記述を返す．
// @note デフォルトの実装では NULL を返す．
const char*
Popt::arg_desc() const
{
  return NULL;
}

// @brief このオブジェクトが解析中に呼ばれていたら true を返す．
bool
Popt::is_specified() const
{
  return count() > 0;
}

// @brief このオブジェクトが呼ばれた回数を返す．
int
Popt::count() const
{
  return mCount;
}

// @brief オプションが指定されたときに呼び出される関数
// @return 処理結果を返す．
bool
Popt::action()
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス PoptNone
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opt_str オプション文字列
// @param[in] opt_char オプション文字
// @param[in] opt_desc オプションの説明文
PoptNone::PoptNone(const char* opt_str,
		   char opt_char,
		   const char* opt_desc) :
  Popt(opt_str, opt_char, opt_desc)
{
}

// @brief デストラクタ
PoptNone::~PoptNone()
{
}

// @brief argInfo の値を返す．
int
PoptNone::arg_info()
{
  return POPT_ARG_NONE;
}

// @brief arg の値を返す．
void*
PoptNone::arg()
{
  return NULL;
}



//////////////////////////////////////////////////////////////////////
// クラス PoptArg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opt_str オプション文字列
// @param[in] opt_char オプション文字
// @param[in] opt_desc オプションの説明文
// @param[in] arg_desc 引数の説明文
PoptArg::PoptArg(const char* opt_str,
		 char opt_char,
		 const char* opt_desc,
		 const char* arg_desc) :
  Popt(opt_str, opt_char, opt_desc),
  mArgDesc(arg_desc)
{
}

// @brief デストラクタ
PoptArg::~PoptArg()
{
}

// @brief オプションの引数の記述を返す．
// @note デフォルトの実装では NULL を返す．
const char*
PoptArg::arg_desc() const
{
  return mArgDesc;
}


//////////////////////////////////////////////////////////////////////
// クラス PoptStr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opt_str オプション文字列
// @param[in] opt_char オプション文字
// @param[in] opt_desc オプションの説明文
// @param[in] arg_desc 引数の説明文
PoptStr::PoptStr(const char* opt_str,
		 char opt_char,
		 const char* opt_desc,
		 const char* arg_desc) :
  PoptArg(opt_str, opt_char, opt_desc, arg_desc)
{
}

// @brief デストラクタ
PoptStr::~PoptStr()
{
}

// @brief argInfo の値を返す．
int
PoptStr::arg_info()
{
  return POPT_ARG_STRING;
}

// @brief arg の値を返す．
void*
PoptStr::arg()
{
  return &mVal;
}

// @brief オプション引数の値を返す．
string
PoptStr::val() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス PoptInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opt_str オプション文字列
// @param[in] opt_char オプション文字
// @param[in] opt_desc オプションの説明文
// @param[in] arg_desc 引数の説明文
PoptInt::PoptInt(const char* opt_str,
		 char opt_char,
		 const char* opt_desc,
		 const char* arg_desc) :
  PoptArg(opt_str, opt_char, opt_desc, arg_desc)
{
}

// @brief デストラクタ
PoptInt::~PoptInt()
{
}

// @brief argInfo の値を返す．
int
PoptInt::arg_info()
{
  return POPT_ARG_INT;
}

// @brief arg の値を返す．
void*
PoptInt::arg()
{
  return &mVal;
}

// @brief オプション引数の値を返す．
ymint
PoptInt::val() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス PoptBool
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opt_str オプション文字列
// @param[in] opt_char オプション文字
// @param[in] opt_desc オプションの説明文
// @param[in] arg_desc 引数の説明文
PoptBool::PoptBool(const char* opt_str,
		   char opt_char,
		   const char* opt_desc,
		   const char* arg_desc) :
  PoptInt(opt_str, opt_char, opt_desc, arg_desc)
{
}

// @brief デストラクタ
PoptBool::~PoptBool()
{
}

// @brief オプション引数の値を返す．
bool
PoptBool::val() const
{
  return static_cast<bool>(PoptInt::val());
}


//////////////////////////////////////////////////////////////////////
// クラス PoptUint
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opt_str オプション文字列
// @param[in] opt_char オプション文字
// @param[in] opt_desc オプションの説明文
// @param[in] arg_desc 引数の説明文
PoptUint::PoptUint(const char* opt_str,
		   char opt_char,
		   const char* opt_desc,
		   const char* arg_desc) :
  PoptInt(opt_str, opt_char, opt_desc, arg_desc)
{
}

// @brief デストラクタ
PoptUint::~PoptUint()
{
}

// @brief オプション引数の値を返す．
ymuint
PoptUint::val() const
{
  return static_cast<ymuint>(PoptInt::val());
}


//////////////////////////////////////////////////////////////////////
// クラス PoptFloat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opt_str オプション文字列
// @param[in] opt_char オプション文字
// @param[in] opt_desc オプションの説明文
// @param[in] arg_desc 引数の説明文
PoptFloat::PoptFloat(const char* opt_str,
		     char opt_char,
		     const char* opt_desc,
		     const char* arg_desc) :
  PoptArg(opt_str, opt_char, opt_desc, arg_desc)
{
}

// @brief デストラクタ
PoptFloat::~PoptFloat()
{
}

// @brief argInfo の値を返す．
int
PoptFloat::arg_info()
{
  return POPT_ARG_FLOAT;
}

// @brief arg の値を返す．
void*
PoptFloat::arg()
{
  return &mVal;
}

// @brief オプション引数の値を返す．
float
PoptFloat::val() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス PoptDouble
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opt_str オプション文字列
// @param[in] opt_char オプション文字
// @param[in] opt_desc オプションの説明文
// @param[in] arg_desc 引数の説明文
PoptDouble::PoptDouble(const char* opt_str,
		       char opt_char,
		       const char* opt_desc,
		       const char* arg_desc) :
  PoptArg(opt_str, opt_char, opt_desc, arg_desc)
{
}

// @brief デストラクタ
PoptDouble::~PoptDouble()
{
}

// @brief argInfo の値を返す．
int
PoptDouble::arg_info()
{
  return POPT_ARG_DOUBLE;
}

// @brief arg の値を返す．
void*
PoptDouble::arg()
{
  return &mVal;
}

// @brief オプション引数の値を返す．
double
PoptDouble::val() const
{
  return mVal;
}

END_NAMESPACE_YM
