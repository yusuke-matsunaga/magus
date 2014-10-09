
/// @file Iscas89Scanner.cc
/// @brief Iscas89Scanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89Scanner.h"


BEGIN_NONAMESPACE

// read_token() の動作をデバッグするときに true にする．
const bool debug_read_token = false;

END_NONAMESPACE

BEGIN_NAMESPACE_YM_ISCAS89

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
Token
Iscas89Scanner::read_token(FileRegion& loc)
{
  Token token = scan();
  loc = cur_loc();

  if ( debug_read_token ) {
    cerr << "read_token()" << " --> "
	 << loc << ": ";
    switch ( token ) {
    case kToken_LPAR:   cerr << "("; break;
    case kToken_RPAR:   cerr << ")"; break;
    case kToken_EQ:     cerr << "="; break;
    case kToken_COMMA:  cerr << ","; break;
    case kToken_INPUT:  cerr << "INPUT"; break;
    case kToken_OUTPUT: cerr << "OUTPUT"; break;
    case kToken_BUFF:   cerr << "BUFF"; break;
    case kToken_NOT:    cerr << "NOT"; break;
    case kToken_AND:    cerr << "AND"; break;
    case kToken_NAND:   cerr << "NAND"; break;
    case kToken_OR:     cerr << "OR"; break;
    case kToken_NOR:    cerr << "NOR"; break;
    case kToken_XOR:    cerr << "XOR"; break;
    case kToken_XNOR:   cerr << "XNOR"; break;
    case kToken_DFF:    cerr << "DFF"; break;
    case kToken_NAME:   cerr << "NAME(" << cur_string() << ")"; break;
    case kToken_EOF:    cerr << "EOF"; break;
    default:            cerr << static_cast<char>(token); break;
    }
    cerr << endl;
  }

  return token;
}

// @brief read_token() の下請け関数
// @return トークンを返す．
Token
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
    return kToken_EOF;

  case ' ':
  case '\t':
  case '\n':
    // ホワイトスペースは読み飛ばす．
    goto ST_INIT;

  case '#':
    goto ST_SHARP;

  case '=':
    return kToken_EQ;

  case '(':
    return kToken_LPAR;

  case ')':
    return kToken_RPAR;

  case ',':
    return kToken_COMMA;

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
    return kToken_EOF;
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
      return kToken_INPUT;
    }
    if ( mCurString == "OUTPUT" ) {
      return kToken_OUTPUT;
    }
    if ( mCurString == "BUFF" ) {
      return kToken_BUFF;
    }
    if ( mCurString == "NOT" ) {
      return kToken_NOT;
    }
    if ( mCurString == "INV" ) {
      return kToken_NOT;
    }
    if ( mCurString == "AND" ) {
      return kToken_AND;
    }
    if ( mCurString == "NAND" ) {
      return kToken_NAND;
    }
    if ( mCurString == "OR" ) {
      return kToken_OR;
    }
    if ( mCurString == "NOR" ) {
      return kToken_NOR;
    }
    if ( mCurString == "XOR" ) {
      return kToken_XOR;
    }
    if ( mCurString == "XNOR" ) {
      return kToken_XNOR;
    }
    if ( mCurString == "DFF" ) {
      return kToken_DFF;
    }
    return kToken_NAME;

  default:
    accept();
    mCurString.put_char(c);
    goto ST_STR;
  }
}

END_NAMESPACE_YM_ISCAS89
