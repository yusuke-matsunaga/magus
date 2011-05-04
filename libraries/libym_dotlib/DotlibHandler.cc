
/// @file libym_dotlib/DotlibHandler.cc
/// @brief DotlibHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"
#include "DotlibParserImpl.h"
#include "GroupHandler.h"
#include "PtMgr.h"
#include "DotlibNodeImpl.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibHandler
//////////////////////////////////////////////////////////////////////

// @brief 親のハンドラを持たない場合のコンストラクタ
// @param[in] parser パーサー
DotlibHandler::DotlibHandler(DotlibParserImpl& parser) :
  mParser(parser),
  mParent(NULL)
{
}

// @brief 親のハンドラを持つ場合のコンストラクタ
// @param[in] parent 親のハンドラ
DotlibHandler::DotlibHandler(GroupHandler* parent) :
  mParser(parent->parser()),
  mParent(parent)
{
}

// @brief デストラクタ
DotlibHandler::~DotlibHandler()
{
}

// @brief 対応するノードを得る．
// @note デフォルトの実装は NULL を返す．
const DotlibNode*
DotlibHandler::pt_node()
{
  return NULL;
}

// @brief group attribute 用のパースを行う．
// @return 読み込んだ値(リスト)を返す．
// @note エラーが起きたら NULL を返す．
DotlibNodeImpl*
DotlibHandler::parse_complex()
{
  if ( !expect(LP) ) {
    return NULL;
  }

  DotlibNodeImpl* value_list = pt_mgr().new_list();
  FileRegion loc;
  tTokenType type = parser().read_token(loc);
  if ( type != RP ) {
    for ( ; ; ) {
      DotlibNodeImpl* value = new_ptvalue(type, loc);
      if ( value == NULL ) {
	return NULL;
      }

      value_list->add_node(value);

      tTokenType type1 = parser().read_token(loc);
      if ( type1 == RP ) {
	break;
      }
      if ( type1 != COMMA ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			loc,
			kMsgError,
			"DOTLIB_PARSER",
			"syntax error. ',' is expected.");
	return NULL;
      }
      type = parser().read_token(loc);
    }
  }

  return value_list;
}

// @brief DotlibNode を生成する．
// @param[in] type 型
// @param[in] loc ファイル上の位置情報
// @note 残りの情報は parser() からとってくる．
DotlibNodeImpl*
DotlibHandler::new_ptvalue(tTokenType type,
			   const FileRegion& loc)
{
  switch ( type ) {
  case INT_NUM:
    return pt_mgr().new_int(parser().cur_int(), loc);
    break;

  case FLOAT_NUM:
    return pt_mgr().new_float(parser().cur_float(), loc);
    break;

  case SYMBOL:
    return pt_mgr().new_string(ShString(parser().cur_string()), loc);
    break;

  default:
    break;
  }
  MsgMgr::put_msg(__FILE__, __LINE__,
		  loc,
		  kMsgError,
		  "DOTLIB_PARSER",
		  "Syntax error. int/float/string value is expected.");
  return NULL;
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

// @brief パーサーを得る．
DotlibParserImpl&
DotlibHandler::parser()
{
  return mParser;
}

// @brief PtMgr を得る．
PtMgr&
DotlibHandler::pt_mgr()
{
  return mParser.pt_mgr();
}

// @brief 親のハンドラを得る．
GroupHandler*
DotlibHandler::parent()
{
  return mParent;
}

// @brief デバッグモードの時に true を返す．
bool
DotlibHandler::debug()
{
  return mParser.debug();
}

// @brief tTokenType 内容をストリームに出力する．
ostream&
operator<<(ostream& s,
	   tTokenType type)
{
  const char* type_str = NULL;
  switch ( type ) {
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
  case ERROR :     type_str = "error"; break;
  case END:        type_str = "end-of-file"; break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  s << type_str;
  return s;
}

END_NAMESPACE_YM_DOTLIB
