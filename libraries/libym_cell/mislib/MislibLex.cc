
/// @file libym_cell/mislib/MislibLex.cc
/// @brief MislibLex の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibLex.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibLex.h"
#include "ym_utils/FileInfoMgr.h"


BEGIN_NAMESPACE_YM_CELL_MISLIB

#include "mislib_grammer.h"


// コンストラクタ
MislibLex::MislibLex(MsgMgr& msg_mgr) :
  mMsgMgr(msg_mgr)
{
}

// デストラクタ
MislibLex::~MislibLex()
{
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @return 失敗したら false を返す．
bool
MislibLex::open_file(const string& filename)
{
  bool stat = mFileScanner.open_file(filename);
  if ( !stat ) {
    return false;
  }
  return true;
}

// トークンを一つとってくる．
int
MislibLex::read_token()
{
  int c;

  mCurString.clear();

 state1:
  c = get();
  mFileScanner.set_first_loc();
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
    mMsgMgr.put_msg(__FILE__, __LINE__,
		    cur_loc(), kMsgError,
		    "MISLIB_PARSE", buf.str().c_str());
  }
  return ERROR;
}

END_NAMESPACE_YM_CELL_MISLIB
