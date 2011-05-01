
/// @file libym_utils/FileScanner.cc
/// @brief FileScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotlibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileScanner.h"
#include <fcntl.h>


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
FileScanner::FileScanner()
{
  init();
}

// デストラクタ
FileScanner::~FileScanner()
{
  close_file();
}

// ファイルをセットする．
// 自動的にに clear() を呼ぶ．
// ファイルのオープンに失敗したら false を返す．
bool
FileScanner::open_file(const string& filename)
{
  init();
  mFd = open(filename.c_str(), O_RDONLY);
  if ( mFd < 0 ) {
    return false;
  }
  return true;
}

// ファイルをクローズする．
void
FileScanner::close_file()
{
  close(mFd);
  mFd = -1;
}

// 初期化
void
FileScanner::init()
{
  mFd = -1;
  mReadPos = 0;
  mEndPos = 0;
  mCR = false;
  mCurChar = 0;
  mCurLine = 1;
  mCurColumn = 0;
  mNeedUpdate = true;
}

// 一文字読み出す．
int
FileScanner::get()
{
  if ( mNeedUpdate ) {
    int c = 0;
    mUngetLine = mCurLine;
    mUngetColumn = mCurColumn;
    for ( ; ; ) {
      if ( mReadPos >= mEndPos ) {
	mReadPos = 0;
	if ( mFd >= 0 ) {
	  ssize_t n = read(mFd, mBuff, 4096);
	  if ( n < 0 ) {
	    // ファイル読み込みエラー
	    return -1;
	  }
	  mEndPos = n;
	}
	else {
	  mEndPos = 0;
	}
      }
      if ( mEndPos == 0 ) {
	return EOF;
      }
      c = mBuff[mReadPos];
      ++ mReadPos;

      // Windows(DOS)/Mac/UNIX の間で改行コードの扱いが異なるのでここで
      // 強制的に '\n' に書き換えてしまう．
      // Windows : '\r', '\n'
      // Mac     : '\r'
      // UNIX    : '\n'
      // なので '\r' を '\n' に書き換えてしまう．
      // ただし次に本当の '\n' が来たときには無視するために
      // mCR を true にしておく．
      if ( c == '\r' ) {
	mCR = true;
	c = '\n';
	break;
      }
      if ( c == '\n' ) {
	if ( mCR ) {
	  // 直前に '\r' を読んで '\n' を返していたので今の '\n' を
	  // 無視する．これが唯一ループを回る条件
	  mCR = false;
	  continue;
	}
	break;
      }
      // 普通の文字の時はそのまま返す．
      mCR = false;
      break;
    }
    if ( mCurChar == '\n' ) {
      ++ mCurLine;
      mCurColumn = 0;
    }
    mCurChar = c;
    ++ mCurColumn;
  }
  mLastLine = mCurLine;
  mLastColumn = mCurColumn;
  mNeedUpdate = true;
  return mCurChar;
}

// 一文字読み戻す．
void
FileScanner::unget()
{
  mNeedUpdate = false;
  mLastLine = mUngetLine;
  mLastColumn = mUngetColumn;
}

END_NAMESPACE_YM
