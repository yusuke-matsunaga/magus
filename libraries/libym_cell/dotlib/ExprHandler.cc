
/// @file libym_cell/dotlib/ExprHandler.cc
/// @brief ExprHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ExprHandler.h"
#include "DotlibParser.h"
#include "GroupHandler.h"
#include "PtMgr.h"
#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス ExprHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
ExprHandler::ExprHandler(GroupHandler* parent) :
  DotlibHandler(parent->parser(), parent->ptmgr(), parent),
  mUngetType(ERROR)
{
}

// @brief デストラクタ
ExprHandler::~ExprHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @return エラーが起きたら false を返す．
bool
ExprHandler::read_attr(const string& attr_name,
		       const FileRegion& attr_loc)
{
  if ( !expect(COLON) ) {
    return false;
  }

  PtValue* expr = read_expr(SEMI);

  if ( debug() ) {
    cout << attr_name << " : " << expr << endl;
  }

  PtNode* node = ptmgr().new_ptsimple(attr_name, attr_loc, expr);
  parent()->pt_node()->add_child(node);

  return expect_nl();
}

// @brief primary を読み込む．
PtValue*
ExprHandler::read_primary()
{
  tTokenType type = parser().read_token();
  if ( type == LP ) {
    return read_expr(RP);
  }
  FileRegion loc = parser().cur_loc();
  if ( type == SYMBOL ) {
    string name = parser().cur_string();
    if ( name != "VDD" && name != "VSS" && name != "VCC" ) {
      put_msg(__FILE__, __LINE__, loc,
	      kMsgError,
	      "DOTLIB_PARSER",
	      "Syntax error. "
	      "Only 'VDD', 'VSS', and 'VCC' are allowed.");
      return NULL;
    }
    return ptmgr().new_value(name, loc);
  }
  if ( type == FLOAT_NUM || type == INT_NUM ) {
    return ptmgr().new_value(parser().cur_float(), loc);
  }

  put_msg(__FILE__, __LINE__, loc,
	  kMsgError,
	  "DOTLIB_PARSER",
	  "Syntax error. number is expected.");
  return NULL;
}

// @brief prudct を読み込む．
PtValue*
ExprHandler::read_product()
{
  PtValue* opr1 = read_primary();
  if ( opr1 == NULL ) {
    return NULL;
  }

  for ( ; ; ) {
    tTokenType type = parser().read_token();
    if ( type == MULT || type == DIV ) {
      PtValue* opr2 = read_primary();
      if ( opr2 == NULL ) {
	return NULL;
      }
      opr1 = ptmgr().new_value(type, opr1, opr2);
    }
    else {
      // token を戻す．
      mUngetType = type;
      mUngetLoc = parser().cur_loc();
      return opr1;
    }
  }
}

// @brief expression を読み込む．
PtValue*
ExprHandler::read_expr(tTokenType end_marker)
{
  // ここだけ mUngetType, mUngetLoc を考慮する必要があるので
  // parser().read_token(), parser().cur_loc() を読んではいけない．

  PtValue* opr1 = read_product();
  if ( opr1 == NULL ) {
    return NULL;
  }
  for ( ; ; ) {
    tTokenType type = read_token();
    if ( type == end_marker ) {
      return opr1;
    }
    if ( type == PLUS || type == MINUS ) {
      PtValue* opr2 = read_product();
      if ( opr2 == NULL ) {
	return NULL;
      }

      opr1 = ptmgr().new_value(type, opr1, opr2);
    }
    else {
      put_msg(__FILE__, __LINE__, cur_loc(),
	      kMsgError,
	      "DOTLIB_PARSER",
	      "Syntax error.");
      return NULL;
    }
  }
}

// @brief トークンを読み込む．
tTokenType
ExprHandler::read_token()
{
  if ( mUngetType != ERROR ) {
    tTokenType ans = mUngetType;
    mCurLoc = mUngetLoc;
    mUngetType = ERROR;
    return ans;
  }
  else {
    tTokenType ans = parser().read_token();
    mCurLoc = parser().cur_loc();
    return ans;
  }
}

// @brief 直前に読み込んだトークンの位置を返す．
FileRegion
ExprHandler::cur_loc()
{
  return mCurLoc;
}

END_NAMESPACE_YM_CELL_DOTLIB
