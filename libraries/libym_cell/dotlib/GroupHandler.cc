
/// @file libym_cell/dotlib/GroupHandler.cc
/// @brief GroupHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"
#include "DotlibParser.h"
#include "PtMgr.h"
#include "PtNode.h"
#include "PtValue.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス GroupHandler
//////////////////////////////////////////////////////////////////////

// @brief 親がない場合のコンストラクタ
// @param[in] parser パーサー
// @param[in] ptmgr パース木を管理するオブジェクト
GroupHandler::GroupHandler(DotlibParser& parser,
			   PtMgr& ptmgr) :
  DotlibHandler(parser, ptmgr, NULL)
{
}

// @brief 親を持つハンドラ用のコンストラクタ
// @param[in] parent 親のハンドラ
GroupHandler::GroupHandler(GroupHandler* parent) :
  DotlibHandler(parent->parser(), parent->ptmgr(), parent)
{
}

// @brief デストラクタ
GroupHandler::~GroupHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @return エラーが起きたら false を返す．
bool
GroupHandler::read_attr(const ShString& attr_name,
			const FileRegion& attr_loc)
{
  PtNode* value = parse_complex();
  if ( value == NULL ) {
    return false;
  }

  if ( debug() ) {
    cout << attr_name << value << endl;
  }

  if ( !begin_group(attr_name, attr_loc, value) ) {
    return false;
  }

  if ( !expect(LCB) ) {
    return false;
  }

  for ( ; ; ) {
    tTokenType type = parser().read_token();
    if ( type == NL ) {
      continue;
    }
    if ( type == RCB ) {
      break;
    }
    FileRegion loc = parser().cur_loc();
    if ( type != SYMBOL ) {
      put_msg(__FILE__, __LINE__, loc,
	      kMsgError,
	      "DOTLIB_PARSER",
	      "string value is expected.");
      return false;
    }
    ShString name(parser().cur_string());
    DotlibHandler* handler = find_handler(name);
    if ( handler == NULL ) {
      ostringstream buf;
      buf << name << ": unknown keyword.";
      put_msg(__FILE__, __LINE__, loc,
	      kMsgError,
	      "DOTLIB_PARSER",
	      buf.str());
      return false;
    }
    if ( !handler->read_attr(name, loc) ) {
      return false;
    }
  }

  if ( !expect(NL) ) {
    return false;
  }

  if ( debug() ) {
    cout << "}" << endl;
  }

  return true;
}

// @brief attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @return 設定が失敗したら false を返す．
// @note デフォルトの実装はエラーとなる．
bool
GroupHandler::add_attr(const ShString& attr_name,
		       PtNode* value)
{
  PtAttr* attr = ptmgr().new_attr(attr_name, value);
  mNode->add_attr(attr);
  return true;
}

#if 0
// @brief セルを追加する．
// @param[in] cell セル
// @note library の時のみ有効
bool
GroupHandler::add_cell(PtCell* cell)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief leakage_power を追加する．
bool
GroupHandler::add_leakage_power(PtLeakagePower* lp)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ピンを追加する．
// @param[in] pin ピン
// @note cell/bus/bundle の時のみ有効
bool
GroupHandler::add_pin(PtPin* pin)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief バスを追加する．
// @param[in] bus バス
// @note cell の時のみ有効
bool
GroupHandler::add_bus(PtBus* bus)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief バンドルを追加する．
// @param[in] bundle バンドル
// @note cell の時のみ有効
bool
GroupHandler::add_bundle(PtBundle* bundle)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief タイミングを追加する．
// @param[in] timing タイミング条件
// @note pin の時のみ有効
bool
GroupHandler::add_timing(PtTiming* timing)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief テーブルを追加する．
// @param[in] attr_name 属性名
// @param[in] table テーブル
// @note
bool
GroupHandler::add_table(const ShString& attr_name,
			PtTable* table)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief domain グループをセットする．
// @param[in] domain ドメイン
bool
GroupHandler::add_domain(PtDomain* domain)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}
#endif

// @brief ハンドラの登録を行う．
// @param[in] attr_name 属性名
// @param[in] handler 対応付けるハンドラ
// @note エラーが起きたら false を返す．
bool
GroupHandler::reg_handler(const char* attr_name,
			  DotlibHandler* handler)
{
  return reg_handler(ShString(attr_name), handler);
}

