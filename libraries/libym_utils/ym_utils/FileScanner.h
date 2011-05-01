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


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileScanner FileScanner.h "ym_utils/FileScanner.h"
/// @brief 位置情報のトラッキング付きのファイル入力
///
/// このクラスで行える処理は
/// - ファイルのオープン (open_file)
/// - ファイルのクローズ (close_file)
/// - 一文字読み出し     (get)
/// - 直前の読み出しの無効化 (unget)
///
/// で，直前の get() に対応するファイル上の位置を
/// cur_line, cur_column で返す．
//////////////////////////////////////////////////////////////////////
class FileScanner
{
public:

  /// @brief コンストラクタ
  FileScanner();

  /// @brief デストラクタ
  ~FileScanner();


public:

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @return ファイルのオープンに失敗したら false を返す．
  bool
  open_file(const string& filename);

  /// @brief ファイルをクローズする．
  void
  close_file();

  /// @brief 一文字読み出す．
  int
  get();

  /// @brief 直前の get() を無かったことにする．
  void
  unget();

  /// @brief 現在の行番号を返す．
  int
  cur_line() const;

  /// @brief 現在のコラム位置を返す．
  int
  cur_column() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  void
  init();

  /// @brief 文字を受け入れる．
  void
  accept(int c);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  ifstream mInput;

  // 直前の文字が \r の時に true となるフラグ
  bool mCR;

  // 現在の文字
  int mCurChar;

  // 現在の行番号
  ymuint32 mCurLine;

  // 現在のコラム位置
  ymuint32 mCurColumn;

  // 新しい文字を読み込む必要がある時 true となるフラグ
  bool mNeedUpdate;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 現在の行番号を返す．
inline
int
FileScanner::cur_line() const
{
  return mCurLine;
}

// 現在のコラム位置を返す．
inline
int
FileScanner::cur_column() const
{
  return mCurColumn;
}

END_NAMESPACE_YM

#endif // YM_UTILS_FILESCANNER_H
