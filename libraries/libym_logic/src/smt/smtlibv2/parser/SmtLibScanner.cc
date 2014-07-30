
/// @file SmtLibScanner.cc
/// @brief SmtLibScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibScanner.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

// コンストラクタ
// @brief コンストラクタ
// @param[in] ido 入力データ
SmtLibScanner::SmtLibScanner(IDO& ido) :
  Scanner(ido)
{
}

// デストラクタ
SmtLibScanner::~SmtLibScanner()
{
}


BEGIN_NONAMESPACE

// symbol/keyword などに用いることの
// できる文字の場合に true を返す．
inline
bool
is_strchar(int c)
{
  if ( isalpha(c) ||
       c == '~' ||
       c == '!' ||
       c == '@' ||
       c == '$' ||
       c == '%' ||
       c == '^' ||
       c == '&' ||
       c == '*' ||
       c == '_' ||
       c == '+' ||
       c == '=' ||
       c == '<' ||
       c == '>' ||
       c == '.' ||
       c == '?' ||
       c == '-' ) {
    return true;
  }
  else {
    return false;
  }
}

// is_strchar() に数字をプラスしたもの．
inline
bool
is_strdigitchar(int c)
{
  return isdigit(c) || is_strchar(c);
}

// 2進数を表す文字の時に true を返す．
inline
bool
is_binchar(int c)
{
  return c == '0' || c == '1';
}

