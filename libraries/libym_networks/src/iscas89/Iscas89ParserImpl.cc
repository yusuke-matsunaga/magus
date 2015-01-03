
/// @file Iscas89ParserImpl.cc
/// @brief BlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89ParserImpl.h"
#include "Iscas89Scanner.h"
#include "Iscas89Handler.h"
#include "YmUtils/FileIDO.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
// Iscas89ParserImpl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Iscas89ParserImpl::Iscas89ParserImpl()
{
  mScanner = NULL;
}

// デストラクタ
Iscas89ParserImpl::~Iscas89ParserImpl()
{
  for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    Iscas89Handler* handler = *p;
    delete handler;
  }
  delete mScanner;
}

// 読み込みを行なう．
//
// 文法：
//
// file   = { line } ;
// line   = input | output | buff | not | and | nand | or | nor | xor | xnor | dff ;
// input  = 'INPUT' '(' NAME ')'
// output = 'OUTPUT' '(' NAME ')'
// buff   = NAME '=' 'BUFF' '(' NAME ')'
// not    = NAME '=' 'NOT' '(' NAME ')'
// and    = NAME '=' 'AND' '(' NAME ',' NAME { ',' NAME } ')'
// nand   = NAME '=' 'NAND' '(' NAME ',' NAME { ',' NAME } ')'
// or     = NAME '=' 'OR' '(' NAME ',' NAME { ',' NAME } ')'
// nor    = NAME '=' 'NOR' '(' NAME ',' NAME { ',' NAME } ')'
// xor    = NAME '=' 'XOR' '(' NAME ',' NAME { ',' NAME } ')'
// xnor   = NAME '=' 'XNOR' '(' NAME ',' NAME { ',' NAME } ')'
// dff    = NAME '=' 'DFF' '(' NAME ')'
//
bool
Iscas89ParserImpl::read(const string& filename)
{
  // ファイルをオープンする．
  FileIDO ido;
  if ( !ido.open(filename) ) {
    // エラー
    ostringstream buf;
    buf << filename << " : No such file.";
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion(),
		    kMsgFailure, "BLIF_PARSER", buf.str());
    return false;
  }

  mScanner = new Iscas89Scanner(ido);

  for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    Iscas89Handler* handler = *p;
    if ( !handler->init() ) {
      delete mScanner;
      mScanner = NULL;
      return false;
    }
  }

  // パーサー本体
  bool go_on = true;
  bool has_error = false;
  while ( go_on ) {
    ymuint name_id;
    FileRegion first_loc;
    FileRegion last_loc;
    Token tok = read_token(name_id, first_loc);
    switch ( tok ) {
    case kToken_INPUT:
      if ( !parse_name(name_id, last_loc) ) {
	goto error;
      }
      if ( !read_input(FileRegion(first_loc, last_loc), name_id) ) {
	goto error;
      }
      break;

    case kToken_OUTPUT:
      if ( !parse_name(name_id, last_loc) ) {
	goto error;
      }
      if ( !read_output(FileRegion(first_loc, last_loc), name_id) ) {
	goto error;
      }
      break;

    case kToken_NAME:
      {
	ymuint cur_lval;
	FileRegion cur_loc;

	if ( !expect(kToken_EQ, cur_lval, cur_loc) ) {
	  goto error;
	}

	tIscas89GateType gate_type;
	if ( !parse_gate_type(gate_type) ) {
	  goto error;
	}

	vector<ymuint> iname_id_list;
	if ( !parse_name_list(iname_id_list, last_loc) ) {
	  goto error;
	}
	if ( !read_gate(FileRegion(first_loc, last_loc),
			name_id, gate_type, iname_id_list) ) {
	  goto error;
	}
      }
      break;

    case kToken_EOF:
      go_on = false;
      break;

    default:
      goto error;
    }
    continue;

  error:
    has_error = true;
    // ')' まで読み進める．
    while ( read_token(name_id, first_loc) != kToken_RPAR ) ;
  }

  delete mScanner;
  mScanner = NULL;

  if ( !has_error ) {
    // 成功
    for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      Iscas89Handler* handler = *p;
      handler->normal_exit();
    }
    mIdHash.clear();
    return true;
  }
  else {
    // 失敗
    for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      Iscas89Handler* handler = *p;
      handler->error_exit();
    }
    mIdHash.clear();
    return false;
  }
}

// @brief イベントハンドラの登録
void
Iscas89ParserImpl::add_handler(Iscas89Handler* handler)
{
  mHandlerList.push_back(handler);
  handler->mParser = this;
}

