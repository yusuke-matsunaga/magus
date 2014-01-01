
/// @file LzmaDecoder.cc
/// @brief LzmaDecoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "LzmaDecoder.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LzmaDecoder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LzmaDecoder::LzmaDecoder()
{
}

// @brief デストラクタ
LzmaDecoder::~LzmaDecoder()
{
  close();
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
LzmaDecoder::open(const char* filename)
{
  bool stat = mBuff.open(filename, O_RDONLY, 0);
  if ( !stat ) {
    return false;
  }

  lzma_ret rcode = mDecompEngine.stream_decoder();
  if ( rcode != LZMA_OK ) {
    cerr << "Error in LzmaEngine::stream_decoder()" << endl;
    mBuff.close();
    return false;
  }

  return true;
}

// @brief ファイルをクローズする．
void
LzmaDecoder::close()
{
  mDecompEngine.end();
  mBuff.close();
}

// @brief 読み出せるデータがある時に true を返す．
bool
LzmaDecoder::is_ready() const
{
  return mBuff.is_ready();
}

// @brief 圧縮されたデータを伸長してバッファに書き込む．
// @param[in] buff 伸長したデータを格納するバッファ
// @param[in] size バッファの空きサイズ
ssize_t
LzmaDecoder::read(ymuint8* buff,
		  ymuint64 size)
{
  mDecompEngine.set_outbuf(buff, size);

  for ( ; ; ) {
    // 入力バッファが空なら新たなデータを読み込む．
    if ( !mBuff.prepare() ) {
      // 読み込みに失敗した．
      cerr << "Error in read()" << endl;
      return -1;
    }

    // 入力データをセットする．
    ymuint old_size = mBuff.buff_size();
    if ( old_size == 0 ) {
      // 入力の読み込みが末尾に達している．
      return 0;
    }

    mDecompEngine.set_inbuf(mBuff.buff_ptr(), old_size);

    // 伸長する．
    lzma_ret rcode = mDecompEngine.code(LZMA_RUN);

    // 今回の decompress で消費した分だけ入力バッファを空読みする．
    ymuint in_size = old_size - mDecompEngine.avail_in();
    mBuff.seek(in_size);

    if ( rcode == LZMA_OK ) {
      if ( mDecompEngine.avail_out() == 0 ) {
	// 出力バッファが満杯になったら今回の処理は終わり
	return size;
      }
    }
    else if ( rcode == LZMA_STREAM_END ) {
      // データの末尾を読んだときの処理
      return size - mDecompEngine.avail_out();
    }
    else {
      cerr << "Error in LzmaEngine::code()" << endl;
      goto error_end;
    }
  }

 error_end:
  close();
  return -1;
}

END_NAMESPACE_YM
