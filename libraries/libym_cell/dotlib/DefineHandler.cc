
/// @file libym_cell/dotlib/DefineHandler.cc
/// @brief DefineHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DefineHandler.h"
#include "DotLibLex.h"
#include "SimpleHandler.h"
#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DefineHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
DefineHandler::DefineHandler(DotLibParser& parser,
			     GroupHandler* parent) :
  DotLibHandler(parser, parent)
{
}

// @brief デストラクタ
DefineHandler::~DefineHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_token 属性名を表すトークン
// @return エラーが起きたら false を返す．
bool
DefineHandler::read_attr(Token attr_token)
{
  if ( !expect(LP) ) {
    return false;
  }

  vector<Token> value_list;
  tTokenType type = lex().read_token();
  if ( type != RP ) {
    for ( ; ; ) {
      string value = lex().cur_string();
      FileRegion loc = lex().cur_loc();
      value_list.push_back(Token(type, value, loc));

      tTokenType type1 = lex().read_token();
      if ( type1 == RP ) {
	break;
      }
      if ( type1 != COMMA ) {
	msg_mgr().put_msg(__FILE__, __LINE__, lex().cur_loc(),
			  kMsgError,
			  "DOTLIB_PARSER",
			  "syntax error. ',' is expected.");
	return false;
      }
      type = lex().read_token();
    }
  }

  if ( debug() ) {
    cout << attr_token << " (";
    const char* comma = "";
    for (vector<Token>::const_iterator p = value_list.begin();
	 p != value_list.end(); ++ p) {
      cout << comma << *p;
      comma = ", ";
    }
    cout << ")" << endl;
  }

#if 0
  if ( !expect(SEMI) ) {
    return false;
  }

  if ( !expect(NL) ) {
    return false;
  }
#else
  if ( !expect_nl() ) {
    return false;
  }
#endif

  bool error = false;
  Token keyword = value_list[0];
  if ( keyword.type() != STR ) {
    msg_mgr().put_msg(__FILE__, __LINE__, keyword.loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 1st argument.");
    error = true;
  }

  Token group = value_list[1];
  if ( group.type() != STR ) {
    msg_mgr().put_msg(__FILE__, __LINE__, group.loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 2nd argument.");
    error = true;
  }

  Token type_token = value_list[2];
  if ( type_token.type() != STR ) {
    msg_mgr().put_msg(__FILE__, __LINE__, type_token.loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 3rd argument.");
    error = true;
  }
  if ( error ) {
    return false;
  }

  DotLibHandler* handler = parent()->find_handler(group.value());
  if ( handler == NULL ) {
    ostringstream buf;
    buf << group.value() << ": Unknown attribute. ignored.";
    msg_mgr().put_msg(__FILE__, __LINE__, group.loc(),
		      kMsgWarning,
		      "DOTLIB_PARSER",
		      buf.str());
    return true;
  }

  GroupHandler* g_handler = dynamic_cast<GroupHandler*>(handler);
  assert_cond( g_handler != NULL, __FILE__, __LINE__);

  // 今は type を無視
  DotLibHandler* new_handler = new SimpleHandler(parser(), g_handler);

  g_handler->reg_handler(keyword.value(), new_handler);

  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
