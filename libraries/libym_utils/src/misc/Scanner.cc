
/// @file Scanner.cc
/// @brief Scanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/Scanner.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
// @param[in] ido 入力データ
Scanner::Scanner(IDO& ido) :
  mIDO(ido)
{
  mReadPos = 0;
  mEndPos = 0;
  mCR = false;
  mCurLine = 1;
  mCurColumn = 1;
  mFirstLine = 1;
  mFirstColumn = 1;
  mNextLine = 1;
  mNextColumn = 1;
  mNeedUpdate = true;
}

// @brief デストラクタ
Scanner::~Scanner()
{
}

// @brief peek() の下請け関数
void
Scanner::update()
{
  int c = 0;
  for ( ; ; ) {
    if ( mReadPos >= mEndPos ) {
      mReadPos = 0;
      ssize_t n = mIDO.read(mBuff, 4096);
      if ( n < 0 ) {
	// ファイル読み込みエラー
	c = -1;
	break;
      }
      mEndPos = n;
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

// @brief 直前の peek() を確定させる．
void
Scanner::accept()
{
  assert_cond( mNeedUpdate == false, __FILE__, __LINE__);
  mNeedUpdate = true;
  mCurLine = mNextLine;
  mCurColumn = mNextColumn;
  // mNextLine と mNextColumn を先に設定しておく
  if ( mNextChar == '\n' ) {
    check_line(mCurLine);
    ++ mNextLine;
    mNextColumn = 0;
  }
  ++ mNextColumn;
}

// @brief 現在の位置をトークンの最初の位置にセットする．
void
Scanner::set_first_loc()
{
  mFirstLine = mCurLine;
  mFirstColumn = mCurColumn;
}

// @brief 改行を読み込んだ時に起動する関数
// @param[in] line 行番号
// @note デフォルトではなにもしない．
void
Scanner::check_line(ymuint line)
{
}

END_NAMESPACE_YM
