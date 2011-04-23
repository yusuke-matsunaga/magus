
/// @file libym_cell/dotlib/SimpleHandler.cc
/// @brief SimpleHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SimpleHandler.h"
#include "DotlibParser.h"
#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

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

END_NAMESPACE_YM_CELL_DOTLIB
