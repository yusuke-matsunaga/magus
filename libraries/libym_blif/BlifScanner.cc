
/// @file libym_bnet/BlifScanner.cc
/// @brief BlibScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifScanner.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BlifScanner.h"


BEGIN_NAMESPACE_YM_BLIF

// get_token() の動作をデバッグするときに true にする．
static
const bool debug_get_token = false;


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
tToken
BlifScanner::get_token()
{
  if ( debug_get_token ) {
    cerr << "get_token()" << " --> ";
  }

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
    if ( debug_get_token ) {
      cerr << kTokenEOF << endl;
    }
    return kTokenEOF;

  case ' ':
  case '\t':
    // ホワイトスペースは読み飛ばす．
    goto ST_INIT;

  case '\n':
    if ( debug_get_token ) {
      cerr << kTokenNL << endl;
    }
    return kTokenNL;

  case '=':
    if ( debug_get_token ) {
      cerr << kTokenEQ << endl;
    }
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
    if ( StartWithDot ) {
      // 予約後の検索
      tToken token = mDic.get_token(mCurString.c_str());
      if ( token != kTokenEOF ) {
	if ( debug_get_token ) {
	  cerr << token << endl;
	}
	return token;
      }
    }
    if ( debug_get_token ) {
      cerr << kTokenSTRING << "(\'" << mCurString << "\')" << endl;
    }
    return kTokenSTRING;
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
    if ( StartWithDot ) {
      // 予約後の検索
      tToken token = mDic.get_token(mCurString.c_str());
      if ( token != kTokenEOF ) {
	if ( debug_get_token ) {
	  cerr << token << endl;
	}
	return token;
      }
    }
    if ( debug_get_token ) {
      cerr << kTokenSTRING << "(\'" << mCurString << "\')" << endl;
    }
    return kTokenSTRING;

  default:
    accept();
    mCurString.put_char(c);
    goto ST_STR;
  }
}

END_NAMESPACE_YM_BLIF
