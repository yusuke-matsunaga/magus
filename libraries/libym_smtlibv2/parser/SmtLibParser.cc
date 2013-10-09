
/// @file libym_smtlibv2/parser/SmtLibParser.cc
/// @brief SmtLibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibParser.h"
#include "SmtLibNodeImpl.h"
#include "ym_utils/FileIDO.h"
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

// @brief 初期化する．
// @param[in] ido 入力データ
void
SmtLibParser::init(IDO* ido)
{
  clear();
  mScanner.attach(ido);
}

// @brief S式を一つ読み込む．
// @param[out] root 読んだS式の根のノード
// @return 結果を返す．
SmtLibParser::tResult
SmtLibParser::read(SmtLibNode*& root)
{
  // ループを回るのは改行コードの場合のみ
  FileRegion loc;
  tTokenType type = read_sexp(root, loc);
  switch ( type ) {
  case kEofToken:
    // ファイルの末尾まで読んだ
    return kEOF;

  case kUnexpEofToken:
    // カッコが閉じられる前に末尾になった．
    return kOpen;

  case kErrorToken:
    // 字句解析レベルでエラーが起きている．
    return kError;

  case kRpToken:
    // 対応する左括弧がない．
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc,
		    kMsgError,
		    "SMTLIB_PARSER",
		    "Unexpected ')'.");
    return kError;

  default:
    // それ以外は正常終了
    break;
  }

  return kOK;
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
  node = NULL;
  for ( ; ; ) {
    // ループを回るのは改行の場合だけ．
    tTokenType type = mScanner.read_token(loc);
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
	  else if ( type1 == kEofToken ) {
	    // 括弧が閉じられる前に末尾になった．
	    return kUnexpEofToken;
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

    case kEofToken:
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
    }
    if ( type != kNlToken ) {
      return type;
    }
  }
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

// @relates SmtLibNode
// @brief SmtLibNode の内容をもとの形で出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
print(ostream& s,
      const SmtLibNode* node)
{
  for ( ; node != NULL; node = node->sibling()) {
    s << " ";
    switch ( node->type() ) {
    case kNumToken:
      s << node->int_value();
      break;

    case kDecToken:
    case kHexToken:
    case kBinToken:
    case kStringToken:
    case kSymbolToken:
    case kKeywordToken:
      s << node->str_value();
      break;

    case kListToken:
      s << "(";
      print(s, node->child());
      s << " )";
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
    }
  }
}

END_NAMESPACE_YM_SMTLIBV2
