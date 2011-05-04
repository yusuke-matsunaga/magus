
/// @file libym_dotlib/DefineHandler.cc
/// @brief DefineHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DefineHandler.h"
#include "DotlibParserImpl.h"
#include "SimpleHandler.h"
#include "GroupHandler.h"
#include "PtNodeImpl.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DefineHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
DefineHandler::DefineHandler(GroupHandler* parent) :
  ComplexHandler(parent)
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
DefineHandler::read_attr(const ShString& attr_name,
			 const FileRegion& attr_loc)
{
  const DotlibNode* value = parse_complex();
  if ( value == NULL ) {
    return false;
  }

  if ( !expect_nl() ) {
    return false;
  }

  if ( debug() ) {
    cout << attr_name << " : " << value << endl;
  }

  const DotlibNode* keyword = value->top();
  if ( keyword == NULL || !keyword->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    keyword->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "string value is expected for 1st argument.");
    return false;
  }

  const DotlibNode* group = keyword->next();
  if ( group == NULL || !group->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    group->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "string value is expected for 2nd argument.");
    return false;
  }

  const DotlibNode* type_token = group->next();
  if ( type_token == NULL || !type_token->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    type_token->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "string value is expected for 3rd argument.");
    return false;
  }

  DotlibHandler* handler = parent()->find_handler(group->string_value());
  if ( handler == NULL ) {
    ostringstream buf;
    buf << group->string_value() << ": Unknown attribute. ignored.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    group->loc(),
		    kMsgWarning,
		    "DOTLIB_PARSER",
		    buf.str());
    return true;
  }

  GroupHandler* g_handler = dynamic_cast<GroupHandler*>(handler);
  if ( g_handler == NULL ) {
    ostringstream buf;
    buf << group->string_value() << ": is not a group statement.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    group->loc(),
		    kMsgWarning,
		    "DOTLIB_PARSER",
		    buf.str());
    return true;
  }

  DotlibHandler* new_handler = NULL;
  ShString type_str = type_token->string_value();
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
    MsgMgr::put_msg(__FILE__, __LINE__,
		    type_token->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    buf.str());
    return false;
  }

  g_handler->reg_handler(keyword->string_value(), new_handler);

  return true;
}

END_NAMESPACE_YM_DOTLIB
