
/// @file libym_cell/dotlib/DotLibParser.cc
/// @brief DotLibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotLibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotLibParser.h"
#include "DotLibLex.h"
#include "LibraryGroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

// コンストラクタ
DotLibParser::DotLibParser() :
  mLex(mMsgMgr),
  mLibraryHandler(new LibraryGroupHandler(*this))
{
}

// デストラクタ
DotLibParser::~DotLibParser()
{
}

// @brief ファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] debug デバッグモード
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
DotLibParser::read_file(const string& filename,
			bool debug)
{
  mDebug = debug;

  if ( !lex().open_file(filename) ) {
    return false;
  }

  bool error = false;
  tTokenType type;
  string name;
  for ( ; ; ) {
    type = lex().read_token();
    if ( type != NL ) {
      name = lex().cur_string();
      break;
    }
  }
  if ( type != STR || name != "library" ) {
    msg_mgr().put_msg(__FILE__, __LINE__, lex().cur_loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "'library' keyword is expected "
		      "on the top of the structure");
    error = true;
    goto last;
  }
  if ( !mLibraryHandler->read_attr(name) ) {
    error = true;
    goto last;
  }

  if ( !expect_nl() ) {
    error = true;
    goto last;
  }
  for ( ; ; ) {
    tTokenType type = lex().read_token();
    if ( type == END ) {
      break;
    }
    msg_mgr().put_msg(__FILE__, __LINE__, lex().cur_loc(),
		      kMsgWarning,
		      "DOTLIB_PARSER",
		      "contents after library group are ignored.");
  }

last:
  lex().close_file();

  return !error;
}

// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
bool
DotLibParser::expect(tTokenType type)
{
  tTokenType token = lex().read_token();
  if ( token == type ) {
    return true;
  }

  const char* type_str = NULL;
  switch ( type ) {
  case COLON:     type_str = "':'"; break;
  case SEMI:      type_str = "';'"; break;
  case COMMA:     type_str = "','"; break;
  case MINUS:     type_str = "'-'"; break;
  case LP:        type_str = "'('"; break;
  case RP:        type_str = "')'"; break;
  case LCB:       type_str = "'{'"; break;
  case RCB:       type_str = "'}'"; break;
  case INT_NUM:   type_str = "integer value"; break;
  case FLOAT_NUM: type_str = "float value"; break;
  case STR:       type_str = "string"; break;
  case NL:        type_str = "new-line"; break;
  case ERROR:     assert_not_reached(__FILE__, __LINE__);
  case END:       assert_not_reached(__FILE__, __LINE__);
  }
  ostringstream buf;
  buf << "syntax error. " << type_str << " is expected.";
  mMsgMgr.put_msg(__FILE__, __LINE__, lex().cur_loc(),
		  kMsgError,
		  "DOTLIB_PARSER",
		  buf.str());
  return false;
}

// @brief 行末まで読み込む．
bool
DotLibParser::expect_nl()
{
  for ( ; ; ) {
    tTokenType token = lex().read_token();
    if ( token == NL ) {
      return true;
    }
    if ( token != SEMI ) {
      ostringstream buf;
      mMsgMgr.put_msg(__FILE__, __LINE__, lex().cur_loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "syntax error.");
    }
  }
}

// @brief メッセージ出力管理オブジェクトを返す．
MsgMgr&
DotLibParser::msg_mgr()
{
  return mMsgMgr;
}

// @brief 字句解析器を返す．
DotLibLex&
DotLibParser::lex()
{
  return mLex;
}

// @brief デバッグモードの時 true を返す．
bool
DotLibParser::debug()
{
  return mDebug;
}

END_NAMESPACE_YM_CELL_DOTLIB
