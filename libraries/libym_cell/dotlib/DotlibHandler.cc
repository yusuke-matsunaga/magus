
/// @file libym_cell/dotlib/DotlibHandler.cc
/// @brief DotlibHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"
#include "DotlibParser.h"
#include "SimpleHandler.h"
#include "ExprHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"
#include "Token.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
DotlibHandler::DotlibHandler(DotlibParser& parser,
			     GroupHandler* parent) :
  mParser(parser),
  mParent(parent)
{
}

// @brief デストラクタ
DotlibHandler::~DotlibHandler()
{
}

// @brief PtNode を生成する．
// @param[in] attr_token 属性名を表すトークン
// @param[in] value_token 値を表すトークン
PtNode*
DotlibHandler::new_ptnode(Token attr_token,
			  Token value_token)
{
  return mParser.new_ptnode(attr_token, value_token);
}

// @brief PtNode を生成する．
// @param[in] attr_token 属性名を表すトークン
// @param[in] value_list 値を表すトークンのリスト
PtNode*
DotlibHandler::new_ptnode(Token attr_token,
			  const vector<Token>& value_list)
{
  return mParser.new_ptnode(attr_token, value_list);
}

// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
bool
DotlibHandler::expect(tTokenType type)
{
  return mParser.expect(type);
}

// @brief 行末まで読み込む．
bool
DotlibHandler::expect_nl()
{
  return mParser.expect_nl();
}

// @brief 親のハンドラを得る．
GroupHandler*
DotlibHandler::parent()
{
  return mParent;
}

// @brief パーサーを得る．
DotlibParser&
DotlibHandler::parser()
{
  return mParser;
}

// @brief メッセージ出力管理オブジェクトを得る．
MsgMgr&
DotlibHandler::msg_mgr()
{
  return mParser.msg_mgr();
}

// @brief デバッグモードの時に true を返す．
bool
DotlibHandler::debug()
{
  return mParser.debug();
}


//////////////////////////////////////////////////////////////////////
// クラス SimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
// @param[in] symbol_mode シンボルモード
SimpleHandler::SimpleHandler(DotlibParser& parser,
			     GroupHandler* parent,
			     bool symbol_mode) :
  DotlibHandler(parser, parent),
  mSymbolMode(symbol_mode)
{
}

// @brief デストラクタ
SimpleHandler::~SimpleHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_token 属性名を表すトークン
// @return エラーが起きたら false を返す．
bool
SimpleHandler::read_attr(Token attr_token)
{
  if ( !expect(COLON) ) {
    return false;
  }

  Token value = parser().read_token(mSymbolMode);

  if ( debug() ) {
    cout << attr_token << " : " << value << endl;
  }

  PtNode* node = new_ptnode(attr_token, value);
  parent()->pt_node()->add_child(node);

  return expect_nl();
}


//////////////////////////////////////////////////////////////////////
// クラス ExprHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
ExprHandler::ExprHandler(DotlibParser& parser,
			 GroupHandler* parent) :
  DotlibHandler(parser, parent)
{
}

// @brief デストラクタ
ExprHandler::~ExprHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_token 属性名を表すトークン
// @return エラーが起きたら false を返す．
bool
ExprHandler::read_attr(Token attr_token)
{
  if ( !expect(COLON) ) {
    return false;
  }

  PtNode* expr_node = read_expr(SEMI);

  if ( debug() ) {
    cout << attr_token << " : " << endl;
  }

  PtNode* node = new_ptnode(attr_token, Token(EXPRESSION));
  node->add_child(expr_node);
  parent()->pt_node()->add_child(node);

  return expect_nl();
}

// @brief primary を読み込む．
PtNode*
ExprHandler::read_primary()
{
  Token token = parser().read_token();
  if ( token.type() == LP ) {
    return read_expr(RP);
  }
  if ( token.type() == SYMBOL ) {
    string name = token.value();
    if ( name != "VDD" && name != "VSS" && name != "VCC" ) {
      msg_mgr().put_msg(__FILE__, __LINE__, parser().cur_loc(),
			kMsgError,
			"DOTLIBPARSER",
			"Syntax error. "
			"Only 'VDD', 'VSS', and 'VCC' are allowed.");
      return NULL;
    }
  }
  else if ( token.type() != FLOAT_NUM && token.type() != INT_NUM ) {
    msg_mgr().put_msg(__FILE__, __LINE__, parser().cur_loc(),
		      kMsgError,
		      "DOTLIBPARSER",
		      "Syntax error. number is expected.");
    return NULL;
  }
  return new_ptnode(token, Token(EXPRESSION));
}