// @brief ゲート型を読み込む．
// @param[out] gate_type ゲート型を格納する変数．
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
//
// エラーが起きたらエラーメッセージをセットする．
bool
Iscas89ParserImpl::parse_gate_type(tIscas89GateType& gate_type)
{
  ymuint cur_lval;
  FileRegion cur_loc;

  Token tok = read_token(cur_lval, cur_loc);
  switch ( tok ) {
  case kToken_BUFF:
    gate_type = kGt89BUFF;
    break;

  case kToken_NOT:
    gate_type = kGt89NOT;
    break;

  case kToken_DFF:
    gate_type = kGt89DFF;
    break;

  case kToken_AND:
    gate_type = kGt89AND;
    break;

  case kToken_NAND:
    gate_type = kGt89NAND;
    break;

  case kToken_OR:
    gate_type = kGt89OR;
    break;

  case kToken_NOR:
    gate_type = kGt89NOR;
    break;

  case kToken_XOR:
    gate_type = kGt89XOR;
    break;

  case kToken_XNOR:
    gate_type = kGt89XNOR;
    break;

  default:
    // シンタックスエラー
    // トークンが期待値と異なっていた
    ostringstream buf;
    buf << "Syntax error: gate-type is expected.";
    MsgMgr::put_msg(__FILE__, __LINE__, cur_loc,
		    kMsgError,
		    "ER_SYNTAX02",
		    buf.str());
    return false;
  }

  return true;
}

// @brief '(' ')' で囲まれた名前を読み込む．
// @param[in] name_id 名前の識別子番号を格納する変数．
// @param[in] last_loc 末尾のファイル位置
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
//
// エラーが起きたらエラーメッセージをセットする．
bool
Iscas89ParserImpl::parse_name(ymuint& name_id,
			      FileRegion& last_loc)
{
  ymuint cur_lval;
  FileRegion cur_loc;

  if ( !expect(kToken_LPAR, cur_lval, cur_loc) ) {
    return false;
  }
  if ( !expect(kToken_NAME, name_id, cur_loc) ) {
    return false;
  }

  if ( !expect(kToken_RPAR, cur_lval, last_loc) ) {
    return false;
  }

  return true;
}

// @brief '(' ')' で囲まれた名前のリストを読み込む．
// @param[in] name_id_list 名前の識別子番号を格納するリスト．
// @param[in] last_loc 末尾のファイル位置
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
//
// エラーが起きたらエラーメッセージをセットする．
bool
Iscas89ParserImpl::parse_name_list(vector<ymuint>& name_id_list,
				   FileRegion& last_loc)
{
  ymuint cur_lval;
  FileRegion cur_loc;

  name_id_list.clear();

  if ( !expect(kToken_LPAR, cur_lval, cur_loc) ) {
    // '(' を期待していたシンタックスエラー
    return false;
  }

  for ( ; ; ) {
    if ( !expect(kToken_NAME, cur_lval, cur_loc) ) {
      // NAME を期待したシンタックスエラー
      return false;
    }
    name_id_list.push_back(cur_lval);

    Token tok = read_token(cur_lval, last_loc);
    if ( tok == kToken_RPAR ) {
      break;
    }
    if ( tok != kToken_COMMA ) {
      // ')' か ',' を期待していたシンタックスエラー
      ostringstream buf;
      buf << "Syntax error: ')' or ',' are expected.";
      MsgMgr::put_msg(__FILE__, __LINE__, cur_loc,
		      kMsgError,
		      "ER_SYNTAX03",
		      buf.str());
      return false;
    }
  }

  return true;
}

// @brief INPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 入力ピン名の ID 番号
// @return エラーが起きたら false を返す．
bool
Iscas89ParserImpl::read_input(const FileRegion& loc,
			      ymuint name_id)
{
  IdCell* cell = id2cell(name_id);
  if ( cell->is_defined() ) {
    ostringstream buf;
    buf << cell->str() << ": Defined more than once. Previous definition is "
	<< cell->def_loc();
    MsgMgr::put_msg(__FILE__, __LINE__, cell->loc(),
		    kMsgError,
		    "ER_MLTDEF01",
		    buf.str());
    return false;
  }
  cell->set_defined();
  cell->set_input();
  for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    Iscas89Handler* handler = *p;
    if ( !handler->read_input(loc, name_id) ) {
      return false;
    }
  }
  return true;
}

