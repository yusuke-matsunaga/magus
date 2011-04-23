
/// @file libym_cell/dotlib/DotlibParser.cc
/// @brief DotlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotlibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibParser.h"
#include "LibraryHandler.h"
#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

// コンストラクタ
DotlibParser::DotlibParser() :
  mAlloc(4096),
  mLibraryHandler(new LibraryHandler(*this))
{
  init();
}

// デストラクタ
DotlibParser::~DotlibParser()
{
  close_file();
}

// @brief ファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] debug デバッグモード
// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
// @return パース木の根のノードを返す．
// @note エラーが起きたら NULL を返す．
PtNode*
DotlibParser::read_file(const string& filename,
			bool debug,
			bool allow_no_semi)
{
  mDebug = debug;
  mAllowNoSemi = allow_no_semi;

  if ( !open_file(filename) ) {
    return NULL;
  }

  bool error = false;
  tTokenType type;
  // 空行を読み飛ばす．
  for (type = _read_token(); type == NL; type = _read_token()) { }
  string name = cur_string();
  FileRegion loc = cur_loc();
  if ( type != SYMBOL || name != "library" ) {
    msg_mgr().put_msg(__FILE__, __LINE__, loc,
		      kMsgError,
		      "DOTLIB_PARSER",
		      "'library' keyword is expected "
		      "on the top of the structure");
    error = true;
    goto last;
  }
  if ( !mLibraryHandler->read_attr(Token(type, name, loc)) ) {
    error = true;
    goto last;
  }

  if ( !expect_nl() ) {
    error = true;
    goto last;
  }
  for ( ; ; ) {
    tTokenType type = _read_token();
    if ( type == END ) {
      break;
    }
    msg_mgr().put_msg(__FILE__, __LINE__, cur_loc(),
		      kMsgWarning,
		      "DOTLIB_PARSER",
		      "contents after library group are ignored.");
  }

last:
  close_file();

  if ( error ) {
    return NULL;
  }
  return mLibraryHandler->pt_node();
}

// @brief PtNode を生成する．
// @param[in] attr_token 属性名を表すトークン
// @param[in] value_token 値を表すトークン
PtNode*
DotlibParser::new_ptnode(Token attr_token,
			 Token value_token)
{
  void* p = mAlloc.get_memory(sizeof(PtNode));
  return new (p) PtNode(attr_token, value_token);
}

// @brief PtNode を生成する．
// @param[in] attr_token 属性名を表すトークン
// @param[in] value_list 値を表すトークンのリスト
PtNode*
DotlibParser::new_ptnode(Token attr_token,
			 const vector<Token>& value_list)
{
  void* p = mAlloc.get_memory(sizeof(PtNode));
  return new (p) PtNode(attr_token, value_list);
}

// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
bool
DotlibParser::expect(tTokenType type)
{
  Token token = read_token();
  if ( token.type() == type ) {
    return true;
  }

  const char* type_str = NULL;
  switch ( type ) {
  case COLON:     type_str = "':'"; break;
  case SEMI:      type_str = "';'"; break;
  case COMMA:     type_str = "','"; break;
  case PLUS:      type_str = "'+'"; break;
  case MINUS:     type_str = "'-'"; break;
  case MULT:      type_str = "'*'"; break;
  case DIV:       type_str = "'/'"; break;
  case LP:        type_str = "'('"; break;
  case RP:        type_str = "')'"; break;
  case LCB:       type_str = "'{'"; break;
  case RCB:       type_str = "'}'"; break;
  case SYMBOL:    type_str = "STR"; break;
  case INT_NUM:   type_str = "INT"; break;
  case FLOAT_NUM: type_str = "FLOAT"; break;
  case EXPRESSION: type_str = "EXPRESSION"; break;
  case NL:        type_str = "new-line"; break;
  case ERROR:     assert_not_reached(__FILE__, __LINE__);
  case END:       assert_not_reached(__FILE__, __LINE__);
  }
  ostringstream buf;
  buf << "syntax error. " << type_str << " is expected.";
  mMsgMgr.put_msg(__FILE__, __LINE__, token.loc(),
		  kMsgError,
		  "DOTLIB_PARSER",
		  buf.str());
  return false;
}

