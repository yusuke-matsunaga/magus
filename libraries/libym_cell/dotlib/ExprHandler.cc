
/// @file libym_cell/dotlib/ExprHandler.cc
/// @brief ExprHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ExprHandler.h"
#include "DotlibParser.h"
#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

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

END_NAMESPACE_YM_CELL_DOTLIB
