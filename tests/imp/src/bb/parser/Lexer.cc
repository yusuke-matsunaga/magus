
/// @file src/Lexer.cc
/// @brief Lexer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Lexer.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lexer.h"
#include "Driver.h"


BEGIN_NAMESPACE_YM_BB

class PtNode;

#include "bb_grammer.hh"

// コンストラクタ
Lexer::Lexer(MsgMgr& msg_mgr) :
  mMsgMgr(msg_mgr)
{
  init();
}

// デストラクタ
Lexer::~Lexer()
{
  close_file();
}

// ファイルをセットする．
// 自動的にに clear() を呼ぶ．
// ファイルのオープンに失敗したら false を返す．
bool
Lexer::open_file(const string& filename)
{
  init();
  mInput.open(filename.c_str());
  if ( !mInput ) {
    return false;
  }
  mFileInfo = FileInfo(filename);

  return true;
}

// ファイルをクローズする．
void
Lexer::close_file()
{
  mInput.close();
}

// 初期化
void
Lexer::init()
{
  mUngetChar = 0;
  mCR = false;
  mCurLine = 1;
  mCurColumn = 0;
}

// トークンを一つとってくる．
// 仕様
// - [0-9]+ からなる文字列を整数値として認識する．
// - [a-zA-Z_][a-zA-Z0-9_]* からなる文字列を識別子として認識する．
// - 識別子が予約語かどうかはここでは判定しない．
// - /* 〜 */ で囲まれた部分は読み飛ばす．
// - // から改行までを読みとばす．(本当の仕様ではない)
// - 改行は改行として扱う．
// - 空白，タブ，バックスラッシュ＋改行は区切り文字とみなす．
// - それ以外の文字はそのまま返す．
int
Lexer::read_token()
{
  int c;

  mCurString.clear();

 ST_INIT: // 初期状態
  c = get();
  mFirstColumn = mCurColumn;
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  if ( isalpha(c) || (c == '_') ) {
    mCurString.put_char(c);
    goto ST_ID;
  }

  switch (c) {
  case EOF:
    return EOF;

  case ' ':
  case '\t':
    goto ST_INIT; // 最初の空白は読み飛ばす．

  case '\n':
    nl();
    goto ST_INIT;

  case '\\':
    c = get();
    if ( c == '\n' ) {
      // 無視する．
      nl();
      goto ST_INIT;
    }
    // それ以外はバックスラッシュがなかったことにする．
    unget();
    goto ST_INIT;

  case ';':
    return SEMI;

  case '(':
    return LP;

  case ')':
    return RP;

  case '[':
    return LB;

  case ']':
    return RB;

  case ',':
    return COMMA;

  case '|':
    return BOR;

  case '&':
    return BAND;

  case '^':
    return BXOR;

  case '~':
    return TILDE;

  case '=':
    return EQ;

  case '+':
    return ADD;

  case '-':
    return SUB;

  case '*':
    return MUL;

  case '/':
    goto ST_COMMENT1;

  case '%':
    return MOD;

  case '<':
    goto ST_LT;

  case '>':
    goto ST_GT;

  case ':':
    goto ST_COLON;

  default:
    break;
  }
  return c;

 ST_NUM1: // 一文字目が[0-9]の時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  unget();
  return NUMBER;

 ST_ID: // 一文字目が[a-zA-Z_]の時
  c = get();
  if ( isalnum(c) || (c == '_') ) {
    mCurString.put_char(c);
    goto ST_ID;
  }
  unget();
  return ID;

 ST_LT: // '<' を読み込んだ直後
  c = get();
  if ( c == '<' ) {
    return LTLT;
  }
  else if ( c == '=' ) {
    return LE;
  }
  else {
    unget();
    return LT;
  }

 ST_GT: // '>' を読み込んだ直後
  c = get();
  if ( c == '>' ) {
    return GTGT;
  }
  else if ( c == '=' ) {
    return GE;
  }
  else {
    unget();
    return GT;
  }

 ST_COLON: // ':' を読み込んだ直後
  c = get();
  if ( c == '=' ) {
    return COLONEQ;
  }
  unget();
  return ERROR;

 ST_COMMENT1: // '/' を読み込んだ直後
  c = get();
  if ( c == '/' ) { // C++ スタイルのコメント
    goto ST_COMMENT2;
  }
  if ( c == '*' ) { // C スタイルのコメント
    goto ST_COMMENT3;
  }
  unget();
  return DIV;

 ST_COMMENT2: // 改行まで読み飛ばす．
  c = get();
  if ( c == '\n' ) {
    nl();
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return EOF;
  }
  goto ST_COMMENT2;

 ST_COMMENT3: // "/*" を読み込んだ直後
  c = get();
  if ( c == EOF ) {
    goto ST_EOF;
  }
  if ( c == '*' ) {
    goto ST_COMMENT4;
  }
  goto ST_COMMENT3;

 ST_COMMENT4: // "/* 〜 *" まで読み込んだ直後
  c = get();
  if ( c == EOF ) {
    goto ST_EOF;
  }
  if ( c == '/' ) {
    goto ST_INIT;
  }
  if ( c == '*' ) {
    goto ST_COMMENT4;
  }
  if ( c == '\n' ) {
    nl();
  }
  goto ST_COMMENT3;

 ST_EOF:
  {
    ostringstream buf;
    buf << "Unexpected end-of-file in comment block.";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_file_region(),
		    kMsgError,
		    "BB_LEXER",
		    buf.str());
  }
  return ERROR;
}

// 一文字読み出す．
int
Lexer::get()
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
Lexer::unget()
{
  mUngetChar = mLastChar;
  -- mCurColumn;
}

// 改行を読み込んだ時の処理
void
Lexer::nl()
{
  ++ mCurLine;
  mCurColumn = 0;
}

END_NAMESPACE_YM_BB
