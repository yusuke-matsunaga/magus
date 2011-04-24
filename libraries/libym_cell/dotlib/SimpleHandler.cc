
/// @file libym_cell/dotlib/SimpleHandler.cc
/// @brief SimpleHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SimpleHandler.h"
#include "DotlibParser.h"
#include "GroupHandler.h"
#include "PtMgr.h"
#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス SimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
// @param[in] symbol_mode シンボルモード
SimpleHandler::SimpleHandler(DotlibParser& parser,
			     GroupHandler* parent) :
  DotlibHandler(parser, parent)
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
SimpleHandler::read_attr(const string& attr_name,
			 const FileRegion& attr_loc)
{
  if ( !expect(COLON) ) {
    return false;
  }

  tTokenType value_type = parser().read_token( symbol_mode() );
  switch ( value_type ) {
  case INT_NUM:
    if ( debug() ) {
      cout << attr_name << " : " << parser().cur_int() << endl;
    }
    if ( !read_value(attr_name, attr_loc,
		     parser().cur_int(), parser().cur_loc()) ) {
      return false;
    }
    break;

  case FLOAT_NUM:
    if ( debug() ) {
      cout << attr_name << " : " << parser().cur_float() << endl;
    }
    if ( !read_value(attr_name, attr_loc,
		     parser().cur_float(), parser().cur_loc()) ) {
      return false;
    }
    break;

  case SYMBOL:
    if ( debug() ) {
      cout << attr_name << " : " << parser().cur_string() << endl;
    }
    if ( !read_value(attr_name, attr_loc,
		     string(parser().cur_string()), parser().cur_loc()) ) {
      return false;
    }
    break;

  default:
    put_msg(__FILE__, __LINE__, parser().cur_loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "Syntax error. int/float/string value is expected.");
    return false;
  }

  return expect_nl();
}

// @brief シンボルモードで read_token() を呼ぶときに true を返す．
// @note デフォルトの実装では false を返す．
bool
SimpleHandler::symbol_mode()
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス IntSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IntSimpleHandler::IntSimpleHandler(DotlibParser& parser,
				   GroupHandler* parent) :
  SimpleHandler(parser, parent)
{
}

// @brief デストラクタ
IntSimpleHandler::~IntSimpleHandler()
{
}

// @brief 整数値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
// @param[in] value_loc 値のファイル上の位置
bool
IntSimpleHandler::read_value(const string& attr_name,
			     const FileRegion& attr_loc,
			     int value,
			     const FileRegion& value_loc)
{
  PtValue* _value = ptmgr().new_value(value, value_loc);
  parent()->pt_node()->add_value(attr_name, attr_loc, _value);
  return true;
}

// @brief 実数値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
// @param[in] value_loc 値のファイル上の位置
bool
IntSimpleHandler::read_value(const string& attr_name,
			     const FileRegion& attr_loc,
			     double value,
			     const FileRegion& value_loc)
{
  put_msg(__FILE__, __LINE__, value_loc,
	  kMsgError,
	  "DOTLIB_PARSER",
	  "Syntax error. int value is expected.");
  return false;
}

// @brief 文字列を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
// @param[in] value_loc 値のファイル上の位置
bool
IntSimpleHandler::read_value(const string& attr_name,
			     const FileRegion& attr_loc,
			     const string& value,
			     const FileRegion& value_loc)
{
  put_msg(__FILE__, __LINE__, value_loc,
	  kMsgError,
	  "DOTLIB_PARSER",
	  "Syntax error. int value is expected.");
  return false;
}

END_NAMESPACE_YM_CELL_DOTLIB