// @brief OUTPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 出力ピン名の ID 番号
// @return エラーが起きたら false を返す．
bool
Iscas89ParserImpl::read_output(const FileRegion& loc,
			       ymuint name_id)
{
  IdCell* cell = id2cell(name_id);
  if ( cell->is_input() ) {
    ostringstream buf;
    buf << cell->str() << ": Defined as both input and output. "
	<< "Previous definition is "
	<< cell->def_loc();
    MsgMgr::put_msg(__FILE__, __LINE__, cell->loc(),
		    kMsgWarning,
		    "WR_MLTDEF02",
		    buf.str());
  }
  cell->set_output();
  for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    Iscas89Handler* handler = *p;
    if ( !handler->read_output(loc, name_id) ) {
      return false;
    }
  }
  return true;
}

// @brief ゲート文を読み込む．
// @param[in] loc ファイル位置
// @param[in] oname_id 出力名の ID 番号
// @param[in] type ゲートタイプ
// @return エラーが起きたら false を返す．
// @note 入力名のリストは push_str() で積まれている．
bool
Iscas89ParserImpl::read_gate(const FileRegion& loc,
			     ymuint oname_id,
			     tIscas89GateType type,
			     const vector<ymuint>& iname_id_list)
{
  IdCell* cell = id2cell(oname_id);
  if ( cell->is_defined() ) {
    // 二重定義
    ostringstream buf;
    buf << cell->str() << ": Defined more than once. "
	<< "Previsous Definition is " << cell->def_loc();
    MsgMgr::put_msg(__FILE__, __LINE__, cell->loc(),
		    kMsgError,
		    "ER_MLTDEF01",
		    buf.str());
    return false;
  }
  cell->set_defined();
  bool stat = true;
  for (list<Iscas89Handler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    Iscas89Handler* handler = *p;
    if ( !handler->read_gate(loc, type, oname_id, iname_id_list) ) {
      stat = false;
      break;
    }
  }
  return stat;
}

BEGIN_NONAMESPACE

const char*
token_str(Token token)
{
  switch (token) {
  case kToken_LPAR:   return "(";
  case kToken_RPAR:   return ")";
  case kToken_EQ:     return "=";
  case kToken_COMMA:  return ",";
  case kToken_INPUT:  return "INPUT";
  case kToken_OUTPUT: return "OUTPUT";
  case kToken_BUFF:   return "BUFF";
  case kToken_NOT:    return "NOT";
  case kToken_AND:    return "AND";
  case kToken_NAND:   return "NAND";
  case kToken_OR:     return "OR";
  case kToken_NOR:    return "NOR";
  case kToken_XOR:    return "XOR";
  case kToken_XNOR:   return "XNOR";
  case kToken_DFF:    return "DFF";
  case kToken_NAME:   return "__name__";
  case kToken_EOF:    return "__eof__";
  case kToken_ERROR:  return "__error__";
  }
  ASSERT_NOT_REACHED;
}

END_NONAMESPACE

// @brief 次のトークンが期待されている型か調べる．
// @param[in] exp_token トークンの期待値
// @param[out] lval トークンの値を格納する変数
// @param[out] loc トークンのファイル位置を格納する変数．
// @retval true トークンの型が一致した．
// @retval false トークンの方が一致しなかった．
//
// トークンの方が一致しなかった場合にはエラーメッセージをセットする．
bool
Iscas89ParserImpl::expect(Token exp_token,
			  ymuint& lval,
			  FileRegion& loc)
{
  if ( read_token(lval, loc) != exp_token ) {
    // トークンが期待値と異なっていた
    ostringstream buf;
    buf << "Syntax error: '" << token_str(exp_token) << "' is expected.";
    MsgMgr::put_msg(__FILE__, __LINE__, loc,
		    kMsgError,
		    "ER_SYNTAX01",
		    buf.str());
    return false;
  }

  return true;
}

// @brief yylex() 用の処理を行う．
// @param[out] lval トークンの値を格納する変数
// @param[out] lloc トークンの位置を格納する変数
// @return トークンの型を返す．
Token
Iscas89ParserImpl::read_token(ymuint& lval,
			      FileRegion& lloc)
{
  Token token = mScanner->read_token(lloc);
  if ( token == kToken_NAME ) {
    lval = reg_str(mScanner->cur_string(), lloc);
  }
  return token;
}

// @brief 文字列用の領域を確保する．
// @param[in] src_str ソース文字列
// @param[in] loc 文字列の位置情報
// @return 文字列の ID 番号
ymuint
Iscas89ParserImpl::reg_str(const char* src_str,
			   const FileRegion& loc)
{
  IdCell* cell = mIdHash.find(src_str, true);
  cell->set_loc(loc);
  return cell->id();
}

END_NAMESPACE_YM_ISCAS89
