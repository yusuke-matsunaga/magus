
/// @file libym_sat/DimacsScanner.cc
/// @brief DimacsScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DimacsScanner.cc 750 2007-06-30 09:09:58Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DimacsScanner.h"


BEGIN_NAMESPACE_YM_SAT

BEGIN_NONAMESPACE
const bool debug_read_token = false;
END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// DimacsScanner
//////////////////////////////////////////////////////////////////////

// コンストラクタ
DimacsScanner::DimacsScanner()
{
}

// デストラクタ
DimacsScanner::~DimacsScanner()
{
}

// @brief トークンの読込み
// @param[out] loc 対応するファイル上の位置情報を格納する変数
tToken
DimacsScanner::read_token(FileRegion& loc)
{
  tToken token = scan();
  loc = cur_loc();

  if ( debug_read_token ) {
    cerr << "read_token() --> "
	 << loc << ": ";
    switch ( token ) {
    case kC:    cerr << "C"; break;
    case kP:    cerr << "P"; break;
    case kNUM:  cerr << "NUM(" << mCurVal << ")"; break;
    case kZERO: cerr << "ZERO"; break;
    case kNL:   cerr << "NL"; break;
    case kEOF:  cerr << "EOF"; break;
    case kERR:  cerr << "ERR"; break;
    }
    cerr << endl;
  }

  return token;
}

// @brief read_token() の下請け関数
// @return トークンを返す．
tToken
DimacsScanner::scan()
{
  // トークンとは空白もしくは改行で区切られたもの
  // とりあえずそれ以外の文字はすべてトークンの構成要素とみなす．
  // 改行は単独のトークンとみなす．
  // EOF も単独のトークンとみなす．

  // 最初の空白文字をスキップする．
  int c;
  for ( ; ; ) {
    c = get();
    if ( c == EOF ) {
      return kEOF;
    }
    if ( c == '\n' ) {
      return kNL;
    }
    if ( c != ' ' && c != '\t' ) {
      break;
    }
  }

  if ( c == 'c' ) {
    // 手抜きで次の NL まで読み飛ばす．
    while ( (c = get()) != '\n' && c != EOF ) ;
    return kC;
  }
  if ( c == 'p' ) {
    c = get();
    // 次は空白でなければならない．
    if ( c != ' ' && c != '\t' ) {
      goto p_error;
    }
    // 続く空白を読み飛ばす．
    for ( ; ; ) {
      c = get();
      if ( c != ' ' && c != '\t' ) {
	break;
      }
    }
    // 次は "cnf" のはず．
    if ( c != 'c' ) {
      goto p_error;
    }
    c = get();
    if ( c != 'n' ) {
      goto p_error;
    }
    c = get();
    if ( c != 'f' ) {
      goto p_error;
    }
    c = get();
    // 次は空白のはず．
    if ( c != ' ' && c != '\t' ) {
      goto p_error;
    }
    return kP;

  p_error:
#if 0
    put_msg(__FILE__, __LINE__,
	    cur_loc(),
	    kMsgError,
	    "ERR05",
	    "syntax error");
#endif
    return kERR;
  }

  bool minus_flag = false;
  if ( c == '-' ) {
    minus_flag = true;
    c = get();
  }

  int val = 0;
  for ( ; ; ) {
    if ( c < '0' || '9' < c ) {
#if 0
      put_msg(__FILE__, __LINE__,
	      cur_loc(),
	      kMsgError,
	      "ERR06",
	      "syntax error");
#endif
      return kERR;
    }
    val = val * 10 + (c - '0');

    c = peek();
    if ( c == ' ' || c == '\t' || c == '\n' || c == EOF ) {
      if ( minus_flag ) {
	mCurVal = - val;
      }
      else {
	mCurVal = val;
      }
      if ( mCurVal == 0 ) {
	return kZERO;
      }
      return kNUM;
    }
    accept();
  }
}

END_NAMESPACE_YM_SAT
