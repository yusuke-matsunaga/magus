
/// @file libym_cell/dotlib/GroupHandler.cc
/// @brief GroupHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"
#include "DotlibParser.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス GroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
GroupHandler::GroupHandler(DotlibParser& parser,
			   GroupHandler* parent) :
  DotlibHandler(parser, parent)
{
}

// @brief デストラクタ
GroupHandler::~GroupHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_token 属性名を表すトークン
// @return エラーが起きたら false を返す．
bool
GroupHandler::read_attr(Token attr_token)
{
  vector<Token> value_list;

  if ( !parse_complex(value_list) ) {
    return false;
  }

  if ( debug() ) {
    cout << attr_token << "( ";
    const char* comma = "";
    for (vector<Token>::const_iterator p = value_list.begin();
	 p != value_list.end(); ++ p) {
      cout << comma << *p;
      comma = ", ";
    }
    cout << " ) {" << endl;
  }

  if ( !begin_group(attr_token, value_list) ) {
    return false;
  }

  if ( !expect(LCB) ) {
    return false;
  }

  for ( ; ; ) {
    Token token = parser().read_token();
    if ( token.type() == NL ) {
      continue;
    }
    if ( token.type() == RCB ) {
      break;
    }
    if ( token.type() != SYMBOL ) {
      msg_mgr().put_msg(__FILE__, __LINE__, parser().cur_loc(),
			kMsgError,
			"DOTLIB_PARSER",
			"string value is expected.");
      return false;
    }
    hash_map<string, DotlibHandler*>::const_iterator p
      = mHandlerMap.find(token.value());
    if ( p == mHandlerMap.end() ) {
      ostringstream buf;
      buf << token.value() << ": unknown keyword.";
      msg_mgr().put_msg(__FILE__, __LINE__, token.loc(),
			kMsgError,
			"DOTLIB_PARSER",
			buf.str());
      return false;
    }
    DotlibHandler* handler = p->second;
    if ( !handler->read_attr(token) ) {
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

// @brief ハンドラの登録を行う．
// @param[in] attr_name 属性名
// @note エラーが起きたら false を返す．
bool
GroupHandler::reg_handler(const string& attr_name,
			  DotlibHandler* handler)
{
  mHandlerMap.insert(make_pair(attr_name, handler));
  return true;
}

// @brief ハンドラを取り出す．
// @param[in] attr_name 属性名
// @note なければ NULL を返す．
DotlibHandler*
GroupHandler::find_handler(const string& attr_name)
{
  hash_map<string, DotlibHandler*>::const_iterator p
    = mHandlerMap.find(attr_name);
  if ( p == mHandlerMap.end() ) {
    return NULL;
  }
  else {
    return p->second;
  }
}

// @brief 対応する PtNode を返す．
PtNode*
GroupHandler::pt_node()
{
  return mPtNode;
}

// @brief 対応する PtNode を設定する．
void
GroupHandler::set_pt_node(PtNode* node)
{
  mPtNode = node;
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_token 属性名を表すトークン
// @param[in] value_list 値を表すトークンのリスト
bool
GroupHandler::begin_group(Token attr_token,
			  const vector<Token>& value_list)
{
  PtNode* node = new_ptnode(attr_token, value_list);
  set_pt_node(node);
  parent()->pt_node()->add_child(node);

  return true;
}

// @brief group statement の最後に呼ばれる関数
bool
GroupHandler::end_group()
{
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
