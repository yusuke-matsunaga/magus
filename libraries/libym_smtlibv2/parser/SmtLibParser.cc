
/// @file libym_smtlibv2/parser/SmtLibParser.cc
/// @brief SmtLibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SmtLibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibParser.h"
#include "SmtLibNodeImpl.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// SmtLibParser
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SmtLibParser::SmtLibParser() :
  mAlloc(4096)
{
}

// デストラクタ
SmtLibParser::~SmtLibParser()
{
  clear();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @retval true 成功した．
// @retval false 失敗した．
bool
SmtLibParser::open(const string& filename)
{
  if ( !mScanner.open_file(filename) ) {
    // エラー
    ostringstream buf;
    buf << filename << " : No such file.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "SMTLIB_PARSER", buf.str());
    return false;
  }

  // 初期化
  clear();

  return true;
}

// @brief S式を一つ読み込む．
// @param[out] error エラーが起きたら true を格納する．
// @return 読み込んだ S式を表すノードを返す．
SmtLibNode*
SmtLibParser::read(bool& error)
{
  for ( ; ; ) {
    SmtLibNode* root = NULL;
    FileRegion loc;
    tTokenType type = read_sexp(root, loc);
    if ( type == kEofToken ) {
      // ファイルの末尾まで読んだ
      return NULL;
    }
    else if ( type == kNlToken ) {
      // 改行コードは無視する．
      continue;
    }
    else if ( type == kErrorToken ) {
      // 字句解析レベルでエラーが起きている．
      error = true;
      return NULL;
    }
    else if ( type == kRpToken ) {
      // 対応する左括弧がない．
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      kMsgError,
		      "SMTLIB_PARSER",
		      "Unexpected ')'.");
      error = true;
      return NULL;
    }
    return root;
  }
}

// 今までに生成したすべてのオブジェクトを解放する．
void
SmtLibParser::clear()
{
  mAlloc.destroy();
}

// @brief S式の読み込みを行う．
// @param[out] node 読み込んだ S式を格納するノード
// @param[out] loc S式のファイル上の位置
// @return トークンの型を返す．
// @note node は NULL の場合もある．
tTokenType
SmtLibParser::read_sexp(SmtLibNode*& node,
			FileRegion& loc)
{
  tTokenType type = mScanner.read_token(loc);
  if ( type == kEofToken ) {
    return kEofToken;
  }

  node = NULL;
  switch ( type ) {
  case kNumToken:
    node = new_num(loc, atoi(mScanner.cur_string()));
    break;

  case kDecToken:
    node = new_dec(loc, ShString(mScanner.cur_string()));
    break;

  case kHexToken:
    node = new_hex(loc, ShString(mScanner.cur_string()));
    break;

  case kBinToken:
    node = new_bin(loc, ShString(mScanner.cur_string()));
    break;

  case kStringToken:
    node = new_string(loc, ShString(mScanner.cur_string()));
    break;

  case kSymbolToken:
    node = new_symbol(loc, ShString(mScanner.cur_string()));
    break;

  case kKeywordToken:
    node = new_keyword(loc, ShString(mScanner.cur_string()));
    break;

  case kLpToken:
    {
      SmtLibNode* top = NULL;
      SmtLibNode** last_ptr = &top;
      ymuint num = 0;
      FileRegion last_loc;
      for ( ; ; ) {
	SmtLibNode* node1;
	FileRegion loc1;
	tTokenType type1 = read_sexp(node1, loc1);
	if ( type1 == kErrorToken ) {
	  // エラー
	  return kErrorToken;
	}
	else if ( type1 == kRpToken ) {
	  last_loc = loc1;
	  break;
	}
	*last_ptr = node1;
	last_ptr = &node1->mSibling;
	++ num;
	last_loc = loc1;
      }
      node = new_list(FileRegion(loc, last_loc), num, top);
    }
    type = kListToken;
    break;

  case kNlToken:
    break;

  case kRpToken:
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
		      ymint32 val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibNumNode));
  return new (p) SmtLibNumNode(loc, val);
}

// @brief DEC タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_dec(const FileRegion& loc,
		      const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibDecNode));
  return new (p) SmtLibDecNode(loc, val);
}

// @brief HEX タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_hex(const FileRegion& loc,
		      const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibHexNode));
  return new (p) SmtLibHexNode(loc, val);
}

// @brief BIN タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_bin(const FileRegion& loc,
		      const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibBinNode));
  return new (p) SmtLibBinNode(loc, val);
}

// @brief STRING タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_string(const FileRegion& loc,
			 const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibStrNode));
  return new (p) SmtLibStrNode(loc, val);
}

// @brief SYMBOL タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_symbol(const FileRegion& loc,
			 const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibSymbolNode));
  return new (p) SmtLibSymbolNode(loc, val);
}

// @brief KEYWORD タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_keyword(const FileRegion& loc,
			  const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibKeywordNode));
  return new (p) SmtLibKeywordNode(loc, val);
}

// @brief LIST タイプのノードを生成する．
SmtLibNode*
SmtLibParser::new_list(const FileRegion& loc,
		       ymuint num,
		       const SmtLibNode* child)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibListNode));
  return new (p) SmtLibListNode(loc, num, child);
}


BEGIN_NONAMESPACE

// 字下げ用の空白を出力する．
void
print_space(ostream& s,
	    ymuint ident_level)
{
  for (ymuint i = 0; i < ident_level; ++ i) {
    s << "  ";
  }
}

END_NONAMESPACE

// @relates SmtLibNode
// @brief SmbLitNode の内容を出力する(デバッグ用)．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
// @param[in] ident_level 字下げのレベル
// @param[in] print_loc ファイル位置の情報を出力するとき true にするフラグ
void
display(ostream& s,
	const SmtLibNode* node,
	ymuint ident_level,
	bool print_loc)
{
  if ( print_loc ) {
    print_space(s, ident_level);
    s << "Loc:  " << node->loc() << endl;
  }
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
  print_space(s, ident_level);
  s << "Type: " << type_str << endl;
  if ( node->type() == kListToken ) {
    ymuint ident_level1 = ident_level + 1;
    ymuint n = node->child_num();
    const SmtLibNode* child = node->child();
    for (ymuint i = 0; i < n; ++ i, child = child->sibling()) {
      print_space(s, ident_level1);
      s << "Child#" << i << endl;
      display(s, child, ident_level1, print_loc);
    }
  }
  else if ( node->type() == kNumToken ) {
    print_space(s, ident_level);
    s << "Value: " << node->int_value() << endl;
  }
  else {
    print_space(s, ident_level);
    s << "Value: " << node->str_value() << endl;
  }
}

END_NAMESPACE_YM_SMTLIBV2