// @brief prudct を読み込む．
PtNode*
ExprHandler::read_product()
{
  for ( ; ; ) {
    PtNode* opr1 = read_primary();
    if ( opr1 == NULL ) {
      return NULL;
    }

    Token token = parser().read_token();
    if ( token.type() == MULT || token.type() == DIV ) {
      PtNode* opr2 = read_primary();
      if ( opr2 == NULL ) {
	return NULL;
      }

      PtNode* node = new_ptnode(token, Token(EXPRESSION));
      node->add_child(opr1);
      node->add_child(opr2);
      return node;
    }
    else {
      // token を戻す．
      parser().unget_token(token);
      return opr1;
    }
  }
}

// @brief expression を読み込む．
PtNode*
ExprHandler::read_expr(tTokenType end_marker)
{
  for ( ; ; ) {
    PtNode* opr1 = read_product();
    if ( opr1 == NULL ) {
      return NULL;
    }

    Token token = parser().read_token();
    if ( token.type() == end_marker ) {
      return opr1;
    }
    else if ( token.type() == PLUS || token.type() == MINUS ) {
      PtNode* opr2 = read_product();
      if ( opr2 == NULL ) {
	return NULL;
      }

      PtNode* node = new_ptnode(token, Token(EXPRESSION));
      node->add_child(opr1);
      node->add_child(opr2);
      return node;
    }
    else {
      msg_mgr().put_msg(__FILE__, __LINE__, parser().cur_loc(),
			kMsgError,
			"DOTLIBPARSER",
			"Syntax error.");
      return NULL;
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
ComplexHandler::ComplexHandler(DotlibParser& parser,
			       GroupHandler* parent) :
  DotlibHandler(parser, parent)
{
}

// @brief デストラクタ
ComplexHandler::~ComplexHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_token 属性名を表すトークン
// @return エラーが起きたら false を返す．
bool
ComplexHandler::read_attr(Token attr_token)
{
  if ( !expect(LP) ) {
    return false;
  }

  vector<Token> value_list;
  Token token = parser().read_token();
  if ( token.type() != RP ) {
    for ( ; ; ) {
      value_list.push_back(token);

      Token token1 = parser().read_token();
      if ( token1.type() == RP ) {
	break;
      }
      if ( token1.type() != COMMA ) {
	msg_mgr().put_msg(__FILE__, __LINE__, parser().cur_loc(),
			  kMsgError,
			  "DOTLIB_PARSER",
			  "syntax error. ',' is expected.");
	return false;
      }
      token = parser().read_token();
    }
  }

  if ( debug() ) {
    cout << attr_token << " (";
    const char* comma = "";
    for (vector<Token>::const_iterator p = value_list.begin();
	 p != value_list.end(); ++ p) {
      cout << comma << *p;
      comma = ", ";
    }
    cout << ")" << endl;
  }

  PtNode* node = new_ptnode(attr_token, value_list);
  parent()->pt_node()->add_child(node);

  return expect_nl();
}


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
  if ( !expect(LP) ) {
    return false;
  }

  vector<Token> value_list;
  Token token = parser().read_token();
  if ( token.type() != RP ) {
    for ( ; ; ) {
      value_list.push_back(token);

      Token token1 = parser().read_token();
      if ( token1.type() == RP ) {
	break;
      }
      if ( token1.type() != COMMA ) {
	msg_mgr().put_msg(__FILE__, __LINE__, parser().cur_loc(),
			  kMsgError,
			  "DOTLIB_PARSER",
			  "syntax error. ',' is expected.");
	return false;
      }
      token = parser().read_token();
    }
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
    token = parser().read_token();
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


//////////////////////////////////////////////////////////////////////
// クラス Token
//////////////////////////////////////////////////////////////////////

// @relates Token
// @brief 内容をストリームに出力する．
ostream&
operator<<(ostream& s,
	   Token token)
{
  const char* type_str = NULL;
  switch ( token.type() ) {
  case COLON:      type_str = "':'"; break;
  case SEMI:       type_str = "';'"; break;
  case COMMA:      type_str = "','"; break;
  case PLUS:       type_str = "'+'"; break;
  case MINUS:      type_str = "'-'"; break;
  case MULT:       type_str = "'*'"; break;
  case DIV:        type_str = "'/'"; break;
  case LP:         type_str = "'('"; break;
  case RP:         type_str = "')'"; break;
  case LCB:        type_str = "'{'"; break;
  case RCB:        type_str = "'}'"; break;
  case INT_NUM:    type_str = "INT"; break;
  case FLOAT_NUM:  type_str = "FLOAT"; break;
  case SYMBOL:     type_str = "SYMBOL"; break;
  case EXPRESSION: type_str = "EXPRESSION"; break;
  case NL:         type_str = "new-line"; break;
  case ERROR :     assert_not_reached(__FILE__, __LINE__);
  case END:        assert_not_reached(__FILE__, __LINE__);
  }
  s << type_str;
  if ( token.type() == INT_NUM ||
       token.type() == FLOAT_NUM ||
       token.type() == SYMBOL ) {
    s << "(" << token.value() << ")";
  }

  return s;
}

END_NAMESPACE_YM_CELL_DOTLIB
