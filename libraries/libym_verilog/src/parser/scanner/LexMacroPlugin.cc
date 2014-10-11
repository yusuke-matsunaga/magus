
/// @file LexMacroPlugin.cc
/// @brief LexMacroPlugin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LexMacroPlugin.h"
#include "RawLex.h"
#include "MacroSource.h"
#include "TokenInfo.h"

#include "parser_common.h"
#include "print_token.h"

#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// @class LpDefine
// @ingroup VlParser
// @brief `define 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
LpDefine::LpDefine(RawLex& lex,
		   const char* name) :
  LexPlugin(lex, name)
{
}

// @brief デストラクタ
LpDefine::~LpDefine()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpDefine::parse()
{
  // 次の非空白文字が IDENTIFIER でなければエラー
  if ( !expect(IDENTIFIER) ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    kMsgError,
		    "LEX",
		    "Syntax error: "
		    "expecting an identifier after `define.");
    return false;
  }

  if ( !lex().check_pluginname(cur_string()) ) {
    // 定義済みのコンパイラディレクティブと同名のマクロ
    ostringstream buf;
    buf << "Could not overwrite predefined compiler directive: "
	<< cur_string() << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    kMsgError,
		    "LEX",
		    buf.str());
    return false;
  }

  StrBuff defsymbol = cur_string();
  FileRegion macro_loc = cur_token_loc();

  // パラメータ名をキーにして位置番号を格納する連想配列
  HashMap<string, ymuint> param_dic;

  // ここは空白が重要なので get_raw_token() を呼ぶ．
  int id = get_raw_token();
  if ( id == SPACE || id == NL || id == COMMENT1 || id == COMMENT2 || id == EOF ) {
    // パラメータなし
  }
  else if ( id == '(' ) {
    // パラメータあり
    // パラメータを param_dic に記録
    ymuint pos = 0;
    if ( !expect(IDENTIFIER) ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      cur_token_loc(),
		      kMsgError,
		      "LEX",
		      "Syntax error: "
		      "expecting an identifier after '('.");
      return false;
    }
    param_dic.add(cur_string(), pos);
    ++ pos;
    for ( bool go = true; go; ) {
      int id = get_nospace_token();
      switch ( id ) {
      case ')':
	go = false;
	break;

      case ',':
	if ( !expect(IDENTIFIER) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  cur_token_loc(),
			  kMsgError,
			  "LEX",
			  "Syntax error: "
			  "expecting an identifier after ','.");
	  return false;
	}
	param_dic.add(cur_string(), pos);
	++ pos;
	break;

      default:
	MsgMgr::put_msg(__FILE__, __LINE__,
			cur_token_loc(),
			kMsgError,
			"LEX",
			"Syntax error: "
			"expecting ')' or ',' after an identifier.");
	return false;
      }
    }
  }
  else {
    // それ以外はエラー
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    kMsgError,
		    "LEX",
		    "Syntax error: "
		    "expecting '(' or SPACE after a macro-name.");
    return false;
  }

  // マクロをプラグインとして生成
  const char* macroname = defsymbol.c_str();
  ymuint n = param_dic.num();
  LpMacro* macro = new LpMacro(lex(), macroname, n);

  // マクロ本体を macro に記録
  if ( id != NL && id != EOF ) {
    for (int id = get_nospace_token();
	 id != NL && id != EOF;
	 id = get_nospace_token()) {
      ymuint pos;
      if ( id == IDENTIFIER &&
	   param_dic.find(cur_string(), pos) ) {
	// 置き換え対象のパラメータ
	macro->mTokenList.add(pos);
      }
      else {
	// それ以外のトークンはそのまま記録
	macro->mTokenList.add(cur_token_loc(), id, cur_string());
      }
    }
  }

  if ( debug() ) {
    ostringstream buf;
    buf << "registering new macro \"" << macroname << "\"" << endl
	<< "# of parameters : " << n << endl;
    string sep = "";
    for (const TokenInfo* token = macro->mTokenList.top();
	 token; token = token->next()) {
      buf << sep;
      sep = ", ";
      int pos = token->pos();
      if ( pos >= 0 ) {
	buf << "PARAM#" << pos;
      }
      else {
	buf << *token;
      }
    }
    MsgMgr::put_msg(__FILE__, __LINE__,
		    macro_loc,
		    kMsgDebug,
		    "LEX",
		    buf.str());
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpUndef
// @ingroup VlParser
// @brief `undef 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
LpUndef::LpUndef(RawLex& lex,
		 const char* name) :
  LexPlugin(lex, name)
{
}

// @brief デストラクタ
LpUndef::~LpUndef()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpUndef::parse()
{
  if ( !expect(IDENTIFIER) ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    kMsgError,
		    "LEX",
		    "Syntax error: "
		    "expecting an identifier after `undef.");
    return false;
  }

  StrBuff macroname = cur_string();
  FileRegion cur_loc = cur_token_loc();

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    kMsgError,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after macro-name.");
    return false;
  }

  if ( lex().erase_plugin(macroname.c_str()) ) {
    if ( debug() ) {
      ostringstream buf;
      buf << "forgetting a macro \"" << macroname << "\".";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      cur_loc,
		      kMsgDebug,
		      "LEX",
		      buf.str());
    }
  }
  else {
    // warning にするんだそうだ
    ostringstream buf;
    buf << "macro `" << macroname << " is not defined.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc,
		    kMsgWarning,
		    "LEX",
		    buf.str());
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpMacro
// @ingroup VlParser
// @brief マクロ置換用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex オブジェクト
// @param[in] name compiler directive 名
// @param[in] num_param パラメータ数
LpMacro::LpMacro(RawLex& lex,
		 const char* name,
		 ymuint num_param) :
  LexPlugin(lex, name),
  mNumParam(num_param)
{
}

// @brief デストラクタ
LpMacro::~LpMacro()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpMacro::parse()
{
  // self-loop のチェックを行う．
  if ( lex().check_macro(name()) ) {
    ostringstream buf;
    buf << "macro `" << name() << " depends on itself.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    kMsgError,
		    "LEX",
		    buf.str());
    return false;
  }

  if ( debug() ) {
    ostringstream buf;
    buf << "macro `" << name() << " found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    kMsgDebug,
		    "LEX",
		    buf.str());
  }

  // マクロ置換用のデータを作る．
  TokenList* param_array = NULL;
  if ( mNumParam > 0 ) {
    param_array = new TokenList[mNumParam];
    if ( !expect('(') ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      cur_token_loc(),
		      kMsgError,
		      "LEX",
		      "Syntax error: "
		      "'(' is expected.");
      return false;
    }
    ymuint pos = 0;
    for (bool go = true; go && pos < mNumParam; ) {
      int id = get_nospace_token();
      switch ( id ) {
      case ')':
	go = false;
	++ pos;
	break;
      case ',':
	++ pos;
	break;
      default:
	param_array[pos].add(cur_token_loc(), id, cur_string());
	break;
      }
    }
    if ( pos != mNumParam ) {
      // パラメータの数が合わない
      ostringstream buf;
      buf << "# of actual parameters ("
	  << pos << ") differs from"
	  << " # of formal paramters ("
	  << mNumParam << ").";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      cur_token_loc(),
		      kMsgError,
		      "LEX",
		      buf.str());
      return false;
    }
  }
  lex().push_macro(name(), mTokenList.top(), param_array);

  return true;
}

// @brief マクロの時 true を返す仮想関数
// @note デフォルトの実装では false を返す．
bool
LpMacro::is_macro()
{
  return true;
}

END_NAMESPACE_YM_VERILOG
