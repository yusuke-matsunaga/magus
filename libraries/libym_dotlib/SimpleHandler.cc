
/// @file libym_dotlib/SimpleHandler.cc
/// @brief SimpleHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SimpleHandler.h"
#include "DotlibParserImpl.h"
#include "GroupHandler.h"
#include "PtMgr.h"
#include "PtNodeImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス SimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
SimpleHandler::SimpleHandler(GroupHandler* parent) :
  DotlibHandler(parent)
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

  PtNodeImpl* value = read_value();
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
// @return 値を表す PtNode を返す．
// @note エラーが起きたら NULL を返す．
// @note デフォルトの実装では普通に DotlibParser::read_token() を呼ぶ．
PtNodeImpl*
SimpleHandler::read_value()
{
  tTokenType value_type = parser().read_token(false);
  PtNodeImpl* value = new_ptvalue(value_type);
  return value;
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
bool
SimpleHandler::set_value(const ShString& attr_name,
			 const FileRegion& attr_loc,
			 PtNodeImpl* value)
{
  FileRegion loc(attr_loc, value->loc());
  return parent()->add_attr(attr_name, value, loc);
}


//////////////////////////////////////////////////////////////////////
// クラス SymSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SymSimpleHandler::SymSimpleHandler(GroupHandler* parent) :
  SimpleHandler(parent)
{
}

// @brief デストラクタ
SymSimpleHandler::~SymSimpleHandler()
{
}

// @brief 値を読み込む処理
// @return 値を表す PtNode を返す．
// @note エラーが起きたら NULL を返す．
// @note ここではシンボルモードで DotlibParser::read_token() を呼ぶ．
PtNodeImpl*
SymSimpleHandler::read_value()
{
  tTokenType value_type = parser().read_token(true);
  PtNodeImpl* value = new_ptvalue(value_type);
  return value;
}


//////////////////////////////////////////////////////////////////////
// クラス IntSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IntSimpleHandler::IntSimpleHandler(GroupHandler* parent) :
  SimpleHandler(parent)
{
}

// @brief デストラクタ
IntSimpleHandler::~IntSimpleHandler()
{
}

// @brief 値を読み込む．
PtNodeImpl*
IntSimpleHandler::read_value()
{
  PtNodeImpl* value = SimpleHandler::read_value();
  if ( !value->is_int() ) {
    put_msg(__FILE__, __LINE__, value->loc(),
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
  SimpleHandler(parent)
{
}

// @brief デストラクタ
FloatSimpleHandler::~FloatSimpleHandler()
{
}

// @brief 値を読み込む．
PtNodeImpl*
FloatSimpleHandler::read_value()
{
  PtNodeImpl* value = SimpleHandler::read_value();
  if ( !value->is_float() ) {
    put_msg(__FILE__, __LINE__, value->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "Syntax error. float value is expected.");
    return NULL;
  }
  return value;
}


//////////////////////////////////////////////////////////////////////
// クラス StrSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
StrSimpleHandler::StrSimpleHandler(GroupHandler* parent) :
  SimpleHandler(parent)
{
}

// @brief デストラクタ
StrSimpleHandler::~StrSimpleHandler()
{
}

// @brief 値を読み込む．
PtNodeImpl*
StrSimpleHandler::read_value()
{
  PtNodeImpl* value = SimpleHandler::read_value();
  if ( !value->is_string() ) {
    put_msg(__FILE__, __LINE__, value->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "Syntax error. string value is expected.");
    return NULL;
  }
  return value;
}

END_NAMESPACE_YM_DOTLIB
