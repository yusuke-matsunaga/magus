
/// @file MislibScanner.cc
/// @brief MislibScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibScanner.h"
#include "utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_MISLIB

// mislib_grammer.hh 中で 必要になる(bison-2.6)
class MislibParserImpl;

#include "mislib_grammer.hh"


// コンストラクタ
MislibScanner::MislibScanner(IDO& ido) :
  Scanner(ido)
{
}

// デストラクタ
MislibScanner::~MislibScanner()
{
}

// トークンを一つとってくる．
// @param[out] loc 対応するファイル上の位置を格納する変数
int
MislibScanner::read_token(FileRegion& loc)
{
  int token = scan();
  loc = cur_loc();
  return token;
}

// @brief read_token の下請け関数
// @return トークンを返す．
int
MislibScanner::scan()
{
  int c = 0;

  mCurString.clear();

 state1:
  c = get();
  set_first_loc();
  if ( isalpha(c) || (c == '_') ) {
    mCurString.put_char(c);
    goto state2;
  }
  if ( isdigit(c) || (c == '-') ) {
    mCurString.put_char(c);
    goto state3;
  }
  switch (c) {
  case EOF: return EOF;
  case ' ':
  case '\t': goto state1;
  case '\n': goto state1;
  case '.': mCurString.put_char(c); goto state4;
  case ';': return SEMI;
  case '=': return EQ;
  case '(': return LP;
  case ')': return RP;
  case '!': return NOT;
  case '*': return STAR;
  case '&': return STAR;
  case '+': return PLUS;
  case '|': return PLUS;
  case '^': return HAT;
  case '#': goto state6;
  case '\"': goto state7;
  }
  goto error;

  // 一文字目が[a-zA-Z_]の時
 state2:
  c = peek();
  if ( isalpha(c) || isdigit(c) || (c == '_') ) {
    accept();
    mCurString.put_char(c);
    goto state2;
  }

  // 予約語（？）の検査
  // 数が少ないのでナイーブな方法をとっている．
  if ( mCurString == "GATE" ) {
    return GATE;
  }
  if ( mCurString == "PIN" ) {
    return PIN;
  }
  if ( mCurString == "NONINV" ) {
    return NONINV;
  }
  if ( mCurString == "INV" ) {
    return INV;
  }
  if ( mCurString == "UNKNOWN" ) {
    return UNKNOWN;
  }
  if ( mCurString == "CONST0" ) {
    return CONST0;
  }
  if ( mCurString == "CONST1" ) {
    return CONST1;
  }
  return STR;

  // 一文字目が[0-9]の時
 state3:
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto state3;
  }
  if ( c == '.' ) {
    accept();
    mCurString.put_char(c);
    goto state5;
  }
  if ( c == 'E' || c == 'e' ) {
    accept();
    mCurString.put_char(c);
    goto state8;
  }
  return NUM;

  // 一文字目が"."の時
 state4:
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto state5;
  }
  goto error;

  // [0-9]*"."を読み終えたところ
 state5:
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto state5;
  }
  if ( c == 'E' || c == 'e' ) {
    accept();
    mCurString.put_char(c);
    goto state8;
  }
  return NUM;

  // [0-9]*"."[Ee]を読み終えたところ
 state8:
  c = peek();
  if ( c == '-' ) {
    accept();
    mCurString.put_char(c);
    goto state9;
  }
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto state9;
  }
  return NUM;

  // [0-9]*"."[Ee]-?を読み終えたところ
 state9:
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto state9;
  }
  return NUM;

  // '#'があったら改行までループする．
 state6:
  c = get();
  if ( c == '\n' ) {
    goto state1;
  }
  goto state6;

  // "があったら次の"までを強制的に文字列だと思う．
 state7:
  c = get();
  if ( c == '\"' ) {
    return STR;
  }
  mCurString.put_char(c);
  goto state7;

 error:
  {
    ostringstream buf;
    buf << "Syntax error: illegal input (" << mCurString << ").";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "MISLIB_PARSE",
		    buf.str().c_str());
  }
  return ERROR;
}

END_NAMESPACE_YM_MISLIB
