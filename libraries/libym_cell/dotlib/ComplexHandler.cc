
/// @file libym_dotlib/ComplexHandler.cc
/// @brief ComplexHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ComplexHandler.h"
#include "DotlibParserImpl.h"
#include "GroupHandler.h"
#include "DotlibNodeImpl.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス ComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
ComplexHandler::ComplexHandler(GroupHandler* parent) :
  DotlibHandler(parent)
{
}

// @brief デストラクタ
ComplexHandler::~ComplexHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @return エラーが起きたら false を返す．
bool
ComplexHandler::read_attr(const ShString& attr_name,
			  const FileRegion& attr_loc)
{
  FileRegion end_loc;
  DotlibNodeImpl* value = parse_complex(end_loc);
  if ( value == NULL ) {
    return false;
  }

  if ( !expect_nl() ) {
    return false;
  }

  if ( debug() ) {
    cout << attr_name << value << endl;
  }

  if ( !set_value(attr_name, attr_loc, value, end_loc) ) {
    return false;
  }

  return true;
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値のリスト
// @param[in] end_loc 右括弧の位置
bool
ComplexHandler::set_value(const ShString& attr_name,
			  const FileRegion& attr_loc,
			  DotlibNodeImpl* value,
			  const FileRegion& end_loc)
{
  FileRegion loc(attr_loc, end_loc);
  return parent()->add_attr(attr_name, value, loc);
}


//////////////////////////////////////////////////////////////////////
// クラス Str1ComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
Str1ComplexHandler::Str1ComplexHandler(GroupHandler* parent) :
ComplexHandler(parent)
{
}

// @brief デストラクタ
Str1ComplexHandler::~Str1ComplexHandler()
{
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値のリスト
// @param[in] end_loc 右括弧の位置
bool
Str1ComplexHandler::set_value(const ShString& attr_name,
			      const FileRegion& attr_loc,
			      DotlibNodeImpl* value,
			      const FileRegion& end_loc)
{
  assert_cond( value->is_list(), __FILE__, __LINE__ );
  if ( value->list_size() != 1 ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    value->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error, a string expected.");
    return false;
  }
  const DotlibNode* top = value->list_elem(0);
  if ( !top->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    top->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error, a string expected.");
    return false;
  }

  return ComplexHandler::set_value(attr_name, attr_loc, value, end_loc);
}


//////////////////////////////////////////////////////////////////////
// クラス StrListComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
StrListComplexHandler::StrListComplexHandler(GroupHandler* parent) :
ComplexHandler(parent)
{
}

// @brief デストラクタ
StrListComplexHandler::~StrListComplexHandler()
{
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値のリスト
// @param[in] end_loc 右括弧の位置
bool
StrListComplexHandler::set_value(const ShString& attr_name,
				 const FileRegion& attr_loc,
				 DotlibNodeImpl* value,
				 const FileRegion& end_loc)
{
  assert_cond( value->is_list(), __FILE__, __LINE__ );
  ymuint n = value->list_size();
  if ( n == 0 ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    value->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error, one ore moare strings expected.");
    return false;
  }
  for (ymuint i = 0; i < n; ++ i) {
    const DotlibNode* elem = value->list_elem(i);
    if ( !elem->is_string() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      elem->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error, a string expected.");
      return false;
    }
  }

  return ComplexHandler::set_value(attr_name, attr_loc, value, end_loc);
}


//////////////////////////////////////////////////////////////////////
// クラス UnitComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
UnitComplexHandler::UnitComplexHandler(GroupHandler* parent) :
  ComplexHandler(parent)
{
}

// @brief デストラクタ
UnitComplexHandler::~UnitComplexHandler()
{
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値のリスト
// @param[in] end_loc 右括弧の位置
bool
UnitComplexHandler::set_value(const ShString& attr_name,
			      const FileRegion& attr_loc,
			      DotlibNodeImpl* value,
			      const FileRegion& end_loc)
{
  assert_cond( value->is_list(), __FILE__, __LINE__ );
  if ( value->list_size() != 2 ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    value->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error, (number, string) pair expected.");
    return false;
  }
  const DotlibNode* top = value->list_elem(0);
  if ( !top->is_float() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    top->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error, first element should be a number.");
    return false;
  }
  const DotlibNode* next = value->list_elem(1);
  if ( !next->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    next->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error, second element should be a string.");
    return false;
  }

  return ComplexHandler::set_value(attr_name, attr_loc, value, end_loc);
}


//////////////////////////////////////////////////////////////////////
// クラス PwComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
PwComplexHandler::PwComplexHandler(GroupHandler* parent) :
  ComplexHandler(parent)
{
}

// @brief デストラクタ
PwComplexHandler::~PwComplexHandler()
{
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値のリスト
// @param[in] end_loc 右括弧の位置
bool
PwComplexHandler::set_value(const ShString& attr_name,
			    const FileRegion& attr_loc,
			    DotlibNodeImpl* value,
			    const FileRegion& end_loc)
{
  assert_cond( value->is_list(), __FILE__, __LINE__ );
  if ( value->list_size() != 2 ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    value->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error, (integer, float) pair expected.");
    return false;
  }
  const DotlibNode* top = value->list_elem(0);
  if ( !top->is_int() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    top->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error, first element should be an ID number.");
    return false;
  }
  const DotlibNode* next = value->list_elem(1);
  if ( !next->is_float() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    next->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error, second element should be a float number.");
    return false;
  }

  return ComplexHandler::set_value(attr_name, attr_loc, value, end_loc);
}

END_NAMESPACE_YM_DOTLIB
