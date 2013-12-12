#ifndef YM_UTILS_SCANNER_H
#define YM_UTILS_SCANNER_H

/// @file ym_utils/Scanner.h
/// @brief Scanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/IDO.h"
#include "ym_utils/FileInfo.h"
#include "ym_utils/FileLoc.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Scanner Scanner.h "ym_utils/Scanner.h"
/// @brief 位置情報のトラッキング付きの入力データ読み出し器
///
/// このクラスで行える処理は
/// - 入力データのアタッチ (attach)
/// - 入力データのデタッチ (dettach)
/// - 一文字読み出し     (get)
/// - 一文字の先読み     (peek)
/// - 先読みした文字の確定 (accept)
/// これ以外にトークンの開始位置を set_first_loc() で記録して
/// cur_loc() で現在の位置までの領域を求める．
//////////////////////////////////////////////////////////////////////
class Scanner
{
public:

  /// @brief コンストラクタ
  /// @param[in] ido 入力データ
  Scanner(IDO& ido);

  /// @brief デストラクタ
  ~Scanner();


public:
  //////////////////////////////////////////////////////////////////////
  // 入力データ関連の操作を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オープン中のファイル情報を得る．
  const FileInfo&
  file_info() const;

  /// @brief 現在のファイル情報を書き換える．
  /// @param[in] file_info 新しいファイル情報
  /// @note プリプロセッサのプラグマなどで用いることを想定している．
  /// @note 通常は使わないこと．
  void
  set_file_info(const FileInfo& file_info);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一文字読み出す．
  /// @note 実際には peek(); acept() と等価
  int
  get();

  /// @brief 次の文字を読み出す．
  /// @note ファイル位置の情報等は変わらない
  int
  peek();

  /// @brief 直前の peek() を確定させる．
  void
  accept();

  /// @brief 現在の位置をトークンの最初の位置にセットする．
  void
  set_first_loc();

  /// @brief 直前の set_first_loc() から現在の位置までを返す．
  FileRegion
  cur_loc() const;

  /// @brief 改行を読み込んだ時に起動する関数
  /// @param[in] line 行番号
  /// @note デフォルトではなにもしない．
  virtual
  void
  check_line(ymuint line);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief peek() の下請け関数
  void
  update();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力データ
  IDO& mIDO;

  // バッファ
  ymuint8 mBuff[4096];

  // バッファ中の読み出し位置
  ymuint32 mReadPos;

  // バッファの末尾
  ymuint32 mEndPos;

  // 直前の文字が \r の時に true となるフラグ
  bool mCR;

  // 現在の行番号
  ymuint32 mCurLine;

  // 現在のコラム位置
  ymuint32 mCurColumn;

  // トークンの最初の行番号
  ymuint32 mFirstLine;

  // トークンの最初のコラム位置
  ymuint32 mFirstColumn;

  // peek() した文字
  ymint16 mNextChar;

  // peek() した文字の行番号
  ymuint32 mNextLine;

  // peek() した文字のコラム位置
  ymuint32 mNextColumn;

  // 新しい文字を読み込む必要がある時 true となるフラグ
  bool mNeedUpdate;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief オープン中のファイル情報を得る．
inline
const FileInfo&
Scanner::file_info() const
{
  return mIDO.file_info();
}

// @brief 現在のファイル情報を書き換える．
// @param[in] new_info 新しいファイル情報
// @note プリプロセッサのプラグマなどで用いることを想定している．
// @note 通常は使わないこと．
inline
void
Scanner::set_file_info(const FileInfo& file_info)
{
  mIDO.set_file_info(file_info);
}

// @brief 次の文字を読み出す．
// @note ファイル位置の情報等は変わらない
inline
int
Scanner::peek()
{
  if ( mNeedUpdate ) {
    update();
  }
  return mNextChar;
}

// 一文字読み出す．
inline
int
Scanner::get()
{
  (void) peek();
  accept();
  return mNextChar;
}

// @brief 直前の set_first_loc() から現在の位置までを返す．
inline
FileRegion
Scanner::cur_loc() const
{
  return FileRegion(file_info(), mFirstLine, mFirstColumn, mCurLine, mCurColumn);
}

END_NAMESPACE_YM

#endif // YM_UTILS_FILESCANNER_H
