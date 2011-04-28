
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
// @param[in] parent 親のハンドラ
SimpleHandler::SimpleHandler(GroupHandler* parent) :
  DotlibHandler(parent->parser(), parent->ptmgr(), parent)
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

  PtNode* value = read_value();
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
PtNode*
SimpleHandler::read_value()
{
  tTokenType value_type = parser().read_token(false);
  PtNode* value = new_ptvalue(value_type);
  return value;
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
// @note デフォルトの実装ではなにもしないで true を返す．
bool
SimpleHandler::set_value(const ShString& attr_name,
			 const FileRegion& attr_loc,
			 PtNode* value)
{
  return parent()->add_attr(attr_name, value);
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
PtNode*
SymSimpleHandler::read_value()
{
  tTokenType value_type = parser().read_token(true);
  PtNode* value = new_ptvalue(value_type);
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
PtNode*
IntSimpleHandler::read_value()
{
  PtNode* value = SimpleHandler::read_value();
  if ( value->type() != PtNode::kInt ) {
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
PtNode*
FloatSimpleHandler::read_value()
{
  PtNode* value = SimpleHandler::read_value();
  if ( value->type() != PtNode::kFloat && value->type() != PtNode::kInt ) {
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
PtNode*
StrSimpleHandler::read_value()
{
  PtNode* value = SimpleHandler::read_value();
  if ( value->type() != PtNode::kString ) {
    put_msg(__FILE__, __LINE__, value->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "Syntax error. string value is expected.");
    return NULL;
  }
  return value;
}

END_NAMESPACE_YM_CELL_DOTLIB
