
/// @file libym_dotlib/ExprHandler.cc
/// @brief ExprHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ExprHandler.h"
#include "DotlibParserImpl.h"
#include "GroupHandler.h"
#include "PtMgr.h"
#include "PtNodeImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス ExprHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
ExprHandler::ExprHandler(GroupHandler* parent) :
  SimpleHandler(parent),
  mUngetType(ERROR)
{
}

// @brief デストラクタ
ExprHandler::~ExprHandler()
{
}

// @brief 値を読み込む処理
// @return 値を表す PtNode を返す．
// @note エラーが起きたら NULL を返す．
// @note ここでは expression のパースを行う．
PtNodeImpl*
ExprHandler::read_value()
{
  PtNodeImpl* expr = read_expr(SEMI);

  return expr;
}

// @brief primary を読み込む．
PtNodeImpl*
ExprHandler::read_primary()
{
  tTokenType type = parser().read_token();
  if ( type == LP ) {
    return read_expr(RP);
  }
  FileRegion loc = parser().cur_loc();
  if ( type == SYMBOL ) {
    ShString name(parser().cur_string());
    if ( name != "VDD" &&
	 name != "VSS" &&
	 name != "VCC" ) {
      put_msg(__FILE__, __LINE__, loc,
	      kMsgError,
	      "DOTLIB_PARSER",
	      "Syntax error. "
	      "Only 'VDD', 'VSS', and 'VCC' are allowed.");
      return NULL;
    }
    return ptmgr().new_string(name, loc);
  }
  if ( type == FLOAT_NUM || type == INT_NUM ) {
    return ptmgr().new_float(parser().cur_float(), loc);
  }

  put_msg(__FILE__, __LINE__, loc,
	  kMsgError,
	  "DOTLIB_PARSER",
	  "Syntax error. number is expected.");
  return NULL;
}

// @brief prudct を読み込む．
PtNodeImpl*
ExprHandler::read_product()
{
  PtNodeImpl* opr1 = read_primary();
  if ( opr1 == NULL ) {
    return NULL;
  }

  for ( ; ; ) {
    tTokenType type = parser().read_token();
    if ( type == MULT || type == DIV ) {
      PtNodeImpl* opr2 = read_primary();
      if ( opr2 == NULL ) {
	return NULL;
      }
      if ( type == MULT ) {
	opr1 = ptmgr().new_mult(opr1, opr2);
      }
      else {
	opr1 = ptmgr().new_div(opr1, opr2);
      }
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
PtNodeImpl*
ExprHandler::read_expr(tTokenType end_marker)
{
  // ここだけ mUngetType, mUngetLoc を考慮する必要があるので
  // parser().read_token(), parser().cur_loc() を読んではいけない．

  PtNodeImpl* opr1 = read_product();
  if ( opr1 == NULL ) {
    return NULL;
  }
  for ( ; ; ) {
    tTokenType type = read_token();
    if ( type == end_marker ) {
      return opr1;
    }
    if ( type == PLUS || type == MINUS ) {
      PtNodeImpl* opr2 = read_product();
      if ( opr2 == NULL ) {
	return NULL;
      }
      if ( type == PLUS ) {
	opr1 = ptmgr().new_plus(opr1, opr2);
      }
      else {
	opr1 = ptmgr().new_minus(opr1, opr2);
      }
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

END_NAMESPACE_YM_DOTLIB
