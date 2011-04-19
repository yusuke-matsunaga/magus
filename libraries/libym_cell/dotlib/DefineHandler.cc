
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
// @param[in] value_list 値の型と値を表す文字列のペアのリスト
// @return エラーが起きたら false を返す．
bool
DefineHandler::read_value(const string& attr_name,
			  const list<pair<tTokenType, string> >& value_list)
{
  assert_cond( attr_name == "define", __FILE__, __LINE__);

  cout << attr_name << " (";
  const char* comma = "";
  for (list<pair<tTokenType, string> >::const_iterator p = value_list.begin();
       p != value_list.end(); ++ p) {
    cout << comma << p->second;
    comma = ", ";
  }
  cout << ")" << endl;

  if ( value_list.size() != 3 ) {
    msg_mgr().put_msg(__FILE__, __LINE__, FileRegion(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "define attributes requires 3 values");
    return false;
  }

  list<pair<tTokenType, string> >::const_iterator p = value_list.begin();

  bool error = false;
  tTokenType type1 = p->first;
  string value1 = p->second;
  if ( type1 != ID_STR && type1 != STR ) {
    msg_mgr().put_msg(__FILE__, __LINE__, FileRegion(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 1st argument.");
    error = true;
  }

  ++ p;
  tTokenType type2 = p->first;
  string value2 = p->second;
  if ( type2 != ID_STR && type2 != STR ) {
    msg_mgr().put_msg(__FILE__, __LINE__, FileRegion(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 2nd argument.");
    error = true;
  }

  ++ p;
  tTokenType type3 = p->first;
  string value3 = p->second;
  if ( type3 != ID_STR && type3 != STR ) {
    msg_mgr().put_msg(__FILE__, __LINE__, FileRegion(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "string value is expected for 3rd argument.");
    error = true;
  }
  if ( error ) {
    return false;
  }

  DotLibHandler* handler = mParent->find_handler(value2);
  if ( handler == NULL ) {
    msg_mgr().put_msg(__FILE__, __LINE__, FileRegion(),
		      kMsgWarning,
		      "DOTLIB_PARSER",
		      "unknown attribute. ignored.");
    return true;
  }

  // 今は value を無視
  DotLibHandler* new_handler = new DummySimpleHandler(parser());

  handler->reg_handler(value1, new_handler);

  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