// 16進数を表す文字の時に true を返す．
inline
bool
is_hexchar(int c)
{
  return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

END_NONAMESPACE

// トークンを一つとってくる．
// 仕様
// DIGIT      [0-9]
// DIGIT1     [1-9]
// BINCHAR    [01]
// HEXCHAR    [0-9a-fA-F]
// STRCHAR    [a-zA-Z~!@$%^&*_+=<>.?-]
//
// - 0|({DIGIT1}{DIGIT}*)                  NUM
// - (0|({DIGIT1}{DIGIT}*)).({DIGIT}+)     DEC
// - #b({BINCHAR}+)                        BIN
// - #x({HEXCHAR}+)                        HEX
// - "(any ASCII char)"                    STRING
// - {STRCHAR}(({DIGIT}|{STRCHAR})*)       S_SYM
// - |(any ASCII char)|                    Q_SYM
// - :({DIGIT}|{STRCHAR})*                 KEYWORD
//
// ; から改行まではコメントと見なし空白を返す．
// S_SYMが予約語かどうかはここでは判定しない．
// - 改行は改行として扱う．
// - 空白，タブ，バックスラッシュ＋改行は区切り文字とみなす．
tTokenType
SmtLibScanner::read_token(FileRegion& loc)
{
  tTokenType tok = scan();
  loc = cur_loc();
  return tok;
}

// @brief read_token() の下請け関数
tTokenType
SmtLibScanner::scan()
{
  int c;

  mCurString.clear();

 ST_INIT: // 初期状態
  c = get();
  set_first_loc();

  if ( c == '0' ) {
    mCurString.put_char(c);
    goto ST_ZERO;
  }
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  if ( is_strchar(c) ) {
    mCurString.put_char(c);
    goto ST_SYM1;
  }

  switch (c) {
  case EOF:
    return kEofToken;

  case ' ':
  case '\t':
    goto ST_INIT; // 最初の空白は読み飛ばす．

  case '\n':
    return kNlToken;

  case '#':
    goto ST_SHARP;

  case '\"':
    goto ST_DQ;

  case '|':
    // | は mCurString に入れない．
    goto ST_Q_SYM1;

  case ':':
    mCurString.put_char(c);
    goto ST_KEY1;

  case ';':
    goto ST_COMMENT1;

  case '(':
    return kLpToken;

  case ')':
    return kRpToken;

  default:
    // それ以外はエラー
    goto SYNTAX_ERROR;
  }
  assert_not_reached(__FILE__, __LINE__);

 ST_ZERO: // 一文字目が 0 の時
  c = peek();
  if ( c == '.' ) {
    accept();
    mCurString.put_char(c);
    goto ST_DEC1;
  }
  return kNumToken;

 ST_NUM1: // 一文字目が[1-9]の時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  if ( c == '.' ) {
    accept();
    mCurString.put_char(c);
    goto ST_DEC1;
  }
  return kNumToken;

 ST_DEC1: // 0|[1-9][0-9]*'.' を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_DEC2;
  }
  // '.' の直後はかならず数字
  MsgMgr::put_msg(__FILE__, __LINE__, cur_loc(),
		  kMsgError,
		  "SMTLIB_LEX",
		  "digit number expected after dot.");
  return kErrorToken;

 ST_DEC2: // 0|[1-9][0-9]*'.'[0-9]* を読み込んだ時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_DEC2;
  }
  return kDecToken;

 ST_SHARP: // 一文字目が # だった時
  c = get();
  if ( c == 'b' ) {
    goto ST_BIN;
  }
  if ( c == 'x' ) {
    goto ST_HEX;
  }
  // それ以外はエラー
  goto SYNTAX_ERROR;

 ST_BIN: // "#b" を読み込んだ後
  c = get();
  if ( is_binchar(c) ) {
    mCurString.put_char(c);
    goto ST_BIN1;
  }
  // 最低1文字は2進数を表す文字がなければならない．
  goto SYNTAX_ERROR;

 ST_BIN1: // "#b[01]+" を読み込んだ後
  c = peek();
  if ( is_binchar(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_BIN1;
  }
  return kBinToken;

 ST_HEX: // "#x" を読み込んだ後
  c = get();
  if ( is_hexchar(c) ) {
    mCurString.put_char(c);
    goto ST_HEX1;
  }
  // 最低1文字は16進数を表す文字がなければならない．
  goto SYNTAX_ERROR;

 ST_HEX1: // "#x[0-9a-fA-F]+" を読み込んだ後
  c = peek();
  if ( is_hexchar(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_HEX1;
  }
  return kHexToken;

 ST_SYM1: // 一文字目が文字だった時
  c = peek();
  if ( is_strdigitchar(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_SYM1;
  }
  return kSymbolToken;

 ST_DQ: // 一文字目が " だった時
  c = get();
  if ( c == '\\' ) {
    c = get();
    if ( c == '"' || c == '\\' ) {
      mCurString.put_char(c);
    }
  }
  if ( c == '"' ) {
    return kStringToken;
  }
  if ( c == '\n' ) {
    // " ... " の間に改行があった．
    MsgMgr::put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "SMTLIB_LEX",
		    "newline inside double-quoted string.");
    return kErrorToken;
  }
  if ( c == EOF ) {
    MsgMgr::put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "SMTLIB_LEX",
		    "unexpected end-of-file in double-quoted string.");
    return kErrorToken;
  }
  mCurString.put_char(c);
  goto ST_DQ;

 ST_Q_SYM1: // 一文字目が | だった時
  c = get();
  if ( c == '|' ) {
    return kSymbolToken;
  }
  if ( c == '\\' ) {
    // | ... | の間にバックスラッシュがあった．
    MsgMgr::put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "SMTLIB_LEX",
		    "baskslash inside quoted symbol.");
    return kErrorToken;
  }
  if ( c == EOF ) {
    // | ... | が終わる前に EOF が来た．
    MsgMgr::put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "SMTLIB_LEX",
		    "unexpected end-of-file in quoted symbol.");
    return kErrorToken;
  }
  mCurString.put_char(c);
  goto ST_Q_SYM1;

 ST_KEY1: // 一文字目が : だった時
  c = peek();
  if ( is_strdigitchar(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_KEY1;
  }
  return kKeywordToken;

 ST_COMMENT1: // ; を読み込んだ直後
  c = get();
  if ( c == '\n' ) {
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return kEofToken;
  }
  goto ST_COMMENT1;

 SYNTAX_ERROR:
  MsgMgr::put_msg(__FILE__, __LINE__,
		  cur_loc(),
		  kMsgError,
		  "SMTLIB_LEX",
		  "syntax error");
  return kErrorToken;
}

END_NAMESPACE_YM_SMTLIBV2
