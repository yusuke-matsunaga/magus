
/// @file libym_smtlibv2/parser/SmtLibParser.cc
/// @brief SmtLibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SmtLibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibParser.h"
#include "ym_utils/FileDescMgr.h"
#include "SmtLibNodeImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// SmtLibParser
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SmtLibParser::SmtLibParser() :
  mAlloc(4096),
  mLex(mMsgMgr)
{
}

// デストラクタ
SmtLibParser::~SmtLibParser()
{
  clear();
}

// @brief smtlib ファイルを読み込んでライブラリを生成する．
// @param[in] filename ファイル名
// @return 生成したライブラリを返す．
// @note 読み込みが失敗したら NULL を返す．
void
SmtLibParser::read(const string& filename)
{
  if ( !mLex.open_file(filename) ) {
    // エラー
    ostringstream buf;
    buf << filename << " : No such file.";
    mMsgMgr.put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "SMTLIB_PARSER", buf.str());
    return;
  }

  // 初期化
  clear();
  mError = false;

  for ( ; ; ) {
    SmtLibNode* root = NULL;
    tTokenType type = read_sexp(root);
    if ( type == kEofToken ) {
      break;
    }
    else if ( type == kErrorToken ) {
      mError = true;
    }
    else if ( type == kRpToken ) {
      mMsgMgr.put_msg(__FILE__, __LINE__,
		      mLex.cur_file_region(),
		      kMsgError,
		      "SMTLIB_PARSER",
		      "Unexpected ')'.");
      mError = true;
    }
    else if ( type == kNlToken ) {
      ;
    }
    else {
      dump(cout, root);
    }
  }

  if ( mError ) {
    // 異常終了
    return;
  }

  // 読み込みまではうまくいった．
}

// 今までに生成したすべてのオブジェクトを解放する．
void
SmtLibParser::clear()
{
  mAlloc.destroy();
}

tTokenType
SmtLibParser::read_sexp(SmtLibNode*& node)
{
  tTokenType type = mLex.read_token();
  if ( type == kEofToken ) {
    return kEofToken;
  }
  FileRegion loc = mLex.cur_file_region();

  node = NULL;
  switch ( type ) {
  case kNumToken:
    node = new_num(loc, ShString(mLex.cur_string()).id());
    break;

  case kDecToken:
    node = new_dec(loc, ShString(mLex.cur_string()).id());
    break;

  case kHexToken:
    node = new_hex(loc, ShString(mLex.cur_string()).id());
    break;

  case kBinToken:
    node = new_bin(loc, ShString(mLex.cur_string()).id());
    break;

  case kStringToken:
    node = new_string(loc, ShString(mLex.cur_string()).id());
    break;

  case kSymbolToken:
    node = new_symbol(loc, ShString(mLex.cur_string()).id());
    break;

  case kKeywordToken:
    node = new_keyword(loc, ShString(mLex.cur_string()).id());
    break;

  case kLpToken:
    {
      list<SmtLibNode*> child_list;
      for ( ; ; ) {
	SmtLibNode* node1;
	tTokenType type1 = read_sexp(node1);
	if ( type1 == kErrorToken ) {
	  // エラー
	  return kErrorToken;
	}
	else if ( type1 == kRpToken ) {
	  break;
	}
	child_list.push_back(node1);
      }
      node = new_list(loc, child_list);
    }
    type = kListToken;
    break;

  case kRpToken:
    break;

  case kNlToken:
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return type;
}

// @brief NUM タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_num(const FileRegion& loc,
		      StrId val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibNumNode));
  return new (p) SmtLibNumNode(loc, val);
}

// @brief DEC タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_dec(const FileRegion& loc,
		      StrId val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibDecNode));
  return new (p) SmtLibDecNode(loc, val);
}

// @brief HEX タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_hex(const FileRegion& loc,
		      StrId val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibHexNode));
  return new (p) SmtLibHexNode(loc, val);
}

// @brief BIN タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_bin(const FileRegion& loc,
		      StrId val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibBinNode));
  return new (p) SmtLibBinNode(loc, val);
}

// @brief STRING タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_string(const FileRegion& loc,
			 StrId val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibStringNode));
  return new (p) SmtLibStringNode(loc, val);
}

// @brief SYMBOL タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_symbol(const FileRegion& loc,
			 StrId val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibSymbolNode));
  return new (p) SmtLibSymbolNode(loc, val);
}

// @brief KEYWORD タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_keyword(const FileRegion& loc,
			  StrId val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibKeywordNode));
  return new (p) SmtLibKeywordNode(loc, val);
}

// @brief LIST タイプのノードを生成する．
SmtLibNode*
SmtLibParser::new_list(const FileRegion& loc,
		       const list<SmtLibNode*>& child_list)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibListNode));
  SmtLibListNode* node = new (p) SmtLibListNode(loc);

  ymuint n = child_list.size();
  void* q = mAlloc.get_memory(sizeof(SmtLibNode*) * n);
  node->mChildNum = n;
  node->mChildArray = new (q) SmtLibNode*[n];
  ymuint i = 0;
  for (list<SmtLibNode*>::const_iterator p = child_list.begin();
       p != child_list.end(); ++ p, ++ i) {
    node->mChildArray[i] = *p;
  }
  return node;
}

// @brief エラーメッセージを出力する．
// @note 副作用で mError が true にセットされる．
void
SmtLibParser::error(const FileRegion& loc,
		    const char* msg)
{
  string buff;
  const char* msg2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(msg, "parse error", 11) ) {
    buff ="syntax error";
    buff += (msg + 11);
    msg2 = buff.c_str();
  }
  else {
    msg2 = msg;
  }

  msg_mgr().put_msg(__FILE__, __LINE__, loc,
		    kMsgError, "SMTLIB_PARSER", msg2);
  mError = true;
}

// @brief S式の内容を出力する．
// @note デバッグ用
void
SmtLibParser::dump(ostream& s,
		   const SmtLibNode* node)
{
  s << "Loc:  " << node->loc() << endl;
  const char* type_str = NULL;
  switch ( node->type() ) {
  case kNumToken:     type_str = "Numeral"; break;
  case kDecToken:     type_str = "Decimal"; break;
  case kHexToken:     type_str = "Hexadecimal"; break;
  case kBinToken:     type_str = "Binary"; break;
  case kStringToken:  type_str = "String"; break;
  case kSymbolToken:  type_str = "Symbol"; break;
  case kKeywordToken: type_str = "Keyword"; break;
  case kListToken:    type_str = "List"; break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  s << "Type: " << type_str << endl;
  if ( node->type() == kListToken ) {
    ymuint n = node->child_num();
    for (ymuint i = 0; i < n; ++ i) {
      const SmtLibNode* node1 = node->child(i);
      s << "Child#" << i << endl;
      dump(s, node1);
    }
  }
  else {
    s << "Value: " << node->value() << endl;
  }
}

END_NAMESPACE_YM_SMTLIBV2
