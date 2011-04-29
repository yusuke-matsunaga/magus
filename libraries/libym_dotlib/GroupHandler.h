#ifndef LIBYM_DOTLIB_GROUPHANDLER_H
#define LIBYM_DOTLIB_GROUPHANDLER_H

/// @file libym_dotlib/GroupHandler.h
/// @brief GroupHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

class PtNodeImpl;

//////////////////////////////////////////////////////////////////////
/// @class GroupHandler GroupHandler.h "GroupHandler.h"
/// @brief group statement 用のハンドラ
//////////////////////////////////////////////////////////////////////
class GroupHandler :
  public DotlibHandler
{
public:

  /// @brief 親を持たないハンドラ用のコンストラクタ
  /// @param[in] parser パーサー
  /// @param[in] ptmgr パース木を管理するオブジェクト
  GroupHandler(DotlibParserImpl& parser,
	       PtMgr& ptmgr);

  /// @brief 親を持つハンドラ用のコンストラクタ
  /// @param[in] parent 親のハンドラ
  GroupHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~GroupHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_token 属性名を表すトークン
  /// @param[in] attr_loc ファイル上の位置
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(const ShString& attr_name,
	    const FileRegion& attr_loc);


public:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドラの登録を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] handler 対応付けるハンドラ
  /// @note エラーが起きたら false を返す．
  bool
  reg_handler(const char* attr_name,
	      DotlibHandler* handler);

  /// @brief ハンドラの登録を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] handler 対応付けるハンドラ
  /// @note エラーが起きたら false を返す．
  bool
  reg_handler(const ShString& attr_name,
	      DotlibHandler* handler);

  /// @brief ハンドラを取り出す．
  /// @param[in] attr_name 属性名
  /// @note なければ NULL を返す．
  DotlibHandler*
  find_handler(const ShString& attr_name);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  /// @return 設定が失敗したら false を返す．
  /// @note デフォルトの実装はエラーとなる．
  virtual
  bool
  add_attr(const ShString& attr_name,
	   PtNodeImpl* value);

  /// @brief 対応するノードを得る．
  virtual
  PtNode*
  pt_node();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の最初に呼ばれる関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値を表すトークンのリスト
  virtual
  bool
  begin_group(const ShString& attr_name,
	      const FileRegion& attr_loc,
	      PtNodeImpl* value);

  /// @brief group statement の最後に呼ばれる関数
  virtual
  bool
  end_group();

  /// @brief group statement の引数のチェックを行う仮想関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値を表すトークンのリスト
  /// @note begin_group() の中で呼ばれる．
  /// @note デフォルトの実装はなにもしないで true を返す．
  virtual
  bool
  check_group_value(const ShString& attr_name,
		    const FileRegion& attr_loc,
		    PtNodeImpl* value);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハンドラの連想配列
  hash_map<ShString, DotlibHandler*> mHandlerMap;

  // 対応する PtNode
  PtNodeImpl* mNode;

};


//////////////////////////////////////////////////////////////////////
/// @class EmptyGroupHandler GroupHandler.h "GroupHadler.h"
/// @brief 値をとらないグループ用のハンドラ
//////////////////////////////////////////////////////////////////////
class EmptyGroupHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  EmptyGroupHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~EmptyGroupHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の引数のチェックを行う仮想関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値を表すトークンのリスト
  /// @note begin_group() の中で呼ばれる．
  /// @note デフォルトの実装はなにもしないで true を返す．
  virtual
  bool
  check_group_value(const ShString& attr_name,
		    const FileRegion& attr_loc,
		    PtNodeImpl* value);

};


//////////////////////////////////////////////////////////////////////
/// @class Str1GroupHandler GroupHandler.h "GroupHadler.h"
/// @brief 1つの文字列型をとるグループ用のハンドラ
//////////////////////////////////////////////////////////////////////
class Str1GroupHandler :
  public GroupHandler
{
public:

  /// @brief 親を持たないハンドラ用のコンストラクタ
  /// @param[in] parser パーサー
  /// @param[in] ptmgr パース木を管理するオブジェクト
  Str1GroupHandler(DotlibParserImpl& parser,
		   PtMgr& ptmgr);

  /// @brief 親を持つハンドラ用のコンストラクタ
  /// @param[in] parent 親のハンドラ
  Str1GroupHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~Str1GroupHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の引数のチェックを行う仮想関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値を表すトークンのリスト
  /// @note begin_group() の中で呼ばれる．
  /// @note デフォルトの実装はなにもしないで true を返す．
  virtual
  bool
  check_group_value(const ShString& attr_name,
		    const FileRegion& attr_loc,
		    PtNodeImpl* value);

};


//////////////////////////////////////////////////////////////////////
/// @class Str2GroupHandler GroupHandler.h "GroupHadler.h"
/// @brief 2つの文字列型をとるグループ用のハンドラ
//////////////////////////////////////////////////////////////////////
class Str2GroupHandler :
  public GroupHandler
{
public:

  /// @brief 親を持つハンドラ用のコンストラクタ
  /// @param[in] parent 親のハンドラ
  Str2GroupHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~Str2GroupHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の引数のチェックを行う仮想関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値を表すトークンのリスト
  /// @note begin_group() の中で呼ばれる．
  /// @note デフォルトの実装はなにもしないで true を返す．
  virtual
  bool
  check_group_value(const ShString& attr_name,
		    const FileRegion& attr_loc,
		    PtNodeImpl* value);

};


//////////////////////////////////////////////////////////////////////
/// @class Str2IntGroupHandler GroupHandler.h "GroupHadler.h"
/// @brief 2つの文字列型と1つの整数型をとるグループ用のハンドラ
//////////////////////////////////////////////////////////////////////
class Str2IntGroupHandler :
  public GroupHandler
{
public:

  /// @brief 親を持つハンドラ用のコンストラクタ
  /// @param[in] parent 親のハンドラ
  Str2IntGroupHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~Str2IntGroupHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の引数のチェックを行う仮想関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値を表すトークンのリスト
  /// @note begin_group() の中で呼ばれる．
  /// @note デフォルトの実装はなにもしないで true を返す．
  virtual
  bool
  check_group_value(const ShString& attr_name,
		    const FileRegion& attr_loc,
		    PtNodeImpl* value);

};

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_GROUPHANDLER_H
