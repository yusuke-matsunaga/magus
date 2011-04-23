
/// @file libym_cell/dotlib/DotlibHandler.cc
/// @brief DotlibHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"
#include "DotlibParser.h"
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

// @brief group attribute 用のパースを行う．
// @param[out] value_list 読み込んだトークンを格納するリスト
bool
DotlibHandler::parse_complex(vector<Token>& value_list)
{
  if ( !expect(LP) ) {
    return false;
  }

  value_list.clear();
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
  return true;
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
