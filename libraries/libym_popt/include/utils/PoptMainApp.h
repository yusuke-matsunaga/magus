#ifndef UTILS_POPTMAINAPP_H
#define UTILS_POPTMAINAPP_H

/// @file utils/PoptMainApp.h
/// @brief PoptMainApp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

// このクラスは絶対に popt が必要
#if HAVE_POPT
#include <popt.h>
#else
#error "<popt.h> not found."
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief popt のオプション解析結果
//////////////////////////////////////////////////////////////////////
enum tPoptStat
{
  /// @brief 通常の処理された
  kPoptOk,
  /// @brief エラーだが処理を続行する．
  kPoptError,
  /// @brief 処理を中断する．
  kPoptAbort
};


//////////////////////////////////////////////////////////////////////
/// @class Popt PoptMainApp.h "utils/PoptMainApp.h"
/// @brief popt の個々の要素を表すクラス
//////////////////////////////////////////////////////////////////////
class Popt
{
  friend class PoptMainApp;

public:

  /// @brief コンストラクタ
  /// @param[in] opt_str このオプションを表す文字列
  /// @param[in] opt_char このオプションを表す文字
  /// @param[in] opt_desc このオプションの説明文
  /// @note opt_str が空文字列だったり opt_char が \0 だったりする場合もある．
  Popt(const char* opt_str,
       char opt_char,
       const char* opt_desc);

  /// @brief デストラクタ
  virtual
  ~Popt();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列を返す．
  const char*
  opt_str() const;

  /// @brief オプション文字を返す．
  char
  opt_char() const;

  /// @brief argInfo の値を返す．
  virtual
  int
  arg_info() = 0;

  /// @brief arg の値を返す．
  virtual
  void*
  arg() = 0;

  /// @brief オプションの説明文を返す．
  const char*
  opt_desc() const;

  /// @brief オプションの引数の記述を返す．
  /// @note デフォルトの実装では NULL を返す．
  virtual
  const char*
  arg_desc() const;

  /// @brief このオブジェクトが解析中に呼ばれていたら true を返す．
  bool
  is_specified() const;

  /// @brief このオブジェクトが呼ばれた回数を返す．
  int
  count() const;

  /// @brief オプションが指定されたときに呼び出される関数
  /// @retval true 正常に処理が行なわれた．
  /// @retval false 続行不可能なエラーが起こった．
  /// @note デフォルトではなにもしないで true を返す．
  virtual
  bool
  action();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オプション文字列
  const char* mOptStr;

  // オプションを表す文字
  char mOptChar;

  // オプションの説明文
  const char* mOptDesc;

