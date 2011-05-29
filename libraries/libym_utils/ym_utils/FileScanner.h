#ifndef YM_UTILS_FILESCANNER_H
#define YM_UTILS_FILESCANNER_H

/// @file ym_utils/FileScanner.h
/// @brief FileScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotlibParser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/FileInfo.h"
#include "ym_utils/FileLoc.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileScanner FileScanner.h "ym_utils/FileScanner.h"
/// @brief 位置情報のトラッキング付きのファイル入力
///
/// このクラスで行える処理は
/// - ファイルのオープン (open_file)
/// - ファイルのクローズ (close_file)
/// - 一文字読み出し     (get)
/// - 一文字の先読み     (peek)
/// - 先読みした文字の確定 (accept)
/// これ以外にトークンの開始位置を set_first_loc() で記録して
/// cur_loc() で現在の位置までの領域を求める．
//////////////////////////////////////////////////////////////////////
class FileScanner
{
public:

  /// @brief コンストラクタ
  FileScanner();

  /// @brief デストラクタ
  ~FileScanner();


public:
  //////////////////////////////////////////////////////////////////////
  // ファイル操作を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元のファイル位置
  /// @return ファイルのオープンに失敗したら false を返す．
  /// @note parent_loc を省略したら単独のオープンとみなす．
  bool
  open_file(const string& filename,
	    const FileLoc& parent_loc = FileLoc());

  /// @brief ファイルをクローズする．
  void
  close_file();


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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  void
  init();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // UNIX のファイル記述子
  int mFd;

  // ファイル情報
  FileInfo mFileInfo;

  // バッファ
  char mBuff[4096];

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

// 一文字読み出す．
inline
int
FileScanner::get()
{
  (void) peek();
  accept();
  return mNextChar;
}

// @brief 直前の set_first_loc() から現在の位置までを返す．
inline
FileRegion
FileScanner::cur_loc() const
{
  return FileRegion(mFileInfo, mFirstLine, mFirstColumn, mCurLine, mCurColumn);
}

END_NAMESPACE_YM

#endif // YM_UTILS_FILESCANNER_H
