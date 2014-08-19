
/// @file FileBuff.cc
/// @brief FileBuff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FileBuff.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// クラス FileBuff
//////////////////////////////////////////////////////////////////////

// @brief buff[0] - buff[num - 1] の内容を書き込む．
// @param[in] buff データを格納したバッファ
// @param[in] num 書き込むバイト数
// @return 実際に書き込んだバイト数を返す．
ssize_t
FileBuff::write(const ymuint8* buff,
		ymuint64 num)
{
  if ( mFd < 0 ) {
    return 0;
  }

  ymuint64 count = 0;
  while ( num > 0 ) {
    // 一度に書き込めるサイズを num1 に入れる．
    ymuint64 num1 = num;
    if ( mPos + num1 > mBuffSize ) {
      // バッファサイズの関係でこれだけしか書けない．
      num1 = mBuffSize - mPos;
    }

    // mBuff に転送する．
    memcpy(reinterpret_cast<void*>(mBuff + mPos),
	   reinterpret_cast<const void*>(buff),
	   num1);

    // 諸元を更新する．
    mPos += num1;
    count += num1;
    buff += num1;
    num -= num1;
    mNeedFlush = true;

    if ( mPos == mBuffSize ) {
      // バッファが満杯になったので実際の書き込みを行う．
      ymuint tmp_size = mBuffSize;
      ymuint8* tmp_buff = mBuff;
      while ( tmp_size > 0 ) {
	ssize_t n = ::write(mFd, reinterpret_cast<void*>(tmp_buff), tmp_size);
	if ( n <= 0 ) {
	  // 書き込みが失敗した．
	  return -1;
	}
	tmp_buff += n;
	tmp_size -= n;
      }

      mPos = 0;
      mNeedFlush = false;
    }
  }

  return count;
}

// @brief num バイトを読み込み buff[] に格納する．
// @param[in] buff データを格納するバッファ
// @param[in] num 読み込むバイト数．
// @return 実際に読み込んだバイト数を返す．
ssize_t
FileBuff::read(ymuint8* buff,
	       ymuint64 num)
{
  if ( mFd < 0 ) {
    return 0;
  }

  ymuint64 count = 0;
  while ( num > 0 ) {
    if ( !prepare() ) {
      return -1;
    }
    if ( mDataSize == 0 ) {
      break;
    }

    // 一度に読み出せるサイズを num1 に入れる．
    ymuint64 num1 = num;
    if ( mPos + num1 > mDataSize ) {
      // バッファサイズの関係でこれしか読み出せない．
      num1 = mDataSize - mPos;
    }

    // buff に転送する．
    memcpy(reinterpret_cast<void*>(buff),
	   reinterpret_cast<void*>(mBuff + mPos),
	   num1);

    // 諸元を更新する．
    mPos += num1;
    count += num1;
    buff += num1;
    num -= num1;
  }

  return count;
}

// @brief num バイトを読み込む
// @note ただし読み込んだデータは捨てる．
// @param[in] num 読み込むバイト数．
// @return 実際に読み込んだバイト数を返す．
ssize_t
FileBuff::dummy_read(ymuint64 num)
{
  // read() との違いは中央の memcpy() がないだけ．

  if ( mFd < 0 ) {
    return 0;
  }

  ymuint64 count = 0;
  while ( num > 0 ) {
    if ( !prepare() ) {
      return -1;
    }

    // 一度に読み出せるサイズを num1 に入れる．
    ymuint64 num1 = num;
    if ( mPos + num1 > mDataSize ) {
      // バッファサイズの関係でこれしか読み出せない．
      num1 = mDataSize - mPos;
    }

    // 諸元を更新する．
    mPos += num1;
    count += num1;
    num -= num1;
  }

  return count;
}

// @brief バッファにデータを読みだす．
bool
FileBuff::prepare()
{
  if ( mPos == mDataSize ) {
    // バッファが空なら実際に読み込む．
    ssize_t n = ::read(mFd, reinterpret_cast<void*>(mBuff), mBuffSize);
    if ( n < 0 ) {
      perror("FileBuff::prepare()");
      return false;
    }

    // n == 0 の場合もこのままでよい．

    mPos = 0;
    mDataSize = static_cast<ymuint32>(n);
  }
  return true;
}

END_NAMESPACE_YM
