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
#include "ym_utils/FileDescMgr.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/StrBuff.h"


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
  /// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_file(const string& filename,
	    bool debug,
	    bool allow_no_semi = true);


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

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @return ファイルのオープンに失敗したら false を返す．
  bool
  open_file(const string& filename);

  /// @brief ファイルをクローズする．
  void
  close_file();

  /// @brief トークンを一つとってくる．
  /// @param[in] symbol_mode 数字も文字とみなすモード
  tTokenType
  read_token(bool symbol_mode = false);

  /// @brief 直前の read_token() に対応する文字列を返す．
  const char*
  cur_string() const;

  // 現在のファイル位置
  FileRegion
  cur_loc();

  /// @brief 現在の行番号を返す．
  int
  cur_line() const;

  /// @brief 現在のトークンの最初のコラム位置
  int
  first_column() const;

  /// @brief 現在のコラム位置
  int
  cur_column() const;

  /// @brief メッセージ出力管理オブジェクトを返す．
  MsgMgr&
  msg_mgr();

  /// @brief デバッグモードの時 true を返す．
  bool
  debug();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  void
  init();

  /// @brief c が文字の時に true を返す．
  /// @note mSymbolMode が true なら数字も文字とみなす．
  bool
  is_symbol(int c);

  /// @brief 一文字読み出す．
  int
  get();

  /// @brief 直前の get() を無かったことにする．
  void
  unget();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージを管理するオブジェクト
  MsgMgr mMsgMgr;

  // library グループを処理するハンドラ
  DotLibHandler* mLibraryHandler;

  // デバッグモード
  bool mDebug;

  // 行末のセミコロンなしを許すかどうかのフラグ
  bool mAllowNoSemi;

  // シンボルモード
  bool mSymbolMode;

  // ファイルを管理するオブジェクト
  FileDescMgr mFileDescMgr;

  // 現在オープン中のファイル
  const FileDesc* mCurFileDesc;

  // 入力ストリーム
  ifstream mInput;

  // 直前に読んだ文字
  int mLastChar;

  // unget() 用の文字バッファ
  int mUngetChar;

  // 直前の文字が \r の時に true となるフラグ
  bool mCR;

  // read_token の結果の文字列を格納する
  StrBuff mCurString;

  // 現在の行番号
  int mCurLine;

  // 現在のトークンの開始位置
  int mFirstColumn;

  // 現在のコラム位置
  int mCurColumn;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 直前の read_token() に対応する文字列を返す．
inline
const char*
DotLibParser::cur_string() const
{
  return mCurString.c_str();
}

// 現在のファイル位置
inline
FileRegion
DotLibParser::cur_loc()
{
  return FileRegion(mCurFileDesc,
		    mCurLine, mFirstColumn,
		    mCurLine, mCurColumn);
}

// 現在の行番号を返す．
inline
int
DotLibParser::cur_line() const
{
  return mCurLine;
}

// 現在のトークンの開始位置
inline
int
DotLibParser::first_column() const
{
  return mFirstColumn;
}

// 現在のコラム位置を返す．
inline
int
DotLibParser::cur_column() const
{
  return mCurColumn;
}

END_NAMESPACE_YM_CELL_DOTLIB

#endif // YM_CELL_DOTLIBPARSER_H
