#ifndef YM_CELL_DOTLIBPARSER_H
#define YM_CELL_DOTLIBPARSER_H

/// @file libym_cell/DotlibParser.h
/// @brief DotlibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotlibParser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/FileDescMgr.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

class LibraryHandler;

//////////////////////////////////////////////////////////////////////
/// Dotlib 用のパーサークラス
//////////////////////////////////////////////////////////////////////
class DotlibParser
{
public:

  /// @brief コンストラクタ
  DotlibParser();

  /// @brief デストラクタ
  ~DotlibParser();


public:

  /// @brief ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] debug デバッグモード
  /// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
  /// @return パース木の根のノードを返す．
  /// @note エラーが起きたら NULL を返す．
  PtNode*
  read_file(const string& filename,
	    bool debug,
	    bool allow_no_semi = true);


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の種類のトークンでなければエラーメッセージを出力する．
  bool
  expect(tTokenType type);

  /// @brief 行末まで読み込む．
  bool
  expect_nl();

  /// @brief トークンを一つ読み込む．
  /// @param[in] symbol_mode 数字も文字とみなすモード
  /// @return トークンの型を返す．
  tTokenType
  read_token(bool symbol_mode = false);

  /// @brief 直前の read_token() に対応する文字列を返す．
  const char*
  cur_string() const;

  /// @brief 直前の read_token() に対応する整数値を返す．
  /// @note 型が INT_NUM でなかったときの値は不定
  int
  cur_int() const;

  /// @brief 直前の read_token() に対応する実数値を返す．
  /// @note 型が FLOAT_NUM/INT_NUM でなかったときの値は不定
  double
  cur_float() const;

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

  /// @brief デバッグモードの時 true を返す．
  bool
  debug();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @return ファイルのオープンに失敗したら false を返す．
  bool
  open_file(const string& filename);

  /// @brief ファイルをクローズする．
  void
  close_file();

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
  LibraryHandler* mLibraryHandler;

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
DotlibParser::cur_string() const
{
  return mCurString.c_str();
}

// 現在のファイル位置
inline
FileRegion
DotlibParser::cur_loc()
{
  return FileRegion(mCurFileDesc,
		    mCurLine, mFirstColumn,
		    mCurLine, mCurColumn);
}

// 現在の行番号を返す．
inline
int
DotlibParser::cur_line() const
{
  return mCurLine;
}

// 現在のトークンの開始位置
inline
int
DotlibParser::first_column() const
{
  return mFirstColumn;
}

// 現在のコラム位置を返す．
inline
int
DotlibParser::cur_column() const
{
  return mCurColumn;
}

END_NAMESPACE_YM_CELL_DOTLIB

#endif // YM_CELL_DOTLIBPARSER_H
