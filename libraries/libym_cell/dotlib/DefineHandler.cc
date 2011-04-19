
/// @file libym_cell/dotlib/DefineHandler.cc
/// @brief DefineHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DefineHandler.h"
#include "LibraryGroupHandler.h"

#include "ym_utils/FileRegion.h"

#include "DummySimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DefineHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
DefineHandler::DefineHandler(LibraryGroupHandler* parent) :
  ComplexHandler(parent->parser()),
  mParent(parent)
{
}

// @brief デストラクタ
DefineHandler::~DefineHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
// @return エラーが起きたら false を返す．
bool
DefineHandler::read_value(const string& attr_name,
			  const vector<Token>& token_list)
{
  assert_cond( attr_name == "define", __FILE__, __LINE__);

  cout << attr_name << " (";
  const char* comma = "";
  for (vector<Token>::const_iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    cout << comma << p->value();
    comma = ", ";
  }
  cout << ")" << endl;

  if ( token_list.size() != 3 ) {
    msg_mgr().put_msg(__FILE__, __LINE__, FileRegion(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "define attributes requires 3 values");
    return false;
  }

  bool error = false;
  Token keyword = token_list[0];
  if ( keyword.type() != STR ) {
    msg_mgr().put_msg(__FILE__, __LINE__, keyword.loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 1st argument.");
    error = true;
  }

  Token group = token_list[1];
  if ( group.type() != STR ) {
    msg_mgr().put_msg(__FILE__, __LINE__, group.loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 2nd argument.");
    error = true;
  }

  Token type = token_list[2];
  if ( type.type() != STR ) {
    msg_mgr().put_msg(__FILE__, __LINE__, type.loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 3rd argument.");
    error = true;
  }
  if ( error ) {
    return false;
  }

  DotLibHandler* handler = mParent->find_handler(group.value());
  if ( handler == NULL ) {
    ostringstream buf;
    buf << group.value() << ": Unknown attribute. ignored.";
    msg_mgr().put_msg(__FILE__, __LINE__, group.loc(),
		      kMsgWarning,
		      "DOTLIB_PARSER",
		      buf.str());
    return true;
  }

  // 今は type を無視
  DotLibHandler* new_handler = new DummySimpleHandler(parser());

  handler->reg_handler(keyword.value(), new_handler);

  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
