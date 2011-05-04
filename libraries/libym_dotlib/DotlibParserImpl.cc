
/// @file libym_dotlib/DotlibParserImpl.cc
/// @brief DotlibParserImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotlibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibParserImpl.h"
#include "DotlibHandler.h"
#include "HandlerFactory.h"
#include "ym_utils/MsgMgr.h"
#include "ym_utils/ShString.h"
#include "PtNodeImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

// @brief コンストラクタ
DotlibParserImpl::DotlibParserImpl() :
  mLibraryHandler( HandlerFactory::new_library(*this) )
{
}

// デストラクタ
DotlibParserImpl::~DotlibParserImpl()
{
  delete mLibraryHandler;
}

// @brief ファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] debug デバッグモード
// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
// @return パース木の根のノードを返す．
// @note エラーが起きたら NULL を返す．
const DotlibNode*
DotlibParserImpl::read_file(const string& filename,
			    bool debug,
			    bool allow_no_semi)
{
  mDebug = debug;
  mAllowNoSemi = allow_no_semi;

  if ( !mScanner.open_file(filename) ) {
    ostringstream buf;
    buf << filename << ": Could not open.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "DOTLIB_PARSER",
		    buf.str());
    return NULL;
  }

  bool error = false;
  tTokenType type;
  FileRegion loc;
  // 空行を読み飛ばす．
  for (type = read_token(loc); type == NL; type = read_token(loc)) { }
  ShString name(cur_string());
  if ( type != SYMBOL || name != "library" ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc,
		    kMsgError,
		    "DOTLIB_PARSER",
		    "'library' keyword is expected "
		    "on the top of the structure");
    error = true;
    goto last;
  }

  if ( !mLibraryHandler->read_attr(name, loc) ) {
    error = true;
    goto last;
  }

  if ( !expect_nl() ) {
    error = true;
    goto last;
  }
  for ( ; ; ) {
    tTokenType type = read_token(loc);
    if ( type == END ) {
      break;
    }
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc,
		    kMsgWarning,
		    "DOTLIB_PARSER",
		    "contents after library group are ignored.");
  }

last:
  mScanner.close_file();

  if ( error ) {
    return NULL;
  }

  const DotlibNode* library = mLibraryHandler->pt_node();
  const DotlibNode* root = mPtMgr.new_attr(ShString("library"),
					   library, library->loc());
  return root;
}

// @brief 直前の read_file() で確保したパース木を解放する．
void
DotlibParserImpl::clear_node()
{
  mPtMgr.clear();
}

// @brief メモリ使用量のサマリを出力する．
// @param[in] s 出力先のストリーム
void
DotlibParserImpl::show_stats(ostream& s)
{
  mPtMgr.show_stats(s);
}

// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
bool
DotlibParserImpl::expect(tTokenType req_type)
{
  FileRegion loc;
  tTokenType type = read_token(loc);
  if ( type == req_type ) {
    return true;
  }

  const char* type_str = NULL;
  switch ( req_type ) {
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
  case SYMBOL:     type_str = "STR"; break;
  case INT_NUM:    type_str = "INT"; break;
  case FLOAT_NUM:  type_str = "FLOAT"; break;
  case EXPRESSION: type_str = "EXPRESSION"; break;
  case NL:         type_str = "new-line"; break;
  case ERROR:      assert_not_reached(__FILE__, __LINE__);
  case END:        assert_not_reached(__FILE__, __LINE__);
  }
  ostringstream buf;
  buf << "syntax error. " << type_str << " is expected.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  loc,
		  kMsgError,
		  "DOTLIB_PARSER",
		  buf.str());
  return false;
}

// @brief 行末まで読み込む．
bool
DotlibParserImpl::expect_nl()
{
  if ( mAllowNoSemi ) {
    FileRegion loc;
    tTokenType type = read_token(loc);
    if ( type == SEMI ) {
      type = read_token(loc);
    }
    if ( type == NL || type == END ) {
      return true;
    }
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc,
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. Semicolon is expected.");
    return false;
  }
  else {
    if ( !expect(SEMI) ) {
      return false;
    }
    if ( !expect(NL) ) {
      return false;
    }
  }
  return true;
}

// @brief パース木を管理するオブジェクトを返す．
PtMgr&
DotlibParserImpl::pt_mgr()
{
  return mPtMgr;
}

// @brief デバッグモードの時 true を返す．
bool
DotlibParserImpl::debug()
{
  return mDebug;
}

END_NAMESPACE_YM_DOTLIB