// @brief 行末まで読み込む．
bool
DotlibParser::expect_nl()
{
  if ( mAllowNoSemi ) {
    Token token = read_token();
    if ( token.type() == SEMI ) {
      token = read_token();
    }
    if ( token.type() == NL || token.type() == END ) {
      return true;
    }
    ostringstream buf;
    mMsgMgr.put_msg(__FILE__, __LINE__, token.loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. Semicolon is expected.");
    return false;
  }
  else {
    if ( !expect(SEMI) ) {
      return false;
    }
    if ( !expect(NL) ) {
      return false;
    }
  }
  return true;
}

// @brief メッセージ出力管理オブジェクトを返す．
MsgMgr&
DotlibParser::msg_mgr()
{
  return mMsgMgr;
}

// @brief デバッグモードの時 true を返す．
bool
DotlibParser::debug()
{
  return mDebug;
}

// ファイルをセットする．
// 自動的にに clear() を呼ぶ．
// ファイルのオープンに失敗したら false を返す．
bool
DotlibParser::open_file(const string& filename)
{
  init();
  mInput.open(filename.c_str());
  if ( !mInput ) {
    return false;
  }
  mCurFileDesc = mFileDescMgr.new_file_desc(filename);
  return true;
}

// ファイルをクローズする．
void
DotlibParser::close_file()
{
  mInput.close();
  mCurFileDesc = NULL;
}

// 初期化
void
DotlibParser::init()
{
  mAlloc.destroy();
  mUngetChar = 0;
  mCR = false;
  mCurLine = 1;
  mCurColumn = 0;
  mUngetToken = Token(ERROR, string(), FileRegion());
}

// @brief トークンを一つとってくる．
// @param[in] symbol_mode 数字も文字とみなすモード
Token
DotlibParser::read_token(bool symbol_mode)
{
  tTokenType type;
  string value;
  FileRegion loc;
  if ( (type = mUngetToken.type()) != ERROR ) {
    value = mUngetToken.value();
    loc = mUngetToken.loc();
    mUngetToken = Token(ERROR, string(), FileRegion());
  }
  else {
    type = _read_token(symbol_mode);
    value = cur_string();
    loc = cur_loc();
  }
  return Token(type, value, loc);
}

// @brief トークンを戻す．
void
DotlibParser::unget_token(Token token)
{
  mUngetToken = token;
}

// @brief トークンを一つとってくる．
// @param[in] symbol_mode 数字も文字とみなすモード
tTokenType
DotlibParser::_read_token(bool symbol_mode)
{
  mSymbolMode = symbol_mode;

  int c;

  mCurString.clear();

 ST_INIT: // 初期状態
  c = get();
  mFirstColumn = mCurColumn;

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
    c = get();
    if ( c == '\n' ) {
      // 無視する．
      goto ST_INIT;
    }
    // それ以外はバックスラッシュがなかったことにする．
    unget();
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
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    "syntax error");
    return ERROR;
  }
  assert_not_reached(__FILE__, __LINE__);

 ST_MINUS: // '-' を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char('-');
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  unget();
  return MINUS;

 ST_NUM1: // 一文字目が[0-9]の時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  if ( c == '.' ) {
    mCurString.put_char(c);
    goto ST_DOT;
  }
  unget();
  return INT_NUM;

 ST_DOT: // [0-9]*'.' を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM2;
  }
  { // '.' の直後はかならず数字
    ostringstream buf;
    buf << "digit number expected after dot";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }

 ST_NUM2: // [0-9]*'.'[0-9]* を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM2;
  }
  if ( c == 'e' || c == 'E' ) {
    mCurString.put_char(c);
    goto ST_NUM3;
  }
  unget();
  return FLOAT_NUM;

 ST_NUM3: // [0-9]*'.'[0-9]*(e|E)を読み込んだ時
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  if ( c == '+' || c == '-' ) {
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  { // (e|E) の直後はかならず数字か符号
    ostringstream buf;
    buf << "exponent value expected";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }

 ST_NUM4: // [0-9]*'.'[0-9]*(e|E)(+|-)?[0-9]*を読み込んだ直後
  c = get();
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  unget();
  return FLOAT_NUM;

 ST_ID: // 一文字目が[a-zA-Z_]の時
  c = get();
  if ( is_symbol(c) || isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_ID;
  }
  unget();
  return SYMBOL;

 ST_DQ: // "があったら次の"までを強制的に文字列だと思う．
  c = get();
  if ( c == '\"' ) {
    return SYMBOL;
  }
  if ( c == '\\' ) {
    c = get();
    if ( c == '\n' ) {
      mCurString.put_char(c);
    }
    // それ以外はバックスラッシュを無視する．
    mCurString.put_char(c);
  }
  else if ( c == '\n' ) {
    ostringstream buf;
    buf << "unexpected newline in quoted string.";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }
  if ( c == EOF ) {
    ostringstream buf;
    buf << "unexpected end-of-file in quoted string.";
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }
  mCurString.put_char(c);
  goto ST_DQ;

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
    mMsgMgr.put_msg(__FILE__, __LINE__, cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
  }
  return ERROR;
}

// @brief c が文字の時に true を返す．
// @note mSymbolMode が true なら数字も文字とみなす．
bool
DotlibParser::is_symbol(int c)
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

// 一文字読み出す．
int
DotlibParser::get()
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
  if ( c == '\n' ) {
    ++ mCurLine;
    mCurColumn = 0;
  }
  return c;
}

// 一文字読み戻す．
void
DotlibParser::unget()
{
  mUngetChar = mLastChar;
  -- mCurColumn;
}

END_NAMESPACE_YM_CELL_DOTLIB