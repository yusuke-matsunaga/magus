#ifndef LIBYM_DOTLIB_DOTLIBHANDLER_H
#define LIBYM_DOTLIB_DOTLIBHANDLER_H

/// @file libym_dotlib/DotlibHandler.h
/// @brief DotlibHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/dotlib_nsdef.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_DOTLIB

class DotlibParserImpl;
class GroupHandler;
class PtNodeImpl;

//////////////////////////////////////////////////////////////////////
/// @class DotlibHandler DotlibHandler.h "DotlibHandler.h"
/// @brief liberty ファイルの各構文要素を読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class DotlibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  /// @param[in] ptmgr パース木を管理するオブジェクト
  /// @param[in] parent 親のハンドラ
  DotlibHandler(DotlibParserImpl& parser,
		PtMgr& ptmgr,
		GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~DotlibHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(const ShString& attr_name,
	    const FileRegion& attr_loc) = 0;

  /// @brief 対応するノードを得る．
  /// @note デフォルトの実装は NULL を返す．
  virtual
  PtNode*
  pt_node();


public:
  //////////////////////////////////////////////////////////////////////
  // 他のクラスが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パーサーを得る．
  DotlibParserImpl&
  parser();

  /// @brief PtMgr を得る．
  PtMgr&
  ptmgr();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group attribute 用のパースを行う．
  /// @return 読み込んだ値(リスト)を返す．
  /// @note エラーが起きたら NULL を返す．
  PtNodeImpl*
  parse_complex();

  /// @brief PtNode を生成する．
  /// @param[in] type 型
  /// @note 残りの情報は parser() からとってくる．
  PtNodeImpl*
  new_ptvalue(tTokenType type);

  /// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
  /// @param[in] req_type 要求するトークンの型
  bool
  expect(tTokenType req_type);

  /// @brief 行末まで読み込む．
  bool
  expect_nl();

  /// @brief 親のハンドラを得る．
  GroupHandler*
  parent();

  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const char* msg);

  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const string& msg);

  /// @brief デバッグモードの時に true を返す．
  bool
  debug();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーサー
  DotlibParserImpl& mParser;

  // パース木を管理するオブジェクト
  PtMgr& mPtMgr;

  // 親のハンドラ
  GroupHandler* mParent;

};

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_DOTLIBHANDLER_H
