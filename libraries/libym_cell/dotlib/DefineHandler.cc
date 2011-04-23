
/// @file libym_cell/dotlib/DefineHandler.cc
/// @brief DefineHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DefineHandler.h"
#include "DotlibParser.h"
#include "SimpleHandler.h"
#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DefineHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
DefineHandler::DefineHandler(DotlibParser& parser,
			     GroupHandler* parent) :
  DotlibHandler(parser, parent)
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
  vector<Token> value_list;
  if ( !parse_complex(value_list) ) {
    return false;
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

  if ( !expect_nl() ) {
    return false;
  }

  bool error = false;
  Token keyword = value_list[0];
  if ( keyword.type() != SYMBOL ) {
    msg_mgr().put_msg(__FILE__, __LINE__, keyword.loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 1st argument.");
    error = true;
  }

  Token group = value_list[1];
  if ( group.type() != SYMBOL ) {
    msg_mgr().put_msg(__FILE__, __LINE__, group.loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 2nd argument.");
    error = true;
  }

  Token type_token = value_list[2];
  if ( type_token.type() != SYMBOL ) {
    msg_mgr().put_msg(__FILE__, __LINE__, type_token.loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 3rd argument.");
    error = true;
  }
  if ( error ) {
    return false;
  }

  DotlibHandler* handler = parent()->find_handler(group.value());
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
  DotlibHandler* new_handler = new SimpleHandler(parser(), g_handler, false);

  g_handler->reg_handler(keyword.value(), new_handler);

  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
