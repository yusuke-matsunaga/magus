
/// @file libym_bnet/Iscas89Scanner.cc
/// @brief Iscas89Scanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Iscas89Scanner.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89Scanner.h"


BEGIN_NAMESPACE_YM_ISCAS89

#include "iscas89_grammer.h"

// get_token() の動作をデバッグするときに true にする．
static
const bool debug_get_token = false;


//////////////////////////////////////////////////////////////////////
// iscas89 用の字句解析器
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Iscas89Scanner::Iscas89Scanner()
{
}

// @brief デストラクタ
Iscas89Scanner::~Iscas89Scanner()
{
}

// @brief トークンを一つ読み出す．
int
Iscas89Scanner::get_token()
{
  if ( debug_get_token ) {
    cerr << "get_token()" << " --> ";
  }

  mCurString = "";
  int c;

  // 状態遷移を goto 文で表現したもの
  // 効率はよい．

 ST_INIT:
  c = get();
  set_first_loc();
  switch ( c ) {
  case EOF:
    if ( debug_get_token ) {
      cerr << "EOF" << endl;
    }
    return EOF;

  case ' ':
  case '\t':
  case '\n':
    // ホワイトスペースは読み飛ばす．
    goto ST_INIT;

  case '#':
    goto ST_SHARP;

  case '=':
    if ( debug_get_token ) {
      cerr << "=" << endl;
    }
    return '=';

  case '(':
    if ( debug_get_token ) {
      cerr << "(" << endl;
    }
    return '(';

  case ')':
    if ( debug_get_token ) {
      cerr << ")" << endl;
    }
    return ')';

  case ',':
    if ( debug_get_token ) {
      cerr << "," << endl;
    }
    return ',';

  default:
    mCurString.put_char(c);
    goto ST_STR;
  }

 ST_SHARP: // 1行コメントの始まり
  c = get();
  if ( c == '\n' ) {
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return EOF;
  }
  // 改行までは読み飛ばす．
  goto ST_SHARP;

 ST_STR:
  c = peek();
  switch ( c ) {
  case ' ':
  case '\t':
  case '\n':
  case '#':
  case '=':
  case '(':
  case ')':
  case ',':
  case EOF:
    // 文字列の終わり

    // 予約後の検索
    if ( mCurString == "INPUT" ) {
      if ( debug_get_token ) {
	cerr << "INPUT" << endl;
      }
      return INPUT;
    }
    if ( mCurString == "OUTPUT" ) {
      if ( debug_get_token ) {
	cerr << "OUTPUT" << endl;
      }
      return OUTPUT;
    }
    if ( mCurString == "BUFF" ) {
      if ( debug_get_token ) {
	cerr << "BUFF" << endl;
      }
      return BUFF;
    }
    if ( mCurString == "NOT" ) {
      if ( debug_get_token ) {
	cerr << "NOT" << endl;
      }
      return NOT;
    }
    if ( mCurString == "INV" ) {
      if ( debug_get_token ) {
	cerr << "INV" << endl;
      }
      return NOT;
    }
    if ( mCurString == "AND" ) {
      if ( debug_get_token ) {
	cerr << "AND" << endl;
      }
      return AND;
    }
    if ( mCurString == "NAND" ) {
      if ( debug_get_token ) {
	cerr << "NAND" << endl;
      }
      return NAND;
    }
    if ( mCurString == "OR" ) {
      if ( debug_get_token ) {
	cerr << "OR" << endl;
      }
      return OR;
    }
    if ( mCurString == "NOR" ) {
      if ( debug_get_token ) {
	cerr << "NOR" << endl;
      }
      return NOR;
    }
    if ( mCurString == "XOR" ) {
      if ( debug_get_token ) {
	cerr << "XOR" << endl;
      }
      return XOR;
    }
    if ( mCurString == "XNOR" ) {
      if ( debug_get_token ) {
	cerr << "XNOR" << endl;
      }
      return XNOR;
    }
    if ( mCurString == "DFF" ) {
      if ( debug_get_token ) {
	cerr << "DFF" << endl;
      }
      return DFF;
    }
    if ( debug_get_token ) {
      cerr << "NAME(\'" << mCurString << "\')" << endl;
    }
    return NAME;

  default:
    accept();
    mCurString.put_char(c);
    goto ST_STR;
  }
}

END_NAMESPACE_YM_ISCAS89
