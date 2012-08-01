
/// @file libym_dotlib/SimpleHandler.cc
/// @brief SimpleHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SimpleHandler.h"
#include "DotlibParserImpl.h"
#include "DotlibMgrImpl.h"
#include "GroupHandler.h"
#include "DotlibNodeImpl.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス SimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
// @param[in] sym_mode シンボルモード
// @note シンボルモードの時は数字で始まっていても文字列とみなす．
SimpleHandler::SimpleHandler(GroupHandler* parent,
			     bool sym_mode) :
  DotlibHandler(parent),
  mSymMode(sym_mode)
{
}

// @brief デストラクタ
SimpleHandler::~SimpleHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @return エラーが起きたら false を返す．
bool
SimpleHandler::read_attr(const ShString& attr_name,
			 const FileRegion& attr_loc)
{
  if ( !expect(COLON) ) {
    return false;
  }

  DotlibNodeImpl* value = read_value();
  if ( value == NULL ) {
    return false;
  }
  if ( !expect_nl() ) {
    return false;
  }

  if ( debug() ) {
    cout << attr_name << " : " << value << endl;
  }

  if ( !set_value(attr_name, attr_loc, value) ) {
    return false;
  }

  return true;
}

// @brief 値を読み込む処理
// @return 値を表す DotlibNode を返す．
// @note エラーが起きたら NULL を返す．
// @note デフォルトの実装では普通に DotlibParser::read_token() を呼ぶ．
DotlibNodeImpl*
SimpleHandler::read_value()
{
  FileRegion loc;
  tTokenType value_type = parser().read_token(loc, mSymMode);
  DotlibNodeImpl* value = new_value(value_type, false, loc);
  return value;
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
bool
SimpleHandler::set_value(const ShString& attr_name,
			 const FileRegion& attr_loc,
			 DotlibNodeImpl* value)
{
  FileRegion loc(attr_loc, value->loc());
  return parent()->add_attr(attr_name, value, loc);
}


//////////////////////////////////////////////////////////////////////
// クラス StrSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
// @param[in] sym_mode シンボルモード
// @note シンボルモードの時は数字で始まっていても文字列とみなす．
StrSimpleHandler::StrSimpleHandler(GroupHandler* parent,
				   bool sym_mode) :
  SimpleHandler(parent, sym_mode)
{
}

// @brief デストラクタ
StrSimpleHandler::~StrSimpleHandler()
{
}

// @brief 値を読み込む．
DotlibNodeImpl*
StrSimpleHandler::read_value()
{
  DotlibNodeImpl* value = SimpleHandler::read_value();
  if ( !value->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    value->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. string value is expected.");
    return NULL;
  }
  return value;
}


//////////////////////////////////////////////////////////////////////
// クラス IntSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IntSimpleHandler::IntSimpleHandler(GroupHandler* parent) :
  SimpleHandler(parent, false)
{
}

// @brief デストラクタ
IntSimpleHandler::~IntSimpleHandler()
{
}

// @brief 値を読み込む．
DotlibNodeImpl*
IntSimpleHandler::read_value()
{
  DotlibNodeImpl* value = SimpleHandler::read_value();
  if ( !value->is_int() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    value->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. int value is expected.");
    return NULL;
  }
  return value;
}


//////////////////////////////////////////////////////////////////////
// クラス FloatSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FloatSimpleHandler::FloatSimpleHandler(GroupHandler* parent) :
  SimpleHandler(parent, false)
{
}

// @brief デストラクタ
FloatSimpleHandler::~FloatSimpleHandler()
{
}

// @brief 値を読み込む．
DotlibNodeImpl*
FloatSimpleHandler::read_value()
{
  DotlibNodeImpl* value = SimpleHandler::read_value();
  if ( !value->is_float() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    value->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. float value is expected.");
    return NULL;
  }
  return value;
}

END_NAMESPACE_YM_DOTLIB
