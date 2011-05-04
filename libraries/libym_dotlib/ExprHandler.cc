
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
#include "DotlibNodeImpl.h"
#include "ym_utils/MsgMgr.h"


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
// @return 値を表す DotlibNode を返す．
// @note エラーが起きたら NULL を返す．
// @note ここでは expression のパースを行う．
DotlibNodeImpl*
ExprHandler::read_value()
{
  DotlibNodeImpl* expr = read_expr(SEMI);

  return expr;
}

// @brief primary を読み込む．
DotlibNodeImpl*
ExprHandler::read_primary()
{
  FileRegion loc;
  tTokenType type = parser().read_token(loc);
  if ( type == LP ) {
    return read_expr(RP);
  }
  if ( type == SYMBOL ) {
    ShString name(parser().cur_string());
    if ( name != "VDD" &&
	 name != "VSS" &&
	 name != "VCC" ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error. "
		      "Only 'VDD', 'VSS', and 'VCC' are allowed.");
      return NULL;
    }
    return pt_mgr().new_string(name, loc);
  }
  if ( type == FLOAT_NUM || type == INT_NUM ) {
    return pt_mgr().new_float(parser().cur_float(), loc);
  }

  MsgMgr::put_msg(__FILE__, __LINE__,
		  loc,
		  kMsgError,
		  "DOTLIB_PARSER",
		  "Syntax error. number is expected.");
  return NULL;
}

// @brief prudct を読み込む．
DotlibNodeImpl*
ExprHandler::read_product()
{
  DotlibNodeImpl* opr1 = read_primary();
  if ( opr1 == NULL ) {
    return NULL;
  }

  for ( ; ; ) {
    FileRegion loc;
    tTokenType type = parser().read_token(loc);
    if ( type == MULT || type == DIV ) {
      DotlibNodeImpl* opr2 = read_primary();
      if ( opr2 == NULL ) {
	return NULL;
      }
      if ( type == MULT ) {
	opr1 = pt_mgr().new_mult(opr1, opr2);
      }
      else {
	opr1 = pt_mgr().new_div(opr1, opr2);
      }
    }
    else {
      // token を戻す．
      mUngetType = type;
      mUngetLoc = loc;
      return opr1;
    }
  }
}

// @brief expression を読み込む．
DotlibNodeImpl*
ExprHandler::read_expr(tTokenType end_marker)
{
  // ここだけ mUngetType, mUngetLoc を考慮する必要があるので
  // じかに parser().read_token() を呼んではいけない．

  DotlibNodeImpl* opr1 = read_product();
  if ( opr1 == NULL ) {
    return NULL;
  }
  for ( ; ; ) {
    FileRegion loc;
    tTokenType type = read_token(loc);
    if ( type == end_marker ) {
      return opr1;
    }
    if ( type == PLUS || type == MINUS ) {
      DotlibNodeImpl* opr2 = read_product();
      if ( opr2 == NULL ) {
	return NULL;
      }
      if ( type == PLUS ) {
	opr1 = pt_mgr().new_plus(opr1, opr2);
      }
      else {
	opr1 = pt_mgr().new_minus(opr1, opr2);
      }
    }
    else {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error.");
      return NULL;
    }
  }
}

// @brief トークンを読み込む．
// @param[out] loc 対応するファイル上の位置情報を格納する変数
tTokenType
ExprHandler::read_token(FileRegion& loc)
{
  if ( mUngetType != ERROR ) {
    tTokenType ans = mUngetType;
    loc = mUngetLoc;
    mUngetType = ERROR;
    return ans;
  }
  else {
    return parser().read_token(loc);
  }
}

END_NAMESPACE_YM_DOTLIB
