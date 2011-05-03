
/// @file libym_dotlib/GroupHandler.cc
/// @brief GroupHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"
#include "DotlibParserImpl.h"
#include "PtMgr.h"
#include "PtNodeImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス GroupHandler
//////////////////////////////////////////////////////////////////////

// @brief 親がない場合のコンストラクタ
// @param[in] parser パーサー
GroupHandler::GroupHandler(DotlibParserImpl& parser) :
  DotlibHandler(parser)
{
}

// @brief 親を持つハンドラ用のコンストラクタ
// @param[in] parent 親のハンドラ
GroupHandler::GroupHandler(GroupHandler* parent) :
  DotlibHandler(parent)
{
}

// @brief デストラクタ
GroupHandler::~GroupHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @return エラーが起きたら false を返す．
bool
GroupHandler::read_attr(const ShString& attr_name,
			const FileRegion& attr_loc)
{
  PtNodeImpl* value = parse_complex();
  if ( value == NULL ) {
    return false;
  }

  if ( debug() ) {
    cout << attr_name << value << endl;
  }

  if ( !begin_group(attr_name, attr_loc, value) ) {
    return false;
  }

  if ( !expect(LCB) ) {
    return false;
  }

  for ( ; ; ) {
    tTokenType type = parser().read_token();
    if ( type == NL ) {
      continue;
    }
    if ( type == RCB ) {
      break;
    }
    FileRegion loc = parser().cur_loc();
    if ( type != SYMBOL ) {
      put_msg(__FILE__, __LINE__, loc,
	      kMsgError,
	      "DOTLIB_PARSER",
	      "string value is expected.");
      return false;
    }
    ShString name(parser().cur_string());
    DotlibHandler* handler = find_handler(name);
    if ( handler == NULL ) {
      ostringstream buf;
      buf << name << ": unknown keyword.";
      put_msg(__FILE__, __LINE__, loc,
	      kMsgError,
	      "DOTLIB_PARSER",
	      buf.str());
      return false;
    }
    if ( !handler->read_attr(name, loc) ) {
      return false;
    }
  }

  if ( !expect(NL) ) {
    return false;
  }

  if ( debug() ) {
    cout << "}" << endl;
  }

  return true;
}

// @brief attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @param[in] loc ファイル上の位置
// @return 設定が失敗したら false を返す．
// @note デフォルトの実装はエラーとなる．
bool
GroupHandler::add_attr(const ShString& attr_name,
		       PtNodeImpl* value,
		       const FileRegion& loc)
{
  PtNodeImpl* attr = pt_mgr().new_attr(attr_name, value, loc);
  mNode->add_attr(attr);
  return true;
}

// @brief ハンドラの登録を行う．
// @param[in] attr_name 属性名
// @param[in] handler 対応付けるハンドラ
// @note エラーが起きたら false を返す．
bool
GroupHandler::reg_handler(const char* attr_name,
			  DotlibHandler* handler)
{
  return reg_handler(ShString(attr_name), handler);
}

// @brief ハンドラの登録を行う．
// @param[in] attr_name 属性名
// @note エラーが起きたら false を返す．
bool
GroupHandler::reg_handler(const ShString& attr_name,
			  DotlibHandler* handler)
{
  mHandlerMap.insert(make_pair(attr_name, handler));
  return true;
}

