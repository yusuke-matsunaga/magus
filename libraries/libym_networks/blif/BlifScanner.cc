
/// @file BlifScanner.cc
/// @brief BlibScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BlifScanner.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

BEGIN_NONAMESPACE

// read_token() の動作をデバッグするときに true にする．
const bool debug_read_token = false;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// blif 用の字句解析器
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifScanner::BlifScanner()
{
}

// @brief デストラクタ
BlifScanner::~BlifScanner()
{
}

// @brief トークンを一つ読み出す．
// @param[out] loc トークンの位置を格納する変数
tToken
BlifScanner::read_token(FileRegion& loc)
{
  tToken token = scan();
  loc = cur_loc();

  if ( debug_read_token ) {
    cerr << "read_token()" << " --> "
	 << loc << ": "
	 << token;
    if ( token == kTokenSTRING ) {
      cerr << "(\'" << cur_string() << "\')";
    }
    cerr<< endl;
  }

  return token;
}

// @brief read_token() の下請け関数
// @return トークンを返す．
tToken
BlifScanner::scan()
{
  mCurString = "";
  bool StartWithDot = false;
  int c;

  // 状態遷移を goto 文で表現したもの
  // 効率はよい．

 ST_INIT:
  c = get();
  set_first_loc();

  switch ( c ) {
  case EOF:
    return kTokenEOF;

  case ' ':
  case '\t':
    // ホワイトスペースは読み飛ばす．
    goto ST_INIT;

  case '\n':
    return kTokenNL;

  case '=':
    return kTokenEQ;

  case '.':
    StartWithDot = true;
    goto ST_STR;

  case '#':
    goto ST_SHARP;

  case '/':
    if ( peek() == '*' ) {
      accept();
      // ここまでで "/*" を読んでいる．
      goto ST_CM1;
    }
    mCurString.put_char('/');
    goto ST_STR;

  case '\\':
    goto ST_ESC;

  default:
    mCurString.put_char(c);
    goto ST_STR;
  }

 ST_SHARP:
  c = get();
  if ( c == '\n' ) {
    return kTokenNL;
  }
  if ( c == EOF ) {
    return kTokenEOF;
  }
  // 改行までは読み飛ばす．
  goto ST_SHARP;

 ST_CM1:
  c = get();
  if ( c == '*' ) {
    goto ST_CM2;
  }
  if ( c == EOF ) {
    return kTokenEOF;
  }
  // '*' までは読み飛ばす．
  goto ST_CM1;

 ST_CM2:
  c = get();
  if ( c == '/' ) {
    // コメントは空白扱いにする．
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return kTokenEOF;
  }
  goto ST_CM1;

 ST_ESC:
  c = get();
  if ( c == '\n' ) {
    // エスケープされた改行は空白扱いにする．
    goto ST_INIT;
  }
  if ( c == EOF ) {
    // これはおかしいけど無視する．
    return check_word(StartWithDot);
  }
  // それ以外は普通の文字として扱う．
  mCurString.put_char(c);
  goto ST_STR;

 ST_STR:
  c = peek();
  switch ( c ) {
  case ' ':
  case '\t':
  case '\n':
  case '=':
  case '#':
  case '\\':
  case EOF:
    // 文字列の終わり
    return check_word(StartWithDot);

  default:
    accept();
    mCurString.put_char(c);
    goto ST_STR;
  }
}

// @brief 予約後の検査をする．
// @param[in] start_with_dot '.' で始まっている時に true を渡す．
// @return トークンを返す．
tToken
BlifScanner::check_word(bool start_with_dot)
{
  if ( start_with_dot ) {
    // 予約後の検索
    tToken token = mDic.get_token(cur_string());
    if ( token != kTokenEOF ) {
      return token;
    }
  }
  return kTokenSTRING;
}

END_NAMESPACE_YM_NETWORKS_BLIF
