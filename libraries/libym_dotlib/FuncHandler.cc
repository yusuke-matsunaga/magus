
/// @file libym_dotlib/FuncHandler.cc
/// @brief FuncHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "FuncHandler.h"
#include "DotlibParserImpl.h"
#include "DotlibMgrImpl.h"
#include "GroupHandler.h"
#include "DotlibNodeImpl.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス FuncHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
FuncHandler::FuncHandler(GroupHandler* parent) :
  SimpleHandler(parent)
{
}

// @brief デストラクタ
FuncHandler::~FuncHandler()
{
}

// @brief 値を読み込む処理
// @return 値を表す DotlibNode を返す．
// @note エラーが起きたら NULL を返す．
// @note ここでは expression のパースを行う．
DotlibNodeImpl*
FuncHandler::read_value()
{
  FileRegion loc;
  tTokenType value_type = parser().read_token(loc, false);
  if ( value_type != SYMBOL ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc,
		    kMsgError,
		    "DOTLIB_PARSER",
		    "String value is expected.");
    return NULL;
  }

  mScanner.init(parser().cur_string(), loc);
  mUngetType = ERROR;

  DotlibNodeImpl* expr = read_expr(END);

  return expr;
}

// @brief primary を読み込む．
DotlibNodeImpl*
FuncHandler::read_primary()
{
  FileRegion loc;
  tTokenType type = read_token(loc);
  if ( type == LP ) {
    return read_expr(RP);
  }
  if ( type == SYMBOL ) {
    ShString name(mScanner.cur_string());
    return mgr()->new_string(name, loc);
  }
  if ( type == INT_NUM ) {
    return mgr()->new_int(mScanner.cur_int(), loc);
  }
  if ( type == NOT ) {
    DotlibNodeImpl* opr = read_primary();
    return mgr()->new_not(opr, FileRegion(loc, opr->loc()));
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
FuncHandler::read_product()
{
  DotlibNodeImpl* opr1 = read_primary();
  if ( opr1 == NULL ) {
    return NULL;
  }

  for ( ; ; ) {
    FileRegion loc;
    tTokenType type = read_token(loc);
    if ( type == AND ) {
      DotlibNodeImpl* opr2 = read_primary();
      if ( opr2 == NULL ) {
	return NULL;
      }
      opr1 = mgr()->new_and(opr1, opr2);
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
FuncHandler::read_expr(tTokenType end_marker)
{
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
    if ( type == OR || type == XOR ) {
      DotlibNodeImpl* opr2 = read_product();
      if ( opr2 == NULL ) {
	return NULL;
      }
      if ( type == OR ) {
	opr1 = mgr()->new_or(opr1, opr2);
      }
      else {
	opr1 = mgr()->new_xor(opr1, opr2);
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
FuncHandler::read_token(FileRegion& loc)
{
  if ( mUngetType != ERROR ) {
    tTokenType ans = mUngetType;
    loc = mUngetLoc;
    mUngetType = ERROR;
    return ans;
  }
  else {
    return mScanner.read_token(loc);
  }
}

END_NAMESPACE_YM_DOTLIB