// @brief ハンドラを取り出す．
// @param[in] attr_name 属性名
// @note なければ NULL を返す．
DotlibHandler*
GroupHandler::find_handler(const ShString& attr_name)
{
  hash_map<ShString, DotlibHandler*>::const_iterator p
    = mHandlerMap.find(attr_name);
  if ( p == mHandlerMap.end() ) {
    return NULL;
  }
  else {
    return p->second;
  }
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
bool
GroupHandler::begin_group(const ShString& attr_name,
			  const FileRegion& attr_loc,
			  PtNodeImpl* value_list)
{
  if ( !check_group_value(attr_name, attr_loc, value_list) ) {
    return false;
  }

  mNode = pt_mgr().new_group(value_list, attr_loc);
  if ( attr_name != "library" ) {
    FileRegion loc(attr_loc, mNode->loc());
    parent()->add_attr(attr_name, mNode, loc);
  }
  return true;
}

// @brief group statement の最後に呼ばれる関数
bool
GroupHandler::end_group()
{
  mNode = NULL;
  return true;
}

// @brief 対応するノードを得る．
const DotlibNode*
GroupHandler::pt_node()
{
  return mNode;
}

// @brief group statement の引数のチェックを行う仮想関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値を表すトークンのリスト
// @note begin_group() の中で呼ばれる．
// @note デフォルトの実装はなにもしないで true を返す．
bool
GroupHandler::check_group_value(const ShString& attr_name,
				const FileRegion& attr_loc,
				PtNodeImpl* value)
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EmptyGroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
EmptyGroupHandler::EmptyGroupHandler(GroupHandler* parent) :
  GroupHandler(parent)
{
}

// @brief デストラクタ
EmptyGroupHandler::~EmptyGroupHandler()
{
}

// @brief group statement の引数のチェックを行う仮想関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
// @note begin_group() の中で呼ばれる．
// @note デフォルトの実装はなにもしないで true を返す．
bool
EmptyGroupHandler::check_group_value(const ShString& attr_name,
				     const FileRegion& attr_loc,
				     PtNodeImpl* value_list)
{
  ymuint n = value_list->list_size();
  if ( n > 0 ) {
    const DotlibNode* top = value_list->top();
    FileRegion loc = top->loc();
    ostringstream buf;
    buf << attr_name << " statement does not have parameters.";
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス Str1GroupHandler
//////////////////////////////////////////////////////////////////////

// @brief 親を持たないハンドラ用のコンストラクタ
// @param[in] parser パーサー
Str1GroupHandler::Str1GroupHandler(DotlibParserImpl& parser) :
  GroupHandler(parser)
{
}

// @brief 親を持つハンドラ用のコンストラクタ
// @param[in] parent 親のハンドラ
Str1GroupHandler::Str1GroupHandler(GroupHandler* parent) :
  GroupHandler(parent)
{
}

// @brief デストラクタ
Str1GroupHandler::~Str1GroupHandler()
{
}

// @brief group statement の引数のチェックを行う仮想関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
// @note begin_group() の中で呼ばれる．
// @note デフォルトの実装はなにもしないで true を返す．
bool
Str1GroupHandler::check_group_value(const ShString& attr_name,
				    const FileRegion& attr_loc,
				    PtNodeImpl* value_list)
{
  ymuint n = value_list->list_size();
  if ( n == 0 ) {
    ostringstream buf;
    buf << attr_name << " statement requires a string parameter.";
    put_msg(__FILE__, __LINE__, attr_loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  const DotlibNode* top = value_list->top();
  if ( n > 1 ) {
    const DotlibNode* second = top->next();
    FileRegion loc = second->loc();
    ostringstream buf;
    buf << attr_name << " statement has only one string parameter.";
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  if ( !top->is_string() ) {
    put_msg(__FILE__, __LINE__, top->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is expected.");
    return false;
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス Str2GroupHandler
//////////////////////////////////////////////////////////////////////

// @brief ハンドラ用のコンストラクタ
// @param[in] parent 親のハンドラ
Str2GroupHandler::Str2GroupHandler(GroupHandler* parent) :
  GroupHandler(parent)
{
}

// @brief デストラクタ
Str2GroupHandler::~Str2GroupHandler()
{
}

// @brief group statement の引数のチェックを行う仮想関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
// @note begin_group() の中で呼ばれる．
// @note デフォルトの実装はなにもしないで true を返す．
bool
Str2GroupHandler::check_group_value(const ShString& attr_name,
				    const FileRegion& attr_loc,
				    PtNodeImpl* value_list)
{
  ymuint n = value_list->list_size();
  if ( n < 2 ) {
    ostringstream buf;
    buf << attr_name << " statement requires two string parameters.";
    put_msg(__FILE__, __LINE__, attr_loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  const DotlibNode* top = value_list->top();
  const DotlibNode* second = top->next();
  if ( n > 2 ) {
    const DotlibNode* third = second->next();
    FileRegion loc = third->loc();
    ostringstream buf;
    buf << attr_name << " statement has two string parameters.";
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  if ( !top->is_string() ) {
    put_msg(__FILE__, __LINE__, top->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is expected.");
    return false;
  }
  if ( !second->is_string() ) {
    put_msg(__FILE__, __LINE__, second->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is expected.");
    return false;
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス Str2IntGroupHandler
//////////////////////////////////////////////////////////////////////

// @brief ハンドラ用のコンストラクタ
// @param[in] parent 親のハンドラ
Str2IntGroupHandler::Str2IntGroupHandler(GroupHandler* parent) :
  GroupHandler(parent)
{
}

// @brief デストラクタ
Str2IntGroupHandler::~Str2IntGroupHandler()
{
}

// @brief group statement の引数のチェックを行う仮想関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
// @note begin_group() の中で呼ばれる．
// @note デフォルトの実装はなにもしないで true を返す．
bool
Str2IntGroupHandler::check_group_value(const ShString& attr_name,
				       const FileRegion& attr_loc,
				       PtNodeImpl* value_list)
{
  ymuint n = value_list->list_size();
  if ( n < 3 ) {
    ostringstream buf;
    buf << attr_name
	<< " statement requires two string and an integer parameters.";
    put_msg(__FILE__, __LINE__, attr_loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  const DotlibNode* top = value_list->top();
  const DotlibNode* second = top->next();
  const DotlibNode* third = second->next();
  if ( n > 3 ) {
    const DotlibNode* forth = third->next();
  FileRegion loc = forth->loc();
    ostringstream buf;
    buf << attr_name << " statement has two string and an integer parameters.";
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  if ( !top->is_string() ) {
    put_msg(__FILE__, __LINE__, top->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is expected.");
    return false;
  }
  if ( !second->is_string() ) {
    put_msg(__FILE__, __LINE__, second->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is expected.");
    return false;
  }
  if ( !third->is_string() ) {
    put_msg(__FILE__, __LINE__, second->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "integer value is expected.");
    return false;
  }

  return true;
}

END_NAMESPACE_YM_DOTLIB