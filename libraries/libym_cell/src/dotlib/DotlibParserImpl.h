#ifndef DOTLIBPARSERIMPL_H
#define DOTLIBPARSERIMPL_H

/// @file DotlibParserImpl.h
/// @brief DotlibParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_int.h"
#include "DotlibScanner.h"
#include "YmUtils/FileRegion.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibParserImpl DotlibParserImpl.h "DotlibParserImpl.h"
/// @brief DotlibParser の実装クラス
//////////////////////////////////////////////////////////////////////
class DotlibParserImpl
{
public:

  /// @brief コンストラクタ
  DotlibParserImpl();

  /// @brief デストラクタ
  ~DotlibParserImpl();


public:

  /// @brief ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] mgr DotlibNode を管理するオブジェクト
  /// @param[in] debug デバッグモード
  /// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
  /// @return 読み込みが成功したら true を返す．
  /// @note パース木は mgr にセットされる．
  bool
  read_file(const string& filename,
	    DotlibMgrImpl* mgr,
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
  /// @param[out] loc ファイル上の位置情報を格納する変数
  /// @param[in] symbol_mode 数字も文字とみなすモード
  /// @return トークンの型を返す．
  tTokenType
  read_token(FileRegion& loc,
	     bool symbol_mode = false);

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

  /// @brief DotlibMgrImpl* を返す．
  DotlibMgrImpl*
  mgr();

  /// @brief デバッグモードの時 true を返す．
  bool
  debug();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  DotlibScanner* mScanner;

  // DotlibNode を管理するオブジェクト
  DotlibMgrImpl* mDotlibMgr;

  // library グループを処理するハンドラ
  DotlibHandler* mLibraryHandler;

  // デバッグモード
  bool mDebug;

  // 行末のセミコロンなしを許すかどうかのフラグ
  bool mAllowNoSemi;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief トークンを一つ読み込む．
// @param[out] loc ファイル上の位置情報を格納する変数
// @param[in] symbol_mode 数字も文字とみなすモード
// @return トークンの型を返す．
inline
tTokenType
DotlibParserImpl::read_token(FileRegion& loc,
			     bool symbol_mode)
{
  return mScanner->read_token(loc, symbol_mode);
}

// @brief 直前の read_token() に対応する文字列を返す．
inline
const char*
DotlibParserImpl::cur_string() const
{
  return mScanner->cur_string();
}

// @brief 直前の read_token() に対応する整数値を返す．
// @note 型が INT_NUM でなかったときの値は不定
inline
int
DotlibParserImpl::cur_int() const
{
  return mScanner->cur_int();
}

// @brief 直前の read_token() に対応する実数値を返す．
// @note 型が FLOAT_NUM/INT_NUM でなかったときの値は不定
inline
double
DotlibParserImpl::cur_float() const
{
  return mScanner->cur_float();
}

// @brief DotlibMgrImpl* を返す．
inline
DotlibMgrImpl*
DotlibParserImpl::mgr()
{
  return mDotlibMgr;
}

END_NAMESPACE_YM_DOTLIB

#endif // DOTLIBPARSERIMPL_H
