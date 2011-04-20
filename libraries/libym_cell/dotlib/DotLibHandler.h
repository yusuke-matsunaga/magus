#ifndef LIBYM_CELL_DOTLIB_DOTLIBHANDLER_H
#define LIBYM_CELL_DOTLIB_DOTLIBHANDLER_H

/// @file libym_cell/dotlib/DotLibHandler.h
/// @brief DotlibHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotLibHandler DotLibHandler.h "DotLibHandler.h"
/// @brief liberty ファイルの各構文要素を読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class DotLibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  DotLibHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~DotLibHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_name 属性名
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(const string& attr_name) = 0;

  /// @brief ハンドラの登録を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] handler 対応付けるハンドラ
  /// @note エラーが起きたら false を返す．
  virtual
  bool
  reg_handler(const string& attr_name,
	      DotLibHandler* handler) = 0;

  /// @brief ハンドラを取り出す．
  /// @param[in] attr_name 属性名
  /// @note なければ NULL を返す．
  virtual
  DotLibHandler*
  find_handler(const string& name) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
  /// @param[in] type 要求するトークンの型
  bool
  expect(tTokenType type);

  /// @brief 行末まで読み込む．
  bool
  expect_nl();

  /// @brief パーサーを得る．
  DotLibParser&
  parser();

  /// @brief メッセージ出力管理オブジェクトを得る．
  MsgMgr&
  msg_mgr();

  /// @brief 字句解析器を得る．
  DotLibLex&
  lex();

  /// @brief デバッグモードの時に true を返す．
  bool
  debug();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーサー
  DotLibParser& mParser;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_DOTLIBHANDLER_H
