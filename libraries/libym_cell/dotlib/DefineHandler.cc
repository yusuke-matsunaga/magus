
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
#include "PtValue.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DefineHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
DefineHandler::DefineHandler(GroupHandler* parent) :
  DotlibHandler(parent->parser(), parent->ptmgr(), parent)
{
}

// @brief デストラクタ
DefineHandler::~DefineHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @return エラーが起きたら false を返す．
bool
DefineHandler::read_attr(const string& attr_name,
			 const FileRegion& attr_loc)
{
  vector<const PtValue*> value_list;
  if ( !parse_complex(value_list) ) {
    return false;
  }

  if ( debug() ) {
    cout << attr_name << " (";
    const char* comma = "";
    for (vector<const PtValue*>::const_iterator p = value_list.begin();
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
  const PtValue* keyword = value_list[0];
  if ( keyword->type() != SYMBOL ) {
    put_msg(__FILE__, __LINE__, keyword->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is expected for 1st argument.");
    error = true;
  }

  const PtValue* group = value_list[1];
  if ( group->type() != SYMBOL ) {
    put_msg(__FILE__, __LINE__, group->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is expected for 2nd argument.");
    error = true;
  }

  const PtValue* type_token = value_list[2];
  if ( type_token->type() != SYMBOL ) {
    put_msg(__FILE__, __LINE__, type_token->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is expected for 3rd argument.");
    error = true;
  }
  if ( error ) {
    return false;
  }

  DotlibHandler* handler = parent()->find_handler(group->string_value());
  if ( handler == NULL ) {
    ostringstream buf;
    buf << group->string_value() << ": Unknown attribute. ignored.";
    put_msg(__FILE__, __LINE__, group->loc(),
	    kMsgWarning,
	    "DOTLIB_PARSER",
	    buf.str());
    return true;
  }

  GroupHandler* g_handler = dynamic_cast<GroupHandler*>(handler);
  if ( g_handler == NULL ) {
    ostringstream buf;
    buf << group->string_value() << ": is not a group statement.";
    put_msg(__FILE__, __LINE__, group->loc(),
	    kMsgWarning,
	    "DOTLIB_PARSER",
	    buf.str());
    return true;
  }

  DotlibHandler* new_handler = NULL;
  string type_str = type_token->string_value();
  if ( type_str == "int" ) {
    new_handler = new IntSimpleHandler(g_handler);
  }
  else if ( type_str == "float" ) {
    new_handler = new FloatSimpleHandler(g_handler);
  }
  else if ( type_str == "string" ) {
    new_handler = new StrSimpleHandler(g_handler);
  }
  else {
    ostringstream buf;
    buf << "Unknown type: " << type_str << ".";
    put_msg(__FILE__, __LINE__, type_token->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  g_handler->reg_handler(keyword->string_value(), new_handler);

  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