// @brief ハンドラの登録を行う．
// @param[in] attr_name 属性名
// @note エラーが起きたら false を返す．
bool
GroupHandler::reg_handler(const ShString& attr_name,
			  DotlibHandler* handler)
{
  mHandlerMap.insert(make_pair(attr_name, handler));
  return true;
}

// @brief ハンドラを取り出す．
// @param[in] attr_name 属性名
// @note なければ NULL を返す．
DotlibHandler*
GroupHandler::find_handler(const ShString& attr_name)
{
  hash_map<ShString, DotlibHandler*>::const_iterator p
    = mHandlerMap.find(attr_name);
  if ( p == mHandlerMap.end() ) {
    return NULL;
  }
  else {
    return p->second;
  }
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
bool
GroupHandler::begin_group(const ShString& attr_name,
			  const FileRegion& attr_loc,
			  PtNode* value_list)
{
  if ( !check_group_value(attr_name, attr_loc, value_list) ) {
    return false;
  }

  mNode = ptmgr().new_group(value_list, attr_loc);
  if ( attr_name != "library" ) {
    parent()->add_attr(attr_name, mNode);
  }
  return true;
}

// @brief group statement の最後に呼ばれる関数
bool
GroupHandler::end_group()
{
  mNode = NULL;
  return true;
}

// @brief 対応するノードを得る．
PtNode*
GroupHandler::pt_node()
{
  return mNode;
}

// @brief group statement の引数のチェックを行う仮想関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値を表すトークンのリスト
// @note begin_group() の中で呼ばれる．
// @note デフォルトの実装はなにもしないで true を返す．
bool
GroupHandler::check_group_value(const ShString& attr_name,
				const FileRegion& attr_loc,
				PtNode* value)
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス Str1GroupHandler
//////////////////////////////////////////////////////////////////////

// @brief 親を持たないハンドラ用のコンストラクタ
// @param[in] parser パーサー
// @param[in] ptmgr パース木を管理するオブジェクト
Str1GroupHandler::Str1GroupHandler(DotlibParser& parser,
				   PtMgr& ptmgr) :
  GroupHandler(parser, ptmgr)
{
}

// @brief 親を持つハンドラ用のコンストラクタ
// @param[in] parent 親のハンドラ
Str1GroupHandler::Str1GroupHandler(GroupHandler* parent) :
  GroupHandler(parent)
{
}

// @brief デストラクタ
Str1GroupHandler::~Str1GroupHandler()
{
}

// @brief group statement の引数のチェックを行う仮想関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
// @note begin_group() の中で呼ばれる．
// @note デフォルトの実装はなにもしないで true を返す．
bool
Str1GroupHandler::check_group_value(const ShString& attr_name,
				    const FileRegion& attr_loc,
				    PtNode* value_list)
{
  ymuint n = value_list->list_size();
  if ( n == 0 ) {
    ostringstream buf;
    buf << attr_name << " statement requires a string parameter.";
    put_msg(__FILE__, __LINE__, attr_loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  const PtNode* top = value_list->top();
  if ( n > 1 ) {
    const PtNode* second = top->next();
    FileRegion loc = second->loc();
    ostringstream buf;
    buf << attr_name << " statement has only one string parameter.";
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  if ( top->type() != PtNode::kString ) {
    put_msg(__FILE__, __LINE__, top->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "string value is expected.");
    return false;
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EmptyGroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
EmptyGroupHandler::EmptyGroupHandler(GroupHandler* parent) :
  GroupHandler(parent)
{
}

// @brief デストラクタ
EmptyGroupHandler::~EmptyGroupHandler()
{
}

// @brief group statement の引数のチェックを行う仮想関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
// @note begin_group() の中で呼ばれる．
// @note デフォルトの実装はなにもしないで true を返す．
bool
EmptyGroupHandler::check_group_value(const ShString& attr_name,
				     const FileRegion& attr_loc,
				     PtNode* value_list)
{
  ymuint n = value_list->list_size();
  if ( n > 0 ) {
    const PtNode* top = value_list->top();
    FileRegion loc = top->loc();
    ostringstream buf;
    buf << attr_name << " statement does not have parameters.";
    put_msg(__FILE__, __LINE__, loc,
	    kMsgError,
	    "DOTLIB_PARSER",
	    buf.str());
    return false;
  }

  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
