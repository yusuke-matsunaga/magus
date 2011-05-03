
/// @file libym_utils/FileScanner.cc
/// @brief FileScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotlibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileScanner.h"
#include "ym_utils/FileInfoMgr.h"
#include <fcntl.h>


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
FileScanner::FileScanner()
{
  init();
}

// @brief デストラクタ
FileScanner::~FileScanner()
{
  close_file();
}


// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元のファイル位置
// @return ファイルのオープンに失敗したら false を返す．
// @note parent_loc を省略したら単独のオープンとみなす．
bool
FileScanner::open_file(const string& filename,
		       const FileLoc& parent_loc)
{
  init();
  mFd = open(filename.c_str(), O_RDONLY);
  if ( mFd < 0 ) {
    return false;
  }
  mFileInfo = FileInfoMgr::new_file_info(filename, parent_loc);
  return true;
}

// ファイルをクローズする．
void
FileScanner::close_file()
{
  close(mFd);
  mFd = -1;
  mFileInfo = FileInfo();
}

// 初期化
void
FileScanner::init()
{
  mFd = -1;
  // mFileInfo を無効化するためのコード
  mFileInfo = FileInfo();
  mReadPos = 0;
  mEndPos = 0;
  mCR = false;
  mCurChar = 0;
  mCurLine = 1;
  mCurColumn = 1;
  mNextLine = 1;
  mNextColumn = 1;
  mNeedUpdate = true;
}

// @brief 次の文字を読み出す．
// @note ファイル位置の情報等は変わらない
int
FileScanner::peek()
{
  if ( mNeedUpdate ) {
    int c = 0;
    for ( ; ; ) {
      if ( mReadPos >= mEndPos ) {
	mReadPos = 0;
	if ( mFd >= 0 ) {
	  ssize_t n = read(mFd, mBuff, 4096);
	  if ( n < 0 ) {
	    // ファイル読み込みエラー
	    c = -1;
	    break;
	  }
	  mEndPos = n;
	}
	else {
	  mEndPos = 0;
	}
      }
      if ( mEndPos == 0 ) {
	c = EOF;
	break;
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
    mNeedUpdate = false;
    mNextChar = c;
  }
  return mNextChar;
}

// @brief 直前の peek() を確定させる．
void
FileScanner::accept()
{
  assert_cond( mNeedUpdate == false, __FILE__, __LINE__);
  mNeedUpdate = true;
  mCurChar = mNextChar;
  mCurLine = mNextLine;
  mCurColumn = mNextColumn;
  // mNextLine と mNextColumn を先に設定しておく
  if ( mCurChar == '\n' ) {
    ++ mNextLine;
    mNextColumn = 0;
  }
  ++ mNextColumn;
}

END_NAMESPACE_YM
