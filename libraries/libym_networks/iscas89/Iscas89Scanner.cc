
/// @file Iscas89Scanner.cc
/// @brief Iscas89Scanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89Scanner.h"


BEGIN_NONAMESPACE

// read_token() の動作をデバッグするときに true にする．
const bool debug_read_token = false;

END_NONAMESPACE

BEGIN_NAMESPACE_YM_ISCAS89

#include "iscas89_grammer.hh"


//////////////////////////////////////////////////////////////////////
// iscas89 用の字句解析器
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ido 入力データ
Iscas89Scanner::Iscas89Scanner(IDO& ido) :
  Scanner(ido)
{
}

// @brief デストラクタ
Iscas89Scanner::~Iscas89Scanner()
{
}

// @brief トークンを一つ読み出す．
// @param[out] loc トークンの位置を格納する変数
int
Iscas89Scanner::read_token(FileRegion& loc)
{
  int token = scan();
  loc = cur_loc();

  if ( debug_read_token ) {
    cerr << "read_token()" << " --> "
	 << loc << ": ";
    switch ( token ) {
    case INPUT:  cerr << "INPUT"; break;
    case OUTPUT: cerr << "OUTPUT"; break;
    case BUFF:   cerr << "BUFF"; break;
    case NOT:    cerr << "NOT"; break;
    case AND:    cerr << "AND"; break;
    case NAND:   cerr << "NAND"; break;
    case OR:     cerr << "OR"; break;
    case NOR:    cerr << "NOR"; break;
    case XOR:    cerr << "XOR"; break;
    case XNOR:   cerr << "XNOR"; break;
    case DFF:    cerr << "DFF"; break;
    case NAME:   cerr << "NAME(" << cur_string() << ")"; break;
    case EOF:    cerr << "EOF"; break;
    default:     cerr << static_cast<char>(token); break;
    }
    cerr << endl;
  }

  return token;
}

// @brief read_token() の下請け関数
// @return トークンを返す．
int
Iscas89Scanner::scan()
{
  int c;

  mCurString = "";

  // 状態遷移を goto 文で表現したもの
  // 効率はよい．

 ST_INIT:
  c = get();
  set_first_loc();
  switch ( c ) {
  case EOF:
    return EOF;

  case ' ':
  case '\t':
  case '\n':
    // ホワイトスペースは読み飛ばす．
    goto ST_INIT;

  case '#':
    goto ST_SHARP;

  case '=':
    return '=';

  case '(':
    return '(';

  case ')':
    return ')';

  case ',':
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
      return INPUT;
    }
    if ( mCurString == "OUTPUT" ) {
      return OUTPUT;
    }
    if ( mCurString == "BUFF" ) {
      return BUFF;
    }
    if ( mCurString == "NOT" ) {
      return NOT;
    }
    if ( mCurString == "INV" ) {
      return NOT;
    }
    if ( mCurString == "AND" ) {
      return AND;
    }
    if ( mCurString == "NAND" ) {
      return NAND;
    }
    if ( mCurString == "OR" ) {
      return OR;
    }
    if ( mCurString == "NOR" ) {
      return NOR;
    }
    if ( mCurString == "XOR" ) {
      return XOR;
    }
    if ( mCurString == "XNOR" ) {
      return XNOR;
    }
    if ( mCurString == "DFF" ) {
      return DFF;
    }
    return NAME;

  default:
    accept();
    mCurString.put_char(c);
    goto ST_STR;
  }
}

END_NAMESPACE_YM_ISCAS89
