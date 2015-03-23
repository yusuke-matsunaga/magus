
/// @file DotlibScanner.cc
/// @brief DotlibScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibScanner.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

// @brief コンストラクタ
// @param[in] ido 入力データ
DotlibScanner::DotlibScanner(IDO& ido) :
  Scanner(ido)
{
}

// デストラクタ
DotlibScanner::~DotlibScanner()
{
}

// @brief 直前の read_token() に対応する整数値を返す．
// @note 型が INT_NUM でなかったときの値は不定
int
DotlibScanner::cur_int() const
{
  return strtol(cur_string(), NULL, 10);
}

// @brief 直前の read_token() に対応する実数値を返す．
// @note 型が FLOAT_NUM/INT_NUM でなかったときの値は不定
double
DotlibScanner::cur_float() const
{
  return strtod(cur_string(), NULL);
}

// @brief トークンを一つとってくる．
// @param[out] loc ファイル上の位置情報を格納する変数
// @param[in] symbol_mode 数字も文字とみなすモード
tTokenType
DotlibScanner::read_token(FileRegion& loc,
			  bool symbol_mode)
{
  mSymbolMode = symbol_mode;
  tTokenType type = scan();
  loc = cur_loc();
  return type;
}

// @brief read_token() の下請け関数
// @return トークンの型を返す．
tTokenType
DotlibScanner::scan()
{
  int c;

  mCurString.clear();

 ST_INIT: // 初期状態
  c = get();
  set_first_loc();
  if ( is_symbol(c) ) {
    mCurString.put_char(c);
    goto ST_ID;
  }
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM1;
  }

  switch (c) {
  case '.':
    mCurString.put_char(c);
    goto ST_DOT;

  case EOF:
    return END;

  case ' ':
  case '\t':
    goto ST_INIT; // 最初の空白は読み飛ばす．

  case '\n':
    return NL;

  case '\"':
    goto ST_DQ;

  case '\\':
    c = peek();
    if ( c == '\n' ) {
      // 無視する．
      accept();
      goto ST_INIT;
    }
    // それ以外はバックスラッシュがなかったことにする．
    goto ST_INIT;

  case '/':
    goto ST_COMMENT1;

  case ':':
    return COLON;

  case ';':
    return SEMI;

  case ',':
    return COMMA;

  case '+':
    return PLUS;

  case '-':
    goto ST_MINUS;

  case '*':
    return MULT;

  case '(':
    return LP;

  case ')':
    return RP;

  case '{':
    return LCB;

  case '}':
    return RCB;

  default:
    // それ以外はエラーなんじゃない？
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    "syntax error");
    return ERROR;
  }
  ASSERT_NOT_REACHED;

 ST_MINUS: // '-' を読み込んだ時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char('-');
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  return MINUS;

 ST_NUM1: // 一文字目が[0-9]の時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  if ( c == '.' ) {
    accept();
    mCurString.put_char(c);
    goto ST_DOT;
  }
  return INT_NUM;

 ST_DOT: // [0-9]*'.' を読み込んだ時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM2;
  }
  { // '.' の直後はかならず数字
    ostringstream buf;
    buf << "digit number expected after dot";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }

 ST_NUM2: // [0-9]*'.'[0-9]* を読み込んだ時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM2;
  }
  if ( c == 'e' || c == 'E' ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM3;
  }
  return FLOAT_NUM;

 ST_NUM3: // [0-9]*'.'[0-9]*(e|E)を読み込んだ時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  if ( c == '+' || c == '-' ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  { // (e|E) の直後はかならず数字か符号
    ostringstream buf;
    buf << "exponent value expected";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }

 ST_NUM4: // [0-9]*'.'[0-9]*(e|E)(+|-)?[0-9]*を読み込んだ直後
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  return FLOAT_NUM;

 ST_ID: // 一文字目が[a-zA-Z_]の時
  c = peek();
  if ( is_symbol(c) || isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_ID;
  }
  return SYMBOL;

 ST_DQ: // "があったら次の"までを強制的に文字列だと思う．
  c = get();
  if ( c == '\"' ) {
    return SYMBOL;
  }
  if ( c == '\n' ) {
    ostringstream buf;
    buf << "unexpected newline in quoted string.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }
  if ( c == EOF ) {
    ostringstream buf;
    buf << "unexpected end-of-file in quoted string.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }
  if ( c == '\\' ) {
    c = get();
    if ( c != '\n' ) {
      // 改行以外はバックスラッシュをそのまま解釈する．
      mCurString.put_char('\\');
    }
  }
  mCurString.put_char(c);
  goto ST_DQ;

 ST_COMMENT1: // '/' を読み込んだ直後
  c = peek();
  if ( c == '/' ) { // C++ スタイルのコメント
    accept();
    goto ST_COMMENT2;
  }
  if ( c == '*' ) { // C スタイルのコメント
    accept();
    goto ST_COMMENT3;
  }
  return DIV;

 ST_COMMENT2: // 改行まで読み飛ばす．
  c = get();
  if ( c == '\n' ) {
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return END;
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
  goto ST_COMMENT3;

 ST_EOF:
  {
    ostringstream buf;
    buf << "Unexpected end-of-file in comment block.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
  }
  return ERROR;
}

// @brief c が文字の時に true を返す．
// @note mSymbolMode が true なら数字も文字とみなす．
bool
DotlibScanner::is_symbol(int c)
{
  if ( isalpha(c) || c == '_' ) {
    return true;
  }
  if ( mSymbolMode ) {
    if ( isdigit(c) || c == '.' ) {
      return true;
    }
  }
  return false;
}

END_NAMESPACE_YM_DOTLIB