  // このオプションが指定された回数
  ymuint32 mCount;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptNone PoptMainApp.h "utils/PoptMainApp.h"
/// @brief 引数をとらないオプションの処理を行なうオブジェクトの基底クラス
//////////////////////////////////////////////////////////////////////
class PoptNone :
  public Popt
{
public:

  /// @brief コンストラクタ
  /// @param[in] opt_str オプション文字列
  /// @param[in] opt_char オプション文字
  /// @param[in] opt_desc オプションの説明文
  PoptNone(const char* opt_str,
	   char opt_char,
	   const char* opt_desc);

  /// @brief デストラクタ
  virtual
  ~PoptNone();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief argInfo の値を返す．
  virtual
  int
  arg_info();

  /// @brief arg の値を返す．
  virtual
  void*
  arg();

};


//////////////////////////////////////////////////////////////////////
/// @class PoptArg PoptMainApp.h "utils/PoptMainApp.h"
/// @brief 引数をとるオプションの処理を行なうオブジェクトの基底クラス
//////////////////////////////////////////////////////////////////////
class PoptArg :
  public Popt
{
public:

  /// @brief コンストラクタ
  /// @param[in] opt_str オプション文字列
  /// @param[in] opt_char オプション文字
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc 引数の説明文
  PoptArg(const char* opt_str,
	  char opt_char,
	  const char* opt_desc,
	  const char* arg_desc);

  /// @brief デストラクタ
  virtual
  ~PoptArg();


public:
  //////////////////////////////////////////////////////////////////////
  // Popt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプションの引数の記述を返す．
  virtual
  const char*
  arg_desc() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 引数の説明文
  const char* mArgDesc;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptStr PoptMainApp.h "utils/PoptMainApp.h"
/// @brief 文字列を引数にとるオプションの処理を行なうオブジェクト
//////////////////////////////////////////////////////////////////////
class PoptStr :
  public PoptArg
{
public:

  /// @brief コンストラクタ
  /// @param[in] opt_str オプション文字列
  /// @param[in] opt_char オプション文字
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc 引数の説明文
  PoptStr(const char* opt_str,
	  char opt_char,
	  const char* opt_desc,
	  const char* arg_desc);

  /// @brief デストラクタ
  virtual
  ~PoptStr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief arg_info の値を返す．
  virtual
  int
  arg_info();

  /// @brief arg の値を返す．
  virtual
  void*
  arg();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  string
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オプション引数を格納する変数
  char* mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptInt PoptMainApp.h "utils/PoptMainApp.h"
/// @brief 整数を引数にとるオプションを処理するクラス
//////////////////////////////////////////////////////////////////////
class PoptInt :
  public PoptArg
{
public:

  /// @brief コンストラクタ
  /// @param[in] opt_str オプション文字列
  /// @param[in] opt_char オプション文字
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc 引数の説明文
  PoptInt(const char* opt_str,
	  char opt_char,
	  const char* opt_desc,
	  const char* arg_desc);

  /// @brief デストラクタ
  virtual
  ~PoptInt();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief argInfo の値を返す．
  virtual
  int
  arg_info();

  /// @brief arg の値を返す．
  virtual
  void*
  arg();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  ymint
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オプション引数を格納する変数
  ymint mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptBool PoptMainApp.h "utils/PoptMainApp.h"
/// @brief ブール値を引数にとるオプションを処理するクラス
//////////////////////////////////////////////////////////////////////
class PoptBool :
  public PoptInt
{
public:

  /// @brief コンストラクタ
  /// @param[in] opt_str オプション文字列
  /// @param[in] opt_char オプション文字
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc 引数の説明文
  PoptBool(const char* opt_str,
	   char opt_char,
	   const char* opt_desc,
	   const char* arg_desc);

  /// @brief デストラクタ
  virtual
  ~PoptBool();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  bool
  val() const;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptUint PoptMainApp.h "utils/PoptMainApp.h"
/// @brief 符号なし整数を引数にとるオプションを処理するクラス
//////////////////////////////////////////////////////////////////////
class PoptUint :
  public PoptInt
{
public:

  /// @brief コンストラクタ
  /// @param[in] opt_str オプション文字列
  /// @param[in] opt_char オプション文字
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc 引数の説明文
  PoptUint(const char* opt_str,
	   char opt_char,
	   const char* opt_desc,
	   const char* arg_desc);

  /// @brief デストラクタ
  virtual
  ~PoptUint();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  ymuint
  val() const;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptFloat PoptMainApp.h "utils/PoptMainApp.h"
/// @brief 単精度浮動小数点数を引数にとるオプションを処理するクラス
//////////////////////////////////////////////////////////////////////
class PoptFloat :
  public PoptArg
{
public:

  /// @brief コンストラクタ
  /// @param[in] opt_str オプション文字列
  /// @param[in] opt_char オプション文字
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc 引数の説明文
  PoptFloat(const char* opt_str,
	    char opt_char,
	    const char* opt_desc,
	    const char* arg_desc);

  /// @brief デストラクタ
  virtual
  ~PoptFloat();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief argInfo の値を返す．
  virtual
  int
  arg_info();

  /// @brief arg の値を返す．
  virtual
  void*
  arg();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  float
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オプション引数を格納する変数
  float mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptDouble PoptMainApp.h "utils/PoptMainApp.h"
/// @brief 倍精度浮動小数点数を引数にとるオプションを処理するクラス
//////////////////////////////////////////////////////////////////////
class PoptDouble :
  public PoptArg
{
public:

  /// @brief コンストラクタ
  /// @param[in] opt_str オプション文字列
  /// @param[in] opt_char オプション文字
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc 引数の説明文
  PoptDouble(const char* opt_str,
	     char opt_char,
	     const char* opt_desc,
	     const char* arg_desc);

  /// @brief デストラクタ
  virtual
  ~PoptDouble();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief argInfo の値を返す．
  virtual
  int
  arg_info();

  /// @brief arg の値を返す．
  virtual
  void*
  arg();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  double
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オプション引数を格納する変数
  double mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptMainApp PoptMainApp.h "utils/PoptMainApp.h"
/// @brief poptContext を用いる main 関数のクラス
//////////////////////////////////////////////////////////////////////
class PoptMainApp
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前 (alias 用)
  /// @param[in] auto_help --help オプションを有効にするフラグ
  PoptMainApp(const char* name = NULL,
	      bool auto_help = true);

  /// @brief デストラクタ
  ~PoptMainApp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプションを追加する．
  /// @param[in] option 追加するオプション
  void
  add_option(Popt* option);

  /// @brief ヘルプ文字列を指定する．
  /// @param[in] text ヘルプ文字列
  void
  set_other_option_help(const char* text);

  /// @brief poptContext をリセットする．
  void
  reset();

  /// @brief オプション解析を行なう．
  /// @param[in] argc コマンド行の引数の数
  /// @param[in] argv コマンド行の引数配列
  /// @param[in] flags フラグ
  tPoptStat
  parse_options(int argc,
		const char** argv,
		int flags);

  /// @brief 残った引数を得る．
  /// @param[in] args 引数を格納するベクタ
  /// @return 引数の数を返す．
  ymuint
  get_args(vector<string>& args);

  /// @brief ヘルプメッセージを出力する．
  /// @param[in] fp FILE 構造体へのポインタ(古！)
  /// @param[in] flags フラグ(現時点では未使用)
  void
  print_help(FILE* fp,
	     int flags);

  /// @brief ユーセージ(ショートヘルプ)メッセージを出力する．
  /// @param[in] fp FILE 構造体へのポインタ(古！)
  /// @param[in] flags フラグ(現時点では未使用)
  void
  print_usage(FILE* fp,
	      int flags);

  /// @brief usage を出力して終了する．
  void
  usage(int exitcode,
	const char* error = NULL,
	const char* addl = NULL);

  /// @brief PoptMainApp 用の strerror() 関数
  /// @param[in] error エラーコード
  static
  const char*
  popt_strerror(const int error);

  /// @brief エラーが起きた場合にそのもととなったオプション文字列を返す．
  /// @param[in] flags フラグ
  /// @note フラグに使用可能な値は以下のとおり
  ///  - 0 なにもしない．
  ///  - POPT_BADOPTION_NOALIAS もっとも外側(outermost)のオプションを返す．
  const char*
  bad_option(int flags);

  /// @brief alias 用のデフォルト設定を読み込む．
  /// @param[in] flags フラグ(現時点では未使用)
  /// @return 返り値に関しては man popt に記載なし．
  int
  read_default_config(int flags);

  /// @brief alias 用の設定ファイルを読み込む．
  /// @param[in] filename ファイル名
  int
  read_config_file(const char* filename);

  /// @brief alias を追加する．
  /// @param[in] alias alias 構造体
  /// @param[in] flags フラグ(現時点では未使用)
  int
  add_alias(struct poptAlias alias,
	    int flags);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // エイリアスで用いる(らしい)名前
  const char* mName;

  // AUTOHELP オプションを有効にするとき true にするフラグ
  bool mAutoHelp;

  // 処理を行なったときに true となるフラグ
  // popt の処理は1度しか行なえない．
  bool mDone;

  // poptContext を生成するのに用いるオプションテーブル
  poptOption* mOptionTable;

  // poptContext 本体
  poptContext mCon;

  // ヘルプ文字列
  const char* mHelpText;

  // オプションオブジェクトのリスト
  vector<Popt*> mOptionList;

};

END_NAMESPACE_YM

#endif // UTILS_POPTMAINAPP_H
