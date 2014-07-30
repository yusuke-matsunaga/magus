
/// @file FuncHandler.cc
/// @brief FuncHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FuncHandler.h"
#include "DotlibParserImpl.h"
#include "DotlibMgrImpl.h"
#include "GroupHandler.h"
#include "DotlibNodeImpl.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス FuncHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
FuncHandler::FuncHandler(GroupHandler* parent) :
  SimpleHandler(parent, false)
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
    int v = mScanner.cur_int();
    if ( v != 0 && v != 1 ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error. 0 or 1 is expected.");
      return NULL;
    }
    return mgr()->new_int(v, loc);
  }

  MsgMgr::put_msg(__FILE__, __LINE__,
		  loc,
		  kMsgError,
		  "DOTLIB_PARSER",
		  "Syntax error. number is expected.");
  return NULL;
}

// @brief プライム付きの primary を読み込む．
DotlibNodeImpl*
FuncHandler::read_primary2()
{
  FileRegion loc;
  tTokenType type = read_token(loc);
  if ( type == NOT ) {
    DotlibNodeImpl* opr = read_primary();
    if ( opr == NULL ) {
      return NULL;
    }
    return mgr()->new_not(opr, FileRegion(loc, opr->loc()));
  }
  unget_token(type, loc);

  DotlibNodeImpl* node = read_primary();
  if ( node == NULL ) {
    return NULL;
  }

  type = read_token(loc);
  if ( type == PRIME ) {
    return mgr()->new_not(node, FileRegion(node->loc(), loc));
  }
  unget_token(type, loc);

  return node;
}

// @brief product を読み込む．
DotlibNodeImpl*
FuncHandler::read_product()
{
  DotlibNodeImpl* opr1 = read_primary2();
  if ( opr1 == NULL ) {
    return NULL;
  }

  for ( ; ; ) {
    FileRegion loc;
    tTokenType type = read_token(loc);
    if ( type == AND ) {
      DotlibNodeImpl* opr2 = read_primary2();
      if ( opr2 == NULL ) {
	return NULL;
      }
      opr1 = mgr()->new_and(opr1, opr2);
    }
    else if ( type == NOT || type == LP || type == SYMBOL ) {
      unget_token(type, loc);
      DotlibNodeImpl* opr2 = read_primary2();
      if ( opr2 == NULL ) {
	return NULL;
      }
      opr1 = mgr()->new_and(opr1, opr2);
    }
    else {
      // token を戻す．
      unget_token(type, loc);
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
      if ( type == XOR ) {
	opr1 = mgr()->new_xor(opr1, opr2);
      }
      else {
	opr1 = mgr()->new_or(opr1, opr2);
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

// @brief 読み込んだトークンを戻す．
// @param[in] type トークンの型
// @param[in] loc トークンの位置
void
FuncHandler::unget_token(tTokenType type,
			 const FileRegion& loc)
{
  mUngetType = type;
  mUngetLoc = loc;
}

END_NAMESPACE_YM_DOTLIB
