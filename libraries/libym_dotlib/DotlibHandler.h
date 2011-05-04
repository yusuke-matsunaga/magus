#ifndef LIBYM_DOTLIB_DOTLIBHANDLER_H
#define LIBYM_DOTLIB_DOTLIBHANDLER_H

/// @file libym_dotlib/DotlibHandler.h
/// @brief DotlibHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_int.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibHandler DotlibHandler.h "DotlibHandler.h"
/// @brief liberty ファイルの各構文要素を読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class DotlibHandler
{
public:

  /// @brief 親のハンドラを持たない場合のコンストラクタ
  /// @param[in] parser パーサー
  DotlibHandler(DotlibParserImpl& parser);

  /// @brief 親のハンドラを持つ場合のコンストラクタ
  /// @param[in] parent 親のハンドラ
  DotlibHandler(GroupHandler* parent);

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
  const DotlibNode*
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
  pt_mgr();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group attribute 用のパースを行う．
  /// @return 読み込んだ値(リスト)を返す．
  /// @note エラーが起きたら NULL を返す．
  DotlibNodeImpl*
  parse_complex();

  /// @brief DotlibNode (の派生クラス)を生成する．
  /// @param[in] type 型
  /// @param[in] loc ファイル上の位置情報
  /// @note 残りの情報は parser() からとってくる．
  DotlibNodeImpl*
  new_ptvalue(tTokenType type,
	      const FileRegion& loc);

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

  /// @brief デバッグモードの時に true を返す．
  bool
  debug();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーサー
  DotlibParserImpl& mParser;

  // 親のハンドラ
  GroupHandler* mParent;

};

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_DOTLIBHANDLER_H
