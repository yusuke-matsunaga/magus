
/// @file libym_cell/dotlib/DotLibHandler.cc
/// @brief DotLibHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotLibHandler.h"
#include "DotLibParser.h"
#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotLibHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
DotLibHandler::DotLibHandler(DotLibParser& parser) :
  mParser(parser)
{
}

// @brief デストラクタ
DotLibHandler::~DotLibHandler()
{
}

// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
bool
DotLibHandler::expect(tTokenType type)
{
  return mParser.expect(type);
}

// @brief パーサーを得る．
DotLibParser&
DotLibHandler::parser()
{
  return mParser;
}

// @brief メッセージ出力管理オブジェクトを得る．
MsgMgr&
DotLibHandler::msg_mgr()
{
  return mParser.msg_mgr();
}

// @brief 字句解析器を得る．
DotLibLex&
DotLibHandler::lex()
{
  return mParser.lex();
}


//////////////////////////////////////////////////////////////////////
// クラス SimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
SimpleHandler::SimpleHandler(DotLibParser& parser) :
  DotLibHandler(parser)
{
}

// @brief デストラクタ
SimpleHandler::~SimpleHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @return エラーが起きたら false を返す．
bool
SimpleHandler::read_attr(const string& attr_name)
{
  if ( !expect(COLON) ) {
    return false;
  }

  tTokenType token = lex().read_token();
  string value = lex().cur_string();
  if ( !read_value(attr_name, token, value) ) {
    return false;
  }

  if ( !expect(SEMI) ) {
    return false;
  }

  if ( !expect(NL) ) {
    return false;
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス ComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
ComplexHandler::ComplexHandler(DotLibParser& parser) :
  DotLibHandler(parser)
{
}

// @brief デストラクタ
ComplexHandler::~ComplexHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @return エラーが起きたら false を返す．
bool
ComplexHandler::read_attr(const string& attr_name)
{
  if ( !expect(LP) ) {
    return false;
  }

  list<pair<tTokenType, string> > value_list;
  for ( ; ; ) {
    tTokenType type = lex().read_token();
    string value = lex().cur_string();
    value_list.push_back(make_pair(type, value));

    tTokenType type1 = lex().read_token();
    if ( type1 == RP ) {
      break;
    }
    if ( type1 != COMMA ) {
      msg_mgr().put_msg(__FILE__, __LINE__, lex().cur_file_region(),
			kMsgError,
			"DOTLIB_PARSER",
			"syntax error. ',' is expected.");
      return false;
    }
  }
  if ( !read_value(attr_name, value_list) ) {
    return false;
  }

  if ( !expect(SEMI) ) {
    return false;
  }

  if ( !expect(NL) ) {
    return false;
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス GroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
GroupHandler::GroupHandler(DotLibParser& parser) :
  DotLibHandler(parser)
{
}

// @brief デストラクタ
GroupHandler::~GroupHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @return エラーが起きたら false を返す．
bool
GroupHandler::read_attr(const string& attr_name)
{
  if ( !expect(LP) ) {
    return false;
  }

  if ( !expect(ID_STR) ) {
    return false;
  }

  string group_name = lex().cur_string();
  read_group_name(attr_name, group_name);

  if ( !expect(RP) ) {
    return false;
  }

  if ( !expect(LCB) ) {
    return false;
  }

  if ( !expect(NL) ) {
    return false;
  }

  for ( ; ; ) {
    tTokenType type = lex().read_token();
    if ( type == RCB ) {
      break;
    }
    if ( type == NL ) {
      continue;
    }
    if ( type != ID_STR ) {
      msg_mgr().put_msg(__FILE__, __LINE__, lex().cur_file_region(),
			kMsgError,
			"DOTLIB_PARSER",
			"identifier expected.");
      return false;
    }
    const char* name1 = lex().cur_string();
    hash_map<string, DotLibHandler*>::const_iterator p = mHandlerMap.find(name1);
    if ( p == mHandlerMap.end() ) {
      ostringstream buf;
      buf << name1 << ": unknown keyword.";
      msg_mgr().put_msg(__FILE__, __LINE__, lex().cur_file_region(),
			kMsgError,
			"DOTLIB_PARSER",
			buf.str());
      return false;
    }
    DotLibHandler* handler = p->second;
    if ( !handler->read_attr(name1) ) {
      return false;
    }
  }

  if ( !expect(NL) ) {
    return false;
  }

  return true;
}

// @brief ハンドラの登録を行う．
void
GroupHandler::reg_handler(const char* name,
			  DotLibHandler* handler)
{
  mHandlerMap.insert(make_pair(string(name), handler));
}


END_NAMESPACE_YM_CELL_DOTLIB
