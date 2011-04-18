
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
  mLex(mMsgMgr)
{
  init_handler();
}

// デストラクタ
DotLibParser::~DotLibParser()
{
}

// @brief ファイルを読み込む．
// @param[in] filename ファイル名
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
DotLibParser::read_file(const string& filename)
{
  if ( !lex().open_file(filename) ) {
    return false;
  }

  bool error = false;
  for ( ; ; ) {
    tTokenType token = lex().read_token();
    if ( token == ID_STR ) {
      const char* name = lex().cur_string();
      DotLibHandler* handler = find_handler(name);
      if ( handler == NULL ) {
	ostringstream buf;
	buf << name << ": unknown keyword.";
	msg_mgr().put_msg(__FILE__, __LINE__, lex().cur_file_region(),
			  kMsgError,
			  "DOTLIB_PARSER",
			  buf.str());
	error = true;
	break;
      }
      bool stat = handler->read_attr(name);
      if ( !stat ) {
	error = true;
	break;
      }
    }
    else if ( token == NL ) {
      continue;
    }
    else if ( token == END ) {
      break;
    }
    else {
      msg_mgr().put_msg(__FILE__, __LINE__, lex().cur_file_region(),
			kMsgError,
			"DOTLIB_PARSER",
			"identifier expected.");
      error = true;
      break;
    }
  }

  lex().close_file();

  return !error;
}

// @brief ハンドラを登録する．
void
DotLibParser::init_handler()
{
  DotLibHandler* handler = new LibraryGroupHandler(*this);
  string name("library");
  mHandlerMap.insert(make_pair(name, handler));
}

// @brief キーワードに対応した構文要素を返す．
// @param[in] name キーワード
// @note なければ NULL を返す．
DotLibHandler*
DotLibParser::find_handler(const char* name)
{
  hash_map<string, DotLibHandler*>::const_iterator p = mHandlerMap.find(name);
  if ( p == mHandlerMap.end() ) {
    return NULL;
  }
  return p->second;
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
  case ID_STR:    type_str = "identifier"; break;
  case STR:       type_str = "double-quoted string"; break;
  case NL:        type_str = "new-line"; break;
  case ERROR:     assert_not_reached(__FILE__, __LINE__);
  case END:       assert_not_reached(__FILE__, __LINE__);
  }
  ostringstream buf;
  buf << "syntax error. " << type_str << " is expected.";
  mMsgMgr.put_msg(__FILE__, __LINE__, lex().cur_file_region(),
		  kMsgError,
		  "DOTLIB_PARSER",
		  buf.str());
  return false;
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

END_NAMESPACE_YM_CELL_DOTLIB
