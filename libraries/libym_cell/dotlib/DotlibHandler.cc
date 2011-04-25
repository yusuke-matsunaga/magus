
/// @file libym_cell/dotlib/DotlibHandler.cc
/// @brief DotlibHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"
#include "DotlibParser.h"
#include "PtMgr.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
// @param[in] ptmgr パース木を管理するオブジェクト
// @param[in] parent 親のハンドラ
DotlibHandler::DotlibHandler(DotlibParser& parser,
			     PtMgr& ptmgr,
			     GroupHandler* parent) :
  mParser(parser),
  mPtMgr(ptmgr),
  mParent(parent)
{
}

// @brief デストラクタ
DotlibHandler::~DotlibHandler()
{
}

// @brief group attribute 用のパースを行う．
// @param[out] value_list 読み込んだトークンを格納するリスト
bool
DotlibHandler::parse_complex(vector<const PtValue*>& value_list)
{
  if ( !expect(LP) ) {
    return false;
  }

  value_list.clear();
  tTokenType type = parser().read_token();
  if ( type != RP ) {
    for ( ; ; ) {
      PtValue* value = NULL;
      FileRegion loc(parser().cur_loc());
      switch ( type ) {
      case INT_NUM:
	value = ptmgr().new_int(parser().cur_int(), loc);
	break;

      case FLOAT_NUM:
	value = ptmgr().new_float(parser().cur_float(), loc);
	break;

      case SYMBOL:
	value = ptmgr().new_string(ShString(parser().cur_string()), loc);
	break;

      default:
	put_msg(__FILE__, __LINE__, parser().cur_loc(),
		kMsgError,
		"DOTLIB_PARSER",
		"Syntax error. int/float/string value is expected.");
	return false;
      }
      value_list.push_back(value);

      tTokenType type1 = parser().read_token();
      if ( type1 == RP ) {
	break;
      }
      if ( type1 != COMMA ) {
	put_msg(__FILE__, __LINE__, parser().cur_loc(),
		kMsgError,
		"DOTLIB_PARSER",
		"syntax error. ',' is expected.");
	return false;
      }
      type = parser().read_token();
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

// @brief PtMgr を得る．
PtMgr&
DotlibHandler::ptmgr()
{
  return mPtMgr;
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
DotlibHandler::put_msg(const char* src_file,
		       int src_line,
		       const FileRegion& file_loc,
		       tMsgType type,
		       const char* label,
		       const char* msg)
{
  mParser.msg_mgr().put_msg(src_file, src_line, file_loc, type, label, msg);
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
DotlibHandler::put_msg(const char* src_file,
		       int src_line,
		       const FileRegion& file_loc,
		       tMsgType type,
		       const char* label,
		       const string& msg)
{
  mParser.msg_mgr().put_msg(src_file, src_line, file_loc, type, label, msg);
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

END_NAMESPACE_YM_CELL_DOTLIB
