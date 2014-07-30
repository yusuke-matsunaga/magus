
/// @file SmtLibParser.cc
/// @brief SmtLibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibParser.h"
#include "SmtLibScanner.h"
#include "SmtLibNodeImpl.h"
#include "YmUtils/FileIDO.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// SmtLibParser
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SmtLibParser::SmtLibParser() :
  mAlloc(4096)
{
  mScanner = NULL;
}

// デストラクタ
SmtLibParser::~SmtLibParser()
{
  clear();
  delete mScanner;
}

// @brief 初期化する．
// @param[in] ido 入力データ
void
SmtLibParser::init(IDO& ido)
{
  clear();
  delete mScanner;
  mScanner = new SmtLibScanner(ido);
}

// @brief S式を一つ読み込む．
// @param[out] root 読んだS式の根のノード
// @return 結果を返す．
SmtLibParser::tResult
SmtLibParser::read(SmtLibNode*& root)
{
  clear();

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
    tTokenType type = mScanner->read_token(loc);
    switch ( type ) {
    case kNumToken:
      node = new_num(loc, atoi(mScanner->cur_string()));
      break;

    case kDecToken:
      node = new_dec(loc, ShString(mScanner->cur_string()));
      break;

    case kHexToken:
      node = new_hex(loc, ShString(mScanner->cur_string()));
      break;

    case kBinToken:
      node = new_bin(loc, ShString(mScanner->cur_string()));
      break;

    case kStringToken:
      node = new_string(loc, ShString(mScanner->cur_string()));
      break;

    case kSymbolToken:
      node = new_symbol(loc, ShString(mScanner->cur_string()));
      break;

    case kKeywordToken:
      node = new_keyword(loc, ShString(mScanner->cur_string()));
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

    case kErrorToken:
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
  return new (p) SmtLibNumNode(loc, new_id(), val);
}

// @brief DEC タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_dec(const FileRegion& loc,
		      const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibDecNode));
  return new (p) SmtLibDecNode(loc, new_id(), val);
}

// @brief HEX タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_hex(const FileRegion& loc,
		      const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibHexNode));
  return new (p) SmtLibHexNode(loc, new_id(), val);
}

// @brief BIN タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_bin(const FileRegion& loc,
		      const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibBinNode));
  return new (p) SmtLibBinNode(loc, new_id(), val);
}

// @brief STRING タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_string(const FileRegion& loc,
			 const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibStrNode));
  return new (p) SmtLibStrNode(loc, new_id(), val);
}

// @brief SYMBOL タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_symbol(const FileRegion& loc,
			 const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibSymbolNode));
  return new (p) SmtLibSymbolNode(loc, new_id(), val);
}

// @brief KEYWORD タイプのノードを生成する．
// @param[in] loc ファイル上の位置
// @param[in] val 値
SmtLibNode*
SmtLibParser::new_keyword(const FileRegion& loc,
			  const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibKeywordNode));
  return new (p) SmtLibKeywordNode(loc, new_id(), val);
}

// @brief LIST タイプのノードを生成する．
SmtLibNode*
SmtLibParser::new_list(const FileRegion& loc,
		       ymuint num,
		       const SmtLibNode* child)
{
  void* p = mAlloc.get_memory(sizeof(SmtLibListNode));
  return new (p) SmtLibListNode(loc, new_id(), num, child);
}

// @brief SmtLibNode の次のID番号を返す．
ymuint
SmtLibParser::new_id()
{
  ymuint val = mLastId;
  ++ mLastId;
  return val;
}

END_NAMESPACE_YM_SMTLIBV2
