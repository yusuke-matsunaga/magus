#ifndef YM_CELL_DOTLIBPARSER_H
#define YM_CELL_DOTLIBPARSER_H

/// @file libym_cell/DotLibParser.h
/// @brief DotLibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotLibParser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "ym_utils/MsgHandler.h"
#include "DotLibLex.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// DotLib 用のパーサークラス
//////////////////////////////////////////////////////////////////////
class DotLibParser
{
public:

  /// @brief コンストラクタ
  DotLibParser();

  /// @brief デストラクタ
  ~DotLibParser();


public:

  /// @brief ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] debug デバッグモード
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_file(const string& filename,
	    bool debug);


public:
  //////////////////////////////////////////////////////////////////////
  // DotLibHandler から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
  bool
  expect(tTokenType type);

  /// @brief 行末まで読み込む．
  bool
  expect_nl();

  /// @brief メッセージ出力管理オブジェクトを返す．
  MsgMgr&
  msg_mgr();

  /// @brief 字句解析器を返す．
  DotLibLex&
  lex();

  /// @brief デバッグモードの時 true を返す．
  bool
  debug();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージを管理するオブジェクト
  MsgMgr mMsgMgr;

  // 字句解析器
  DotLibLex mLex;

  // library グループを処理するハンドラ
  DotLibHandler* mLibraryHandler;

  // デバッグモード
  bool mDebug;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // YM_CELL_DOTLIBPARSER_H
