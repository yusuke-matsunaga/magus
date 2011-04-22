
/// @file libym_cell/dotlib/DotLibHandler.cc
/// @brief DotLibHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotLibHandler.h"
#include "DotLibParser.h"
#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"
#include "Token.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotLibHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
DotLibHandler::DotLibHandler(DotLibParser& parser,
			     GroupHandler* parent) :
  mParser(parser),
  mParent(parent)
{
}

// @brief デストラクタ
DotLibHandler::~DotLibHandler()
{
}

// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
bool
DotLibHandler::expect(tTokenType type)
{
  return mParser.expect(type);
}

// @brief 行末まで読み込む．
bool
DotLibHandler::expect_nl()
{
  return mParser.expect_nl();
}

// @brief 親のハンドラを得る．
GroupHandler*
DotLibHandler::parent()
{
  return mParent;
}

// @brief パーサーを得る．
DotLibParser&
DotLibHandler::parser()
{
  return mParser;
}

// @brief メッセージ出力管理オブジェクトを得る．
MsgMgr&
DotLibHandler::msg_mgr()
{
  return mParser.msg_mgr();
}

// @brief デバッグモードの時に true を返す．
bool
DotLibHandler::debug()
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
SimpleHandler::SimpleHandler(DotLibParser& parser,
			     GroupHandler* parent,
			     bool symbol_mode) :
  DotLibHandler(parser, parent),
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

#if 0
  cout << "attr = " << attr_token << endl;
  switch ( mReqType ) {
  case kNormal: cout << "kNormal"; break;
  case kFloat:  cout << "kFloat"; break;
  default:      cout << "unknown"; break;
  }
  cout << endl;
#endif

  tTokenType type = parser().read_token(mSymbolMode);
  string str = parser().cur_string();
  FileRegion loc = parser().cur_loc();
  Token value(type, str, loc);

  if ( debug() ) {
    cout << attr_token << " : " << value << endl;
  }

  PtNode* node = new PtNode(attr_token, value);
  parent()->pt_node()->add_child(node);

  return expect_nl();
}


//////////////////////////////////////////////////////////////////////
// クラス ComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
ComplexHandler::ComplexHandler(DotLibParser& parser,
			       GroupHandler* parent) :
  DotLibHandler(parser, parent)
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
  tTokenType type = parser().read_token();
  if ( type != RP ) {
    for ( ; ; ) {
      string value = parser().cur_string();
      FileRegion loc = parser().cur_loc();
      value_list.push_back(Token(type, value, loc));

      tTokenType type1 = parser().read_token();
      if ( type1 == RP ) {
	break;
      }
      if ( type1 != COMMA ) {
	msg_mgr().put_msg(__FILE__, __LINE__, parser().cur_loc(),
			  kMsgError,
			  "DOTLIB_PARSER",
			  "syntax error. ',' is expected.");
	return false;
      }
      type = parser().read_token();
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

  PtNode* node = new PtNode(attr_token, value_list);
  parent()->pt_node()->add_child(node);

  return expect_nl();
}


//////////////////////////////////////////////////////////////////////
// クラス GroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] parent 親のハンドラ
GroupHandler::GroupHandler(DotLibParser& parser,
			   GroupHandler* parent) :
  DotLibHandler(parser, parent)
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
  tTokenType type = parser().read_token();
  if ( type != RP ) {
    for ( ; ; ) {
      string value = parser().cur_string();
      FileRegion loc = parser().cur_loc();
      value_list.push_back(Token(type, value, loc));

      tTokenType type1 = parser().read_token();
      if ( type1 == RP ) {
	break;
      }
      if ( type1 != COMMA ) {
	msg_mgr().put_msg(__FILE__, __LINE__, parser().cur_loc(),
			  kMsgError,
			  "DOTLIB_PARSER",
			  "syntax error. ',' is expected.");
	return false;
      }
      type = parser().read_token();
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
    type = parser().read_token();
    if ( type == NL ) {
      continue;
    }
    if ( type == RCB ) {
      break;
    }
    if ( type != SYMBOL ) {
      msg_mgr().put_msg(__FILE__, __LINE__, parser().cur_loc(),
			kMsgError,
			"DOTLIB_PARSER",
			"string value is expected.");
      return false;
    }
    const string name1 = parser().cur_string();
    FileRegion loc1 = parser().cur_loc();
    hash_map<string, DotLibHandler*>::const_iterator p
      = mHandlerMap.find(name1);
    if ( p == mHandlerMap.end() ) {
      ostringstream buf;
      buf << name1 << ": unknown keyword.";
      msg_mgr().put_msg(__FILE__, __LINE__, loc1,
			kMsgError,
			"DOTLIB_PARSER",
			buf.str());
      return false;
    }
    DotLibHandler* handler = p->second;
    if ( !handler->read_attr(Token(SYMBOL, name1, loc1)) ) {
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
			  DotLibHandler* handler)
{
  mHandlerMap.insert(make_pair(attr_name, handler));
  return true;
}

// @brief ハンドラを取り出す．
// @param[in] attr_name 属性名
// @note なければ NULL を返す．
DotLibHandler*
GroupHandler::find_handler(const string& attr_name)
{
  hash_map<string, DotLibHandler*>::const_iterator p
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
  PtNode* node = new PtNode(attr_token, value_list);
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
  case COLON:     type_str = "':'"; break;
  case SEMI:      type_str = "';'"; break;
  case COMMA:     type_str = "','"; break;
  case PLUS:      type_str = "'+'"; break;
  case MINUS:     type_str = "'-'"; break;
  case MULT:      type_str = "'*'"; break;
  case DIV:       type_str = "'/'"; break;
  case LP:        type_str = "'('"; break;
  case RP:        type_str = "')'"; break;
  case LCB:       type_str = "'{'"; break;
  case RCB:       type_str = "'}'"; break;
  case INT_NUM: type_str = "FLOAT"; break;
  case FLOAT_NUM: type_str = "FLOAT"; break;
  case SYMBOL:    type_str = "SYMBOL"; break;
  case NL:        type_str = "new-line"; break;
  case ERROR:     assert_not_reached(__FILE__, __LINE__);
  case END:       assert_not_reached(__FILE__, __LINE__);
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
