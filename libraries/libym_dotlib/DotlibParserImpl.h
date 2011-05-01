#ifndef LIBYM_DOTLIB_DOTLIBPARSERIMPL_H
#define LIBYM_DOTLIB_DOTLIBPARSERIMPL_H

/// @file libym_dotlib/DotlibParserImpl.h
/// @brief DotlibParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotlibParser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_int.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/FileDescMgr.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/FileScanner.h"
#include "ym_utils/StrBuff.h"
#include "PtMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// DotlibParser の実装クラス
//////////////////////////////////////////////////////////////////////
class DotlibParserImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] msg_mgr メッセージを管理するオブジェクト
  DotlibParserImpl(MsgMgr& msg_mgr);

  /// @brief デストラクタ
  ~DotlibParserImpl();


public:

  /// @brief ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] debug デバッグモード
  /// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
  /// @return パース木の根のノードを返す．
  /// @note エラーが起きたら NULL を返す．
  const DotlibNode*
  read_file(const string& filename,
	    bool debug,
	    bool allow_no_semi = true);

  /// @brief 直前の read_file() で確保したパース木を解放する．
  void
  clear_node();

  /// @brief メモリ使用量のサマリを出力する．
  /// @param[in] s 出力先のストリーム
  void
  show_stats(ostream& s);


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

  /// @brief パース木を管理するオブジェクトを返す．
  PtMgr&
  pt_mgr();

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

#if 0
  /// @brief 初期化を行う．
  void
  init();
#endif

  /// @brief c が文字の時に true を返す．
  /// @note mSymbolMode が true なら数字も文字とみなす．
  bool
  is_symbol(int c);

  /// @brief 文字を受け入れる．
  void
  accept(int c);

#if 0
  /// @brief 一文字読み出す．
  int
  get();

  /// @brief 直前の get() を無かったことにする．
  void
  unget();
#endif


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージを管理するオブジェクト
  MsgMgr& mMsgMgr;

  // DotlibNode を管理するオブジェクト
  PtMgr mPtMgr;

  // library グループを処理するハンドラ
  DotlibHandler* mLibraryHandler;

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

  // ファイル入力用のオブジェクト
  FileScanner mFileScanner;

  // read_token の結果の文字列を格納する
  StrBuff mCurString;

  // 現在のトークンの開始位置
  ymuint32 mFirstColumn;

  // 現在のトークンの終了位置
  ymuint32 mLastColumn;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 直前の read_token() に対応する文字列を返す．
inline
const char*
DotlibParserImpl::cur_string() const
{
  return mCurString.c_str();
}

// 現在のファイル位置
inline
FileRegion
DotlibParserImpl::cur_loc()
{
  return FileRegion(mCurFileDesc,
		    cur_line(), mFirstColumn,
		    cur_line(), mLastColumn);
}

// 現在の行番号を返す．
inline
int
DotlibParserImpl::cur_line() const
{
  return mFileScanner.cur_line();
}

// 現在のコラム位置を返す．
inline
int
DotlibParserImpl::cur_column() const
{
  return mFileScanner.cur_column();
}

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_DOTLIBPARSERIMPL_H
