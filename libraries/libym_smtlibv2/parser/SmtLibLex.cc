
/// @file libym_smtlibv2/parser/SmtLibLex.cc
/// @brief SmtLibLex の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SmtLibLex.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibLex.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

// コンストラクタ
SmtLibLex::SmtLibLex(MsgMgr& msg_mgr) :
  mMsgMgr(msg_mgr)
{
  init();
}

// デストラクタ
SmtLibLex::~SmtLibLex()
{
  close_file();
}

// ファイルをセットする．
// 自動的にに clear() を呼ぶ．
// ファイルのオープンに失敗したら false を返す．
bool
SmtLibLex::open_file(const string& filename)
{
  init();
  mInput.open(filename.c_str());
  if ( !mInput ) {
    return false;
  }
  mCurFileDesc = mFileDescMgr.new_file_desc(filename);
  return true;
}

// ファイルをクローズする．
void
SmtLibLex::close_file()
{
  mInput.close();
  mCurFileDesc = NULL;
}

// 初期化
void
SmtLibLex::init()
{
  mCurFileDesc = NULL;
  mUngetChar = 0;
  mCR = false;
  mCurLine = 1;
  mCurColumn = 0;
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
SmtLibLex::read_token()
{
  int c;

  mCurString.clear();

 ST_INIT: // 初期状態
  c = get();
  mFirstColumn = mCurColumn;
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
    nl();
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
  c = get();
  if ( c == '.' ) {
    mCurString.put_char(c);
    goto ST_DEC1;
  }
  unget();
  return kNumToken;

 ST_NUM1: // 一文字目が[1-9]の時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  if ( c == '.' ) {
    mCurString.put_char(c);
    goto ST_DEC1;
  }
  unget();
  return kNumToken;

 ST_DEC1: // 0|[1-9][0-9]*'.' を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_DEC2;
  }
  { // '.' の直後はかならず数字
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
		    kMsgError,
		    "SMTLIB_LEX",
		    "digit number expected after dot.");
    return kErrorToken;
  }

 ST_DEC2: // 0|[1-9][0-9]*'.'[0-9]* を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_DEC2;
  }
  unget();
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
  // 最低1文字は2進数を表すもじがなければならない．
  goto SYNTAX_ERROR;

 ST_BIN1: // "#b[01]+" を読み込んだ後
  c = get();
  if ( is_binchar(c) ) {
    mCurString.put_char(c);
    goto ST_BIN1;
  }
  unget();
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
  c = get();
  if ( is_hexchar(c) ) {
    mCurString.put_char(c);
    goto ST_HEX1;
  }
  unget();
  return kHexToken;

 ST_SYM1: // 一文字目が文字だった時
  c = get();
  if ( is_strdigitchar(c) ) {
    mCurString.put_char(c);
    goto ST_SYM1;
  }
  unget();
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
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
		    kMsgError,
		    "SMTLIB_LEX",
		    "newline inside double-quoted string.");
    return kErrorToken;
  }
  if ( c == EOF ) {
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
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
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
		    kMsgError,
		    "SMTLIB_LEX",
		    "baskslash inside quoted symbol.");
    return kErrorToken;
  }
  if ( c == EOF ) {
    // | ... | が終わる前に EOF が来た．
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
		    kMsgError,
		    "SMTLIB_LEX",
		    "unexpected end-of-file in quoted symbol.");
    return kErrorToken;
  }
  if ( c == '\n' ) {
    nl();
  }
  mCurString.put_char(c);
  goto ST_Q_SYM1;

 ST_KEY1: // 一文字目が : だった時
  c = get();
  if ( is_strdigitchar(c) ) {
    mCurString.put_char(c);
    goto ST_KEY1;
  }
  unget();
  return kKeywordToken;

 ST_COMMENT1: // ; を読み込んだ直後
  c = get();
  if ( c == '\n' ) {
    nl();
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return kEofToken;
  }
  goto ST_COMMENT1;

 SYNTAX_ERROR:
  mMsgMgr.put_msg(__FILE__, __LINE__,
		  cur_file_region(),
		  kMsgError,
		  "SMTLIB_LEX",
		  "syntax error");
  return kErrorToken;
}

// 一文字読み出す．
int
SmtLibLex::get()
{
  int c = 0;

  if ( mUngetChar != 0 ) {
    // 戻された文字があったらそれを返す．
    c = mUngetChar;
    mUngetChar = 0;
  }
  else {
    for ( ; ; ) {
      c = mInput.get();
      if ( c == EOF ) {
	break;
      }
      // Windows(DOS)/Mac/UNIX の間で改行コードの扱いが異なるのでここで
      // 強制的に '\n' に書き換えてしまう．
      // Windows : '\r', '\n'
      // Mac     : '\r'
      // UNIX    : '\n'
      // なので '\r' を '\n' に書き換えてしまう．
      // ただし次に本当の '\n' が来たときには無視するために
      // mCR を true にしておく．
      if ( c == '\r' ) {
	mCR = true;
	c = '\n';
	break;
      }
      if ( c == '\n' ) {
	if ( mCR ) {
	  // 直前に '\r' を読んで '\n' を返していたので今の '\n' を
	  // 無視する．これが唯一ループを回る条件
	  mCR = false;
	  continue;
	}
	break;
      }
      // 普通の文字の時はそのまま返す．
      mCR = false;
      break;
    }
  }
  ++ mCurColumn;
  mLastChar = c;
  return c;
}

// 一文字読み戻す．
void
SmtLibLex::unget()
{
  mUngetChar = mLastChar;
  -- mCurColumn;
}

// 改行を読み込んだ時の処理
void
SmtLibLex::nl()
{
  ++ mCurLine;
  mCurColumn = 0;
}

END_NAMESPACE_YM_SMTLIBV2
