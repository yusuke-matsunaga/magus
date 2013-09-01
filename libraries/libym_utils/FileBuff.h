#ifndef FILEBUFF_H
#define FILEBUFF_H

/// @file FileBuff.h
/// @brief FileBuff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include <fcntl.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileBuff FileBuff.h "FileBuff.h"
/// @brief C の標準ライブラリの FILE 構造体を模したクラス
//////////////////////////////////////////////////////////////////////
class FileBuff
{
public:

  /// @brief コンストラクタ
  /// @param[in] buff_size バッファサイズ
  FileBuff(ymuint buff_size = 4096);

  /// @brief デストラクタ
  ~FileBuff();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @param[in] flags フラグ
  /// @param[in] mode モード
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  open(const char* filename,
       int flags,
       mode_t mode = 0);

  /// @brief ファイルをクローズする．
  void
  close();

  /// @brief 適正なファイルディスクリプタを持っているとき true を返す．
  bool
  is_ready() const;

  /// @brief buff[0] - buff[num - 1] の内容を書き込む．
  /// @param[in] buff データを格納したバッファ
  /// @param[in] num 書き込むバイト数
  /// @return 実際に書き込んだバイト数を返す．
  ssize_t
  write(const ymuint8* buff,
	ymuint64 num);

  /// @brief num バイトを読み込み buff[] に格納する．
  /// @param[in] buff データを格納するバッファ
  /// @param[in] num 読み込むバイト数．
  /// @return 実際に読み込んだバイト数を返す．
  ssize_t
  read(ymuint8* buff,
       ymuint64 num);

  /// @brief num バイトを読み込む
  /// @note ただし読み込んだデータは捨てる．
  /// @param[in] num 読み込むバイト数．
  /// @return 実際に読み込んだバイト数を返す．
  ssize_t
  dummy_read(ymuint64 num);

  /// @brief バッファにデータを読みだす．
  bool
  prepare();

  /// @brief バッファの現在位置を返す．
  ymuint8*
  buff_ptr() const;

  /// @brief バッファのサイズを返す．
  /// @note 読み出しモードの場合にはバッファ中のデータ量を返す．
  /// @note 書き込みモードの場合にはバッファの空き容量を返す．
  ymuint64
  buff_size() const;

  /// @brief バッファから num バイト読み出した事にする．
  void
  peek(ymuint64 num);


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルディスクリプタ
  int mFd;

  // flush が必要なことを示すフラグ
  bool mNeedFlush;

  // バッファサイズ
  ymuint64 mBuffSize;

  // バッファ
  ymuint8* mBuff;

  // 有効なデータのサイズ ( <= mBuffSize )
  ymuint64 mDataSize;

  // 読み出し/書き込み位置
  ymuint64 mPos;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] buff_size バッファサイズ
inline
FileBuff::FileBuff(ymuint buff_size)
{
  mFd = -1;
  mBuffSize = buff_size;
  assert_cond( mBuffSize > 0 , __FILE__, __LINE__);
  mBuff = new ymuint8[mBuffSize];
  mDataSize = 0;
  mPos = mBuffSize;
  mNeedFlush = false;
}

// @brief デストラクタ
inline
FileBuff::~FileBuff()
{
  close();
  delete [] mBuff;
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @param[in] flags フラグ
// @param[in] mode モード
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
inline
bool
FileBuff::open(const char* filename,
	       int flags,
	       mode_t mode)
{
  close();
  mFd = ::open(filename, flags, mode);
  mPos = mBuffSize;
  mDataSize = mBuffSize;
  mNeedFlush = false;
  return mFd >= 0;
}

// @brief ファイルをクローズする．
inline
void
FileBuff::close()
{
  if ( mFd >= 0 ) {
    if ( mNeedFlush ) {
      ::write(mFd, reinterpret_cast<void*>(mBuff), mPos);
    }
    ::close(mFd);
  }
  mFd = -1;
  mNeedFlush = false;
}

// @brief 適正なファイルディスクリプタを持っているとき true を返す．
inline
bool
FileBuff::is_ready() const
{
  return mFd >= 0;
}

// @brief バッファの現在位置を返す．
inline
ymuint8*
FileBuff::buff_ptr() const
{
  return mBuff + mPos;
}

// @brief バッファのサイズを返す．
// @note 読み出しモードの場合にはバッファ中のデータ量を返す．
// @note 書き込みモードの場合にはバッファの空き容量を返す．
inline
ymuint64
FileBuff::buff_size() const
{
  return mDataSize - mPos;
}

// @brief バッファから num バイト読み出した事にする．
inline
void
FileBuff::peek(ymuint64 num)
{
  assert_cond( mPos + num <= mDataSize, __FILE__, __LINE__);
  mPos += num;
}

END_NAMESPACE_YM

#endif // FILEBUFF_H
